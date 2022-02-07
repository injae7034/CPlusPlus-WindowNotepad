#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandCreator.h"
#include "KeyActionCreator.h"
#include "Command.h"
#include "Glyph.h"
#include "CaretController.h"
#include "TextExtent.h"
#include "File.h"
#include "afxdlgs.h"//CFileDialog헤더파일
#include "KeyAction.h"

#include "ScrollActionCreator.h"
#include "ScrollAction.h"
#include "ScrollController.h"
#include "HorizontalScroll.h"
#include "VerticalScroll.h"
#include "DummyRow.h"
#include "PageMoveController.h"
#include "RowAutoChange.h"

#include "PrintInformation.h"
#include "CommandHistory.h"
#include "TextingOutVisitor.h"
#include "SelectingTexts.h"
#include "PreviewForm.h"
#include "GlyphFinder.h"

//#include <htmlhelp.h>
//#pragma comment(lib, "htmlhelp")

HHOOK hSaveMessageBoxHook;//전역변수 선언

BEGIN_MESSAGE_MAP(NotepadForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()//더블버퍼링을 막기 위한 조치
	ON_MESSAGE(WM_IME_COMPOSITION, OnComposition)
	ON_MESSAGE(WM_IME_CHAR, OnImeChar)
	ON_MESSAGE(WM_IME_STARTCOMPOSITION, OnStartCompostion)
	ON_COMMAND_RANGE(IDM_FILE_OPEN, ID_ONCHARCOMMAND, OnCommand)
	ON_WM_MENUSELECT(IDR_MENU1, OnMenuSelect)
	//ON_REGISTERED_MESSAGE() //찾기공통대화상자에서 부모윈도우로 메세지를 전달하기 위해 필요함
	ON_WM_KEYDOWN()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

//NotepadForm생성자
NotepadForm::NotepadForm()
	//여기서 콜론초기화로 따로지정안하면 Font()와 Caret()의 기본생성자가 호출됨
	//왜냐하면 NotepadForm이 멤버로 font와 caret을 가지고 있기때문에 notepadForm이 생성되면서
	//font와 caret의 기본생성자가 호출되어 생성됨. 그렇기 때문에 Font와 Caret의
	//기본생성자 Font()와 Caret()이 필요함.
{
	this->note = NULL;//NULL로 초기화시킴.
	this->current = NULL;//NULL로 초기화시킴.
	this->isComposing = false;//false로 초기화시킴
	this->isDirty = false;//false로 초기화시킴
	this->isSelecting = false;//false로 초기화시킴
	this->isRowAutoChanging = false;//false로 초기화 시킴.
	this->isMouseLButtonClicked = false;//false로 초기화 시킴.
	this->fileName = "제목 없음";
	this->filePath = "";
	this->previousPageWidth = 0;//처음생성될때는 현재 화면 너비를 0으로 초기화해줌
	this->selectedStartXPos = 0;//처음생성될때는 선택된 texts가 없기 때문에 0으로 초기화해줌
	this->selectedStartYPos = 0;//처음생성될때는 선택된 texts가 없기 때문에 0으로 초기화해줌
	this->glyph = 0;//메모장이 처음 생성되면 글자가 없기 때문에 0으로 초기화해줌
	this->removedSelectedTexts = 0;//메모장이 처음 생성되면 지워진 선택된 글자가 없기 때문에 0으로 초기화함
	this->textExtent = NULL;
	this->selectingTexts = NULL;
	this->printInformation = NULL;
	this->previewForm = NULL;
	this->pageSetUpInformation = NULL;
	this->caretController = 0;
}

//메모장 윈도우가 생성될 때
int NotepadForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CFrameWnd::OnCreate(lpCreateStruct);
	this->icon = AfxGetApp()->LoadIcon(IDI_ICON1); ///< 변경된 아이콘
	this->SetIcon(this->icon, ICON_SMALL);
	//HWND hwnd = (HWND)HtmlHelpA(GetDesktopWindow(), "notepad.chm", HH_DISPLAY_TOPIC, NULL);
	//HWND hwnd = (HWND)HtmlHelpA(this->GetSafeHwnd(), "notepad.chm", HH_DISPLAY_TOPIC, NULL);

	//1. glyphCreator를 만든다.
	GlyphCreator glyphCreator;
	//2. 클립보드를 만든다.
	this->clipboard = glyphCreator.Create((char*)"clipboard");
	//3. 노트를 만든다.
	this->note = glyphCreator.Create((char*)"\0");
	//4. 줄을 만든다.
	Glyph* row = glyphCreator.Create((char*)"\n");
	//5. 줄을 메모장에 추가한다.
	Long rowIndex;
	rowIndex = this->note->Add(row);
	//6. 현재 줄의 위치를 저장한다.
	this->current = this->note->GetAt(rowIndex);
	//7. 현재 화면의 가로 길이를 저장한다.
	CRect rect;
	this->GetClientRect(&rect);
	this->previousPageWidth = rect.Width();
	//8. CMenu를 notepadForm에 연결한다.
	this->menu.LoadMenu(IDR_MENU1);
	SetMenu(&this->menu);
	//마우스 오른쪽 버튼을 클릭했을 때 메뉴를 notepadForm에 연결한다.
	this->mouseRButtonMenu.LoadMenu(IDR_MENU3);
	//9. CMenu의 메뉴들이 자동으로 Enable되는 것을 막기 위해 FALSE 처리를 해줘야함
	//아니면 다른 곳에서 Unenable시켜도 계속해서 자동으로 Enable시켜버림!
	this->m_bAutoMenuEnable = FALSE;
	//10. 복사하기, 잘라내기, 삭제 메뉴를 비활성화시킨다. 비활성화가 디폴트고 선택영역이 생기면 활성화시켜줌!
	this->menu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->menu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->menu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	//실행취소, 다시실행 메뉴를 비활성화 시킨다.
	this->menu.EnableMenuItem(IDM_NOTE_UNDO, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->menu.EnableMenuItem(IDM_NOTE_REDO, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	//마우스클릭 메뉴의 복사하기, 잘라내기, 삭제, 실행취소, 다시 실행 메뉴를 비활성화시킨다.
	this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_UNDO, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_REDO, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	//11. 외부클립보드에 현재 문자열이 저장되어있으면
	unsigned int priority_list = CF_TEXT;
	if (GetPriorityClipboardFormat(&priority_list, 1) == CF_TEXT)
	{
		//11.1 붙여넣기 메뉴를 활성화시켜준다.
		this->menu.EnableMenuItem(IDM_NOTE_PASTE, MF_BYCOMMAND | MF_ENABLED);
		this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_PASTE, MF_BYCOMMAND | MF_ENABLED);
	}
	//12. 외부클립보드에 현재 문자열이 저장되어 있지않으면
	else if (GetPriorityClipboardFormat(&priority_list, 1) == NULL)
	{
		//12.1 붙여넣기 메뉴를 비활성화시킨다.
		this->menu.EnableMenuItem(IDM_NOTE_PASTE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_PASTE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
	//CFont에서 사용하고자 하는 글자크기와 글자체로 초기화시킴.
	//기본생성자로 생성된 this->font에 매개변수 5개생성자로 치환(=)시킴
	CFont font;
	CDC* dc = this->GetDC();
	Long size = AfxGetApp()->GetProfileInt("NotepadSection", "FontSize", 200);
	CString style = AfxGetApp()->GetProfileString("NotepadSection", "FontStyle", "맑은 고딕");
	font.CreatePointFont(size, style, dc);
	LOGFONT logFont;
	font.GetLogFont(&logFont);
	this->font = Font(logFont, size, RGB(0, 0, 0));
	//13. textExtent를 힙에 할당한다.
	this->textExtent = new TextExtent(this);
	//14. 처음 만들어지는 메모장 이름을 정한다.
	string name = this->fileName;
	name += " - 메모장";
	SetWindowText(CString(name.c_str()));
	//19. scrollController를 생성한다.
	this->scrollController = new ScrollController(this);
	//21. pageMoveController를 생성한다.
	this->pageMoveController = new PageMoveController(this);
	//22. selectingTexts를 생성한다.
	this->selectingTexts = new SelectingTexts(this);
	//23. CFindReplaceDialog를 초기화해준다.
	this->findReplaceDialog = 0;
	//24. CommandHistory를 생성한다.
	this->commandHistory = new CommandHistory(this);
	//25. 자동개행여부를 프로그램이 꺼져도 저장하기 위해서 레지스트리에 NotepadSection파일을 만들고
	//IsRowAutoChanging 값을 저장함.
	this->isRowAutoChanging = (bool)!AfxGetApp()->GetProfileInt(
		"NotepadSection", "IsRowAutoChanging", 1);
	//26. 레지스트리에 NotepadSection파일에 저장된 IsRowAutoChanging 값으로 RowAutoChange에
	//메세지를 보낼 지 말지 결정함.
	if (this->isRowAutoChanging == true)
	{
		this->SendMessage(WM_COMMAND, IDM_ROW_AUTOCHANGE);
	}

	return 0;
}

//키보드에 글자를 입력할 때
void NotepadForm::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. Ctrl키가 눌러졌는지 안눌러졌는지 상태를 체크해서 저장한다.
	Long ctrlPressedCheck = GetKeyState(VK_CONTROL);
	//백스페이스가 아니면(백스페이키는 OnKeyDown을 먼저 실행하고 OnChar로 들어온다)
	//백스페이스인경우 OnChar에서 아무 처리도 안해주고 바로 나가면 된다!
	//단어단위로 왼쪽으로 지울 때, OnKeyDown에 SendMessage를 보낼 때, 인위적으로 Ctrl키를 누른 채로
	//VK_BACK을 보냈기 때문에, 여기서 OnCharCommand가 실행되지 않도록 막기 위해서
	//ctrlPressedCheck & 0x8000 && nChar != VK_BACK 조건문이 필요하다.
	if (nChar != VK_BACK && nChar != 27)
	{
		if (!(ctrlPressedCheck & 0x80))
		{
			//1.1.1 glyphCreator를 생성한다.
			GlyphCreator glyphCreator;
			//1.1.2 glyph를 생성해서 저장한다.
			this->glyph = glyphCreator.Create((char*)&nChar);
			//1.1.3 OnCommand로 메세지를 보낸다.
			this->SendMessage(WM_COMMAND, ID_ONCHARCOMMAND);
		}

	}
}

//메모장에 텍스트를 출력할 떄//출력시 Visitor패턴적용
void NotepadForm::OnPaint()
{
	//1. CPaintDC를 생성한다.
	CPaintDC dc(this);

	// 더블버퍼링을 막기 위한 조치
	CRect rect;
	this->GetClientRect(&rect);
	CDC dcTemp;
	dcTemp.CreateCompatibleDC(&dc);
	HBITMAP hbmp = ::CreateCompatibleBitmap(dc, rect.right, rect.bottom);
	HBITMAP oldBMP = (HBITMAP)dcTemp.SelectObject(hbmp);
	dcTemp.FillRect(&rect, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	
	//2. 메모장에 텍스트를 출력할 클래스를 생성한다.
	TextingOutVisitor textingOutVisitor = TextingOutVisitor(this, &dcTemp, 0, 0);
	//3. 메모장에 텍스트를 출력한다.
	this->note->Accept(&textingOutVisitor);
	
	// 더블버퍼링을 막기 위한 조치
	dc.BitBlt(0, 0, rect.right, rect.bottom, &dcTemp, 0, 0, SRCCOPY);
	dcTemp.SelectObject(oldBMP);
	::DeleteObject(hbmp);
	dcTemp.DeleteDC();
}

//한글을 입력받을 때
LRESULT NotepadForm::OnComposition(WPARAM wParam, LPARAM lParam)
{
	//1. glyphCreator를 생성한다.
	GlyphCreator glyphCreator;
	WORD word = LOWORD(wParam);
	//2. 키보드로부터 입력받을 정보를 바탕으로 새로운 한글을 저장한다.
	char koreanLetter[3];
	koreanLetter[0] = HIBYTE(word);
	koreanLetter[1] = LOBYTE(word);
	koreanLetter[2] = '\0';
	//3. 현재 줄의 캐럿의 가로 위치를 구한다.
	Long index = this->current->GetCurrent();
	//4. IsComposing값이 '참'이면(한글이 조립중인 상태이면)
	if (this->isComposing == true)
	{
		//4.1 현재 줄의 캐럿의 가로 위치 바로 앞에 있는 기존 한글을 지운다.
		//그러기 위해서는 캐럿의 현재 가로 위치에 1감소한 값을 넣어주면 된다.
		//기존 한글을 지워야 새로 입력 받은 한글을 대체할 수 있다.
		this->current->Remove(index - 1);
		//4.2 갱신된 current의 위치를 index에 저장한다.
		index = this->current->GetCurrent();
	}
	//5. 현재위치의 한글을 지웠기 때문에 한글이 조립중이 아님으로 상태를 변경한다.
	this->isComposing = false;
	//6. 새로운 한글이 입력되었으면(한글 조립중에 글자를 다 지워버리면 '\0'문자로
	//OnComposition에 입력된다 백스페이스키가 입력되면 기존 한글이 지워지고 '\0'가 들어 오게 된다.)
	if (koreanLetter[0] != '\0')
	{
		//6.1 doubleByteLetter를 생성한다.
		Glyph* doubleByteLetter = glyphCreator.Create((char*)koreanLetter);
		//6.2 index가 현재 줄의 length와 같으면
		if (index == this->current->GetLength())
		{
			//6.2.1 현재 줄의 마지막 글자 뒤에 새로운 한글을 추가한다.
			index = this->current->Add(doubleByteLetter);
		}
		//6.3 index가 현재 줄의 length와 다르면
		else
		{
			//6.3.1 현재 줄의 index번째에 새로운 한글을 끼워 쓴다.
			index = this->current->Add(index, doubleByteLetter);
		}
		//6.4 한글을 현재 위치에 추가했기때문에 한글이 조립중인 상태로 변경한다.
		this->isComposing = true;
		//BackSpace와 Delete키와 별도로 한글조립중에 지우는 경우도 OnSize로 보내줘야 한다.
		//7.1 자동 줄 바꿈이 진행중이면
		if (this->isRowAutoChanging == true)
		{
			//7.1.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
			//OnSize에서 부분자동개행을 하도록 한다. 
			this->SendMessage(WM_SIZE);
		}
	}
	//7. 한글 조립중에 백스페이스키룰 눌러서 조립 중인 한글을 지워버리면
	else
	{
		//BackSpace와 Delete키와 별도로 한글조립중에 지우는 경우도 OnSize로 보내줘야 한다.
		//7.1 자동 줄 바꿈이 진행중이면
		if (this->isRowAutoChanging == true)
		{
			//7.1.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
			//OnSize에서 부분자동개행을 하도록 한다. 
			this->SendMessage(WM_SIZE);
		}
	}
	//8. 캐럿의 위치와 크기가 변경되었음을 알린다.
	this->Notify();
	//9. 메모장 제목에 *를 추가한다.
	string name = this->fileName;
	name.insert(0, "*");
	name += " - 메모장";
	SetWindowText(CString(name.c_str()));
	//10. 메모장에 변경사항이 있음을 저장한다.
	this->isDirty = true;
	//11. 갱신한다.
	this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	return ::DefWindowProc(this->m_hWnd, WM_IME_COMPOSITION, wParam, lParam);
}

//완성된 한글을 입력받았을 때
LRESULT NotepadForm::OnImeChar(WPARAM wParam, LPARAM lParam)
{
	//1. glyphCreator를 생성한다.
	GlyphCreator glyphCreator;
	//2. 매개변수로 입력받은 wParam을 통해 한글을 버퍼에 옮긴다.
	WORD word = LOWORD(wParam);
	char koreanLetter[3];
	koreanLetter[0] = HIBYTE(word);
	koreanLetter[1] = LOBYTE(word);
	koreanLetter[2] = '\0';
	//3. doubleByteLetter를 생성한다.
	this->glyph = glyphCreator.Create((char*)koreanLetter);
	//4. OnCommand로 메세지를 보낸다.
	this->SendMessage(WM_COMMAND, ID_ONIMECHARCOMMAND);
	//5. 지운 선택된 영역을 OnImeCharCommand로 보냈기 때문에 초기화한다.
	this->removedSelectedTexts = 0;

	return 0;
}

//한글 조립 시작을 알림
LRESULT NotepadForm::OnStartCompostion(WPARAM wParam, LPARAM lParam)
{
	//1. 메모장에서 선택된 texts가 있으면
	if (this->isSelecting == true)
	{
		//1.1 선택이 시작되는 줄과 글자 위치, 선택이 끝나는 줄과 글자 위치를 저장한다.
		Long selectedStartRowPos = this->selectedStartYPos;//선택이 시작되는 줄 위치
		Long selectedStartLetterPos = this->selectedStartXPos;//선택이 시작되는 글자 위치
		Long selectedEndRowPos = this->note->GetCurrent();//선택이 끝나는 줄 위치
		Long selectedEndLetterPos = this->current->GetCurrent();//선택이 끝나는 글자 위치
		//1.2 content를 복사하고 지운다.
		this->removedSelectedTexts = this->note->CopySelectedTextsAndRemove(selectedStartRowPos,
			selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
		//1.3 메모장 제목에 *를 추가한다.
		string name = this->fileName;
		name.insert(0, "*");
		name += " - 메모장";
		this->SetWindowText(CString(name.c_str()));
		//1.4 메모장에 변경사항이 있음을 저장한다.
		this->isDirty = true;
		//1.5 연산이 끝났기 때문에 현재 줄의 위치를 다시 조정해준다.(note의연산안에서 현재 줄의 위치와 글자 위치는
		//조정이 되지만 notepadForm의 current(현재줄)는 조정할 수 없어서 notepadForm에서 해준다.)
		this->current = this->note->GetAt(this->note->GetCurrent());
		//1.6 자동 줄 바꿈 메뉴가 체크되어 있으면
		if (this->isRowAutoChanging == true)
		{
			//1.6.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
			//OnSize에서 부분자동개행을 하도록 한다. 
			this->SendMessage(WM_SIZE);
		}
		//1.7 메모장에서 선택된 texts를 다 지웠기 때문에 메모장에서 선택이 안된 상태로 바꾼다.
		this->isSelecting = false;
		//1.8 선택이 끝났기 때문에 캐럿의 x좌표를 0으로 저장한다.
		this->selectedStartXPos = 0;
		//1.9 선택이 끝났기 때문에 캐럿의 y좌표를 0으로 저장한다.
		this->selectedStartYPos = 0;
		//1.10 복사하기, 잘라내기, 삭제 메뉴를 비활성화 시킨다.
		this->menu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->menu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->menu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}

	return 0;
}

//메모장이 Focus를 얻을 때
void NotepadForm::OnSetFocus(CWnd* pOldWnd)
{
	//1. 캐럿 매니저를 생성한다.
	this->caretController = new CaretController(this);
	//2. 캐럿이 변경되었음을 옵저버들에게 알린다.
	this->Notify();
}

//메모장이 Focus를 잃을 때
void NotepadForm::OnKillFocus(CWnd* pNewWnd)
{
	//1. CaretController를 구독해제한다.(여기서 할당해제도 같이됨)
	Subject::Detach(this->caretController);
	this->caretController = 0;
}

//Command패턴
void NotepadForm::OnCommand(UINT nId)
{
	//1. CommandCreator를 생성한다.
	CommandCreator commandCreator(this);
	//2. ConcreteCommand를 생성한다.
	Command* command = commandCreator.Create(nId);
	bool isStop = false;
	//3. command가 NULL이 아니면
	if (command != NULL)
	{
		//3.1 ConcreteCommand의 execute 함수를 실행한다.
		command->Execute();
		//3.2 글자를 입력하는 command이면 
		if (nId == ID_ONCHARCOMMAND || nId == ID_ONIMECHARCOMMAND)
		{
			//3.2.1 text를 입력하는 command는 멈추지 않는다는 표시를 한다.
			isStop = false;
			//3.2.2 UndoList에 추가한다.
			this->commandHistory->PushUndoList(command, isStop);
			//3.2.3 실행취소를 활성화 시킨다.
			this->menu.EnableMenuItem(IDM_NOTE_UNDO, MF_BYCOMMAND | MF_ENABLED);
			this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_UNDO, MF_BYCOMMAND | MF_ENABLED);
			//3.2.4 redoList를 초기화시킨다.
			this->commandHistory->MakeRedoListEmpty();
			//3.2.5 다시 실행을 비활성화시킨다.
			this->menu.EnableMenuItem(IDM_NOTE_REDO, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_REDO, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		}
		//3.3 글자를 지우는 command이거나 붙여넣기 command이면
		else if (nId == ID_BACKSPACEKEYACTIONCOMMAND || nId == ID_DELETEKEYACTIONCOMMAND
			|| nId == ID_CTRLBACKSPACEKEYACTIONCOMMAND || nId == ID_CTRLDELETEKEYACTIONCOMMAND
			|| nId == ID_SHIFTCTRLBACKSPACEKEYACTIONCOMMAND ||
			nId == ID_SHIFTCTRLDELETEKEYACTIONCOMMAND || nId == IDM_NOTE_PASTE)
		{
			//3.3.1 멈춘다는 표시를 한다.
			isStop = true;
			//3.3.2 Command에 변경사항이 있으면
			if (command->IsDirty() == true)
			{
				//3.3.2.1 UndoList에 추가한다.
				this->commandHistory->PushUndoList(command, isStop);
				//3.3.2.2 실행취소를 활성화 시킨다.
				this->menu.EnableMenuItem(IDM_NOTE_UNDO, MF_BYCOMMAND | MF_ENABLED);
				this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_UNDO, MF_BYCOMMAND | MF_ENABLED);
				//3.3.2.3 redoList를 초기화시킨다.
				this->commandHistory->MakeRedoListEmpty();
				//3.3.2.4 다시 실행을 비활성화시킨다.
				this->menu.EnableMenuItem(IDM_NOTE_REDO, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
				this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_REDO, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			}
			//3.3.3 Command에 변경사항이 없으면
			else
			{
				//3.3.3.1 command를 할당해제한다.
				if (command != 0)
				{
					delete command;
				}
			}
		}
		//3.4 선택영역을 지우거나 선택영역을 잘라내거나 바꾸는 command이면
		else if (nId == IDM_NOTE_REMOVE || nId == IDM_NOTE_CUT
			|| nId == ID_ONREPLACEBUTTONCLICKEDCOMMAND
			|| nId == ID_ONREPLACEALLBUTTONCLICKEDCOMMAND)
		{
			//3.4.1 멈춘다는 표시를 한다.
			isStop = true;
			//3.4.2 UndoList에 추가한다.
			this->commandHistory->PushUndoList(command, isStop);
			//3.4.3 실행취소를 활성화 시킨다.
			this->menu.EnableMenuItem(IDM_NOTE_UNDO, MF_BYCOMMAND | MF_ENABLED);
			this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_UNDO, MF_BYCOMMAND | MF_ENABLED);
			//3.4.4 redoList를 초기화시킨다.
			this->commandHistory->MakeRedoListEmpty();
			//3.4.5 다시 실행을 비활성화시킨다.
			this->menu.EnableMenuItem(IDM_NOTE_REDO, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_REDO, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		}
		//3.5 글자를 입력하는 command나 글자를 지우는 command가 아니면
		//undoList에 들어간 command가 아니면 따로 할당해제를 해줘야 메모리 누수가 안생긴다.
		//undoList에 들어간 command는 commandHistory가 소멸될 때, 소멸자에서 같이 할당해제된다.
		else
		{
			//3.5.1 command를 할당해제한다.
			if (command != 0)
			{
				delete command;
			}
		}
	}
	//4. 변화를 메모장에 갱신한다.
	this->Notify();
	this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

//메모장에서 키보드로 이동하기
void NotepadForm::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//3.1 KeyActionCreator를 생성한다.
	KeyActionCreator keyActionCreator(this);
	//3.2 ConcreteKeyAction을 생성한다.
	KeyAction* keyAction = keyActionCreator.Create(nChar);
	//3.3 keyAction이 NULL이 아니면
	if (keyAction != NULL)
	{
		//3.3.1 ConcreteKeyAction의 OnKeyDown 함수를 실행한다.
		keyAction->OnKeyDown(nChar, nRepCnt, nFlags);
		//3.3.2 keyAction을 할당해제한다.
		delete keyAction;
		//3.3.3 메모장에 선택이 안되어 있으면
		if (this->isSelecting == false)
		{
			this->menu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			this->menu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			this->menu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		}
		//3.3.4 메모장에 선택이 되어 있으면
		else
		{
			this->menu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
			this->menu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
			this->menu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
			this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
			this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
			this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
		}
		//3.3.5 변화를 메모장에 갱신한다.
		//if 구조안에서 Notify를 해줘야 Ctrl이나 Shift, Alt Capslock과 같은 특수기능키가 눌렸을 때
		//Notify를 호출해 캐럿이 있는 곳으로 스크롤이 이동하지 않는다. OnKeyDown은 키보드키 중 어떠한
		//키가 눌려져도 호출되기 때문에 원하는 keyAction이 아닌경우 Notify가 실행되지 않게 해야한다!
		this->Notify();
		this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}
}

//메모장에서 세로 스크롤을 클릭할 때
void NotepadForm::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. pageMoveController가 할당되어 있으면
	if (this->pageMoveController != 0)
	{
		//1.1 pageMoveController를 구독해제(할당해제)한다.
		Subject::Detach(this->pageMoveController);
		//1.2 댕글링 포인터를 없애기 위해 0을 저장한다.
		this->pageMoveController = 0;
	}
	//2. scrollActionCreator를 생성한다.
	ScrollActionCreator scrollActionCreator(this);
	//3. concreteScrollAction을 생성한다.
	ScrollAction* scrollAction = scrollActionCreator.Create(nSBCode);
	//4. scrollAction이 NULL이 아니면
	if (scrollAction != NULL)
	{
		//4.1 수직scroll의 정보를 구한다.
		SCROLLINFO scrollInfo;
		this->GetScrollInfo(SB_VERT, &scrollInfo);
		//4.2 ConcreteScrollAction의 OnVScroll 함수를 실행한다.
		//여기서 nPos를 매개변수로 넣으면 값이 커졌을 때 범위를 벗어나서 ThumbPositionScrollAction에서
		//nPos를 이용하면 이동이 일어나지 않는다. scrollInfo.nTrackPos를 이용해야 값이 커져도
		//제대로 된 이동이 일어난다.
		scrollAction->OnVScroll(nSBCode, scrollInfo.nTrackPos, pScrollBar);
		//4.3 scrollAction을 할당해제한다.
		delete scrollAction;
	}
	//5. 변경사항을 옵저버들에게 알린다.
	this->Notify();
	this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	//6. pageMoveController가 할당해제되어있으면
	if (this->pageMoveController == 0)
	{
		//6.1 pageMoveController를 다시 할당해준다.
		this->pageMoveController = new PageMoveController(this);
	}
}

//메모장에서 가로 스크롤을 클릭할 때
void NotepadForm::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. pageMoveController가 할당되어 있으면
	if (this->pageMoveController != 0)
	{
		//1.1 pageMoveController를 구독해제(할당해제)한다.
		Subject::Detach(this->pageMoveController);
		//1.2 댕글링 포인터를 없애기 위해 0을 저장한다.
		this->pageMoveController = 0;
	}
	//2. scrollActionCreator를 생성한다.
	ScrollActionCreator scrollActionCreator(this);
	//3. concreteScrollAction을 생성한다.
	ScrollAction* scrollAction = scrollActionCreator.Create(nSBCode);
	//4. scrollAction이 NULL이 아니면
	if (scrollAction != NULL)
	{
		//4.1 수평scroll의 정보를 구한다.
		SCROLLINFO scrollInfo;
		this->GetScrollInfo(SB_HORZ, &scrollInfo);
		//4.2 ConcreteScrollAction의 OnHScroll 함수를 실행한다.
		//여기서 nPos를 매개변수로 넣으면 값이 커졌을 때 범위를 벗어나서 ThumbPositionScrollAction에서
		//nPos를 이용하면 이동이 일어나지 않는다. scrollInfo.nTrackPos를 이용해야 값이 커져도
		//제대로 된 이동이 일어난다.
		scrollAction->OnHScroll(nSBCode, scrollInfo.nTrackPos, pScrollBar);
		//4.3 scrollAction을 할당해제한다.
		delete scrollAction;
	}
	//5. 변경사항을 옵저버들에게 알린다.
	this->Notify();
	this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	//6. pageMoveController가 할당해제되어있으면
	if (this->pageMoveController == 0)
	{
		//6.1 pageMoveController를 다시 할당해준다.
		this->pageMoveController = new PageMoveController(this);
	}
}

//메모장에서 마우스 휠로 스크롤 이동할 때
BOOL NotepadForm::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	//1. pageMoveController가 할당되어 있으면
	if (this->pageMoveController != 0)
	{
		//1.1 pageMoveController를 구독해제(할당해제)한다.
		Subject::Detach(this->pageMoveController);
		//1.2 댕글링 포인터를 없애기 위해 0을 저장한다.
		this->pageMoveController = 0;
	}
	//2. scrollActionCreator를 생성한다.
	ScrollActionCreator scrollActionCreator(this);
	//3. 휠을 아래로 움직였으면
	UINT nSBCode;
	if (zDelta < 0)
	{
		nSBCode = 1;
	}
	//4. 휠을 위로 움직였으면
	else
	{
		nSBCode = 0;
	}
	//5. concreteScrollAction을 생성한다.
	ScrollAction* scrollAction = scrollActionCreator.Create(nSBCode);
	//6. scrollAction이 NULL이 아니면
	if (scrollAction != NULL)
	{
		//6.1 수직스크롤이 있으면
		Long i = 0;
		if (GetStyle() & WS_VSCROLL)
		{
			//6.1.1 3번 반복한다.
			while (i < 3)
			{
				scrollAction->OnVScroll(nSBCode, 0, NULL);
				i++;
			}
		}
		//6.2 수직 스크롤이 없고 수평스크롤이 있으면
		else if (GetStyle() & WS_HSCROLL)
		{
			//6.2.1 3번 반복한다.
			while (i < 3)
			{
				scrollAction->OnHScroll(nSBCode, 0, NULL);
				i++;
			}
		}
		//6.3 scrollAction을 할당해제한다.
		delete scrollAction;
	}
	//7. 변경사항을 옵저버들에게 알린다.
	this->Notify();
	this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	//8. pageMoveController가 할당해제되어있으면
	if (this->pageMoveController == 0)
	{
		//8.1 pageMoveController를 다시 할당해준다.
		this->pageMoveController = new PageMoveController(this);
	}

	return CFrameWnd::OnMouseWheel(nFlags, zDelta, pt);
}

//왼쪽 마우스 버튼을 클릭했을 때
void NotepadForm::OnLButtonDown(UINT nFlags, CPoint point)
{
	//마우스 왼쪽 버튼을 클릭했다고 표시한다.
	this->isMouseLButtonClicked = true;
	//1. shift키가 눌러져 있는지 확인한다.
	Long shiftPressedCheck = GetKeyState(VK_SHIFT);
	//2. shift키를 안눌렀고, 선택이 진행중이면
	if (!(shiftPressedCheck & 0x80) && this->isSelecting == true)
	{
		//2.1 선택된 텍스트를 선택해제한다.(선택을 끝낸다.)
		this->selectingTexts->Undo();
		//2.2 선택이 끝난 상태로 바꾼다.
		this->isSelecting = false;
		//2.3 선택이 끝났기 때문에 캐럿의 x좌표를 0으로 저장한다.
		this->selectedStartXPos = 0;
		//2.4 선택이 끝났기 때문에 캐럿의 y좌표를 0으로 저장한다.
		this->selectedStartYPos = 0;
		this->menu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->menu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->menu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
	//3. 마우스가 이동하기 전에 줄의 위치와 글자 위치를 구한다.
	Long previousRowIndex = this->note->GetCurrent();
	Long previousLetterIndex = this->current->GetCurrent();
	//3. 수직스크롤의 현재 위치를 구한다.
	Long currentScrollYPos = this->GetScrollPos(SB_VERT);
	//4. 마우스가 클릭한 곳의 y좌표를 구한다.
	Long mouseClickYPos = point.y + currentScrollYPos;
	//5. 노트의 첫 줄부터 길이를 세서 마우스가 클릭한 곳의 y좌표보다 작거나 같은동안
	//그리고 마지막 줄의 위치보다 작은동안 반복한다.
	Long currentRowIndex = 0;
	Long currentRowPos = currentRowIndex * this->textExtent->GetHeight()
		+ this->textExtent->GetHeight();
	Long lastRowIndex = this->note->GetLength() - 1;
	while (currentRowPos < mouseClickYPos && currentRowIndex < lastRowIndex)
	{
		//5.1 줄의 위치를 증가시킨다.
		currentRowIndex++;
		//5.2 줄의 길이를 구한다.
		currentRowPos = currentRowIndex * this->textExtent->GetHeight()
			+ this->textExtent->GetHeight();
	}
	//6. 줄의 위치를 이동시킨다.
	currentRowIndex = this->note->Move(currentRowIndex);
	this->current = this->note->GetAt(currentRowIndex);
	//7. 수평스크롤의 현재 위치를 구한다.
	Long currentScrollXPos = this->GetScrollPos(SB_HORZ);
	//8. 마우스가 클릭한 곳의 x좌표를 구한다.
	Long mouseClickXPos = point.x + currentScrollXPos;
	//9. 줄의 첫 글자부터 너비를 세서 마우스가 클릭한 곳의 x좌표보다 작거나 같은동안
	//그리고 글자위치가 줄의 글자개수보다 작은동안 반복한다.
	Long currentLetterIndex = 0;
	Long currentLetterPos = this->textExtent->GetTextWidth(this->current
		->GetPartOfContent(currentLetterIndex));
	while (currentLetterPos < mouseClickXPos && currentLetterIndex < this->current->GetLength())
	{
		//9.1 글자 위치를 증가시킨다.
		currentLetterIndex++;
		//9.2 글자 너비를 구한다.
		currentLetterPos = this->textExtent->GetTextWidth(this->current
			->GetPartOfContent(currentLetterIndex));
	}
	//10. 이전 글자까지 줄의 텍스트 너비를 구한다.
	Long previousLetterPos = this->textExtent->
		GetTextWidth(this->current->GetPartOfContent(currentLetterIndex - 1));
	//11. 마우스클릭 x좌표에서 이전 글자까지 줄의 텍스트 너비의 차를 구한다.
	Long previousDifference = mouseClickXPos - previousLetterPos;
	//12. 현재 글자까지 줄의 텍스트 너비에서 마우스클릭 x좌표의 차를 구한다.
	Long currentDifference = currentLetterPos - mouseClickXPos;
	//13. 이전 차가 현재 차보다 작거나 같으면
	if (previousDifference <= currentDifference)
	{
		//13.1 이전 글자 위치로 이동시킨다.
		currentLetterIndex = this->current->Move(currentLetterIndex - 1);
	}
	//14. 이전 차가 현재 차보다 크면
	else
	{
		//14.1 글자 위치를 이동시킨다.
		currentLetterIndex = this->current->Move(currentLetterIndex);
	}
	//15. Shift키를 누른 채로 왼쪽 마우스를 클릭했으면
	if (shiftPressedCheck & 0x80)
	{
		//15.1 마우스가 실질적으로 이동했으면
		if (previousRowIndex != currentRowIndex || previousLetterIndex != currentLetterIndex)
		{
			//15.1.1 선택이 진행 중이 아니면
			if (this->isSelecting == false)
			{
				//15.1.1.1 선택이 진행되고 있는 중으로 상태를 바꾼다.
				this->isSelecting = true;
				//15.1.1.2 선택이 시작되는 캐럿의 x좌표를 저장한다.
				this->selectedStartXPos = previousLetterIndex;
				//15.1.1.3 선택이 시작되는 캐럿의 y좌표를 저장한다.
				this->selectedStartYPos = previousRowIndex;
				//15.1.1.4 선택영역이 생겼기 때문에 메뉴버튼에서 복사하기, 잘라내기, 삭제메뉴를 활성화시킨다.
				this->menu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
				this->menu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
				this->menu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
				this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
				this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
				this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
			}
			//15.1.2 선택이 진행 중이면
			else
			{
				//15.1.2.1 선택된 텍스트를 선택해제한다.(선택을 끝낸다.)
				this->selectingTexts->Undo();
			}
			//15.1.3 선택이 시작되는 줄보다 현재 줄의 위치가 더크면
			if (this->selectedStartYPos < currentRowIndex)
			{
				//15.1.3.1 텍스트를 다음으로 선택한다.
				this->selectingTexts->DoNext(this->selectedStartYPos, this->selectedStartXPos,
					currentRowIndex, currentLetterIndex);
			}
			//15.1.4 선택이 시작되는 줄의 위치와 현재 줄의 위치가 같으면 
			else if (this->selectedStartYPos == currentRowIndex)
			{
				//15.1.4.1 선택이 시작되는 글자 위치보다 현재 글자 위치가 더크면
				if (this->selectedStartXPos < currentLetterIndex)
				{
					//15.1.4.1.1 텍스트를 다음으로 선택한다.
					this->selectingTexts->DoNext(this->selectedStartYPos, this->selectedStartXPos,
						currentRowIndex, currentLetterIndex);
				}
				//15.1.4.2 선택이 시작되는 글자 위치가 현재 글자 위치보다 더 크면
				else
				{
					//15.1.4.2.1 텍스트를 이전으로 선택한다.
					this->selectingTexts->DoPrevious(this->selectedStartYPos,
						this->selectedStartXPos, currentRowIndex, currentLetterIndex);
				}
			}
			//15.1.5 선택이 시작되는 줄의 위치가 현재 줄의 위치보다 더 크면
			else if (this->selectedStartYPos > currentRowIndex)
			{
				//15.1.5.1 텍스트를 이전으로 선택한다.
				this->selectingTexts->DoPrevious(this->selectedStartYPos,
					this->selectedStartXPos, currentRowIndex, currentLetterIndex);
			}
		}
	}

	this->SetCapture();

	//16. 변경사항을 옵저버들에게 알린다.
	this->Notify();
	this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

//왼쪽 마우스 버튼을 더블 클릭했을 때
void NotepadForm::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//1. 메모장에서 현재 줄의 위치를 구한다.
	Long currentRowIndex = this->note->GetCurrent();
	//2. 메모장에서 현재 글자의 위치를 구한다.
	Long currentLetterIndex = this->note->GetAt(currentRowIndex)->GetCurrent();
	Long previousRowIndex = 0;
	Long previousLetterIndex = 0;
	//3. 자동개행이 진행 중이 아니면
	if (this->isRowAutoChanging == false)
	{
		//3.1 이동하기 전에 줄의 위치를 구한다.
		previousRowIndex = currentRowIndex;
		//3.2 이동하기 전에 글자의 위치를 구한다.
		previousLetterIndex = currentLetterIndex;
		//3.3 노트에서 단어단위로 왼쪽으로 이동한다.
		currentLetterIndex = this->note->PreviousWord();
		//3.4 이동한 후에 줄의 위치를 구한다.
		currentRowIndex = this->note->GetCurrent();
		//3.5 현재 줄을 이동한 후의 줄로 변경한다.
		this->current = this->note->GetAt(currentRowIndex);
	}
	//4. 자동개행이 진행 중이면
	else
	{
		//4.1 노트에서 자동개행 진행 중인 상태에서 단어단위로 오른쪽으로 이동한다.
		this->note->
			PreviousWordOnRowAutoChange(currentRowIndex, currentLetterIndex);
		//4.2 이동한 후에 줄의 위치를 구한다.
		currentRowIndex = this->note->GetCurrent();
		//4.3 메모장의 현재 줄을 이동한 후의 줄로 변경한다.
		this->current = this->note->GetAt(currentRowIndex);
	}
	//5. 단어단위로 오른쪽으로 이동하기 전에 줄의 위치를 구한다.
	previousRowIndex = this->note->GetCurrent();
	//6. 단어단위로 오른쪽으로 이동하기 전에 글자의 위치를 구한다.
	previousLetterIndex = this->current->GetCurrent();
	//7. 자동개행이 진행 중이 아니면
	currentRowIndex = previousRowIndex;
	currentLetterIndex = previousLetterIndex;
	if (this->isRowAutoChanging == false)
	{
		//7.1 노트에서 단어단위로 오른쪽으로 이동한다.
		currentLetterIndex = this->note->NextWord();
	}
	//8. 자동개행이 진행 중이면
	else
	{
		//8.1 노트에서 자동개행이 진행 중인 상태에서 단어단위로 오른쪽으로 이동한다.
		this->note->NextWordOnRowAutoChange(currentRowIndex, currentLetterIndex);
	}
	//9. 단어 단위로 오른쪽으로 이동한 후에 현재 줄과 글자의 위치를 구한다.
	currentRowIndex = this->note->GetCurrent();
	this->current = this->note->GetAt(currentRowIndex);
	currentLetterIndex = this->current->GetCurrent();
	//10. 단어단위로 이동한 후에 줄의 위치가 서로 다르면
	if (previousRowIndex < currentRowIndex)
	{
		//10.1 선택이 진행되고 있는 중으로 상태를 바꾼다.
		this->isSelecting = true;
		//10.2 선택이 시작되는 캐럿의 x좌표를 저장한다.
		this->selectedStartXPos = previousLetterIndex;
		//10.3 선택이 시작되는 캐럿의 y좌표를 저장한다.
		this->selectedStartYPos = previousRowIndex;
	}
	//11. 단어단위로 이동한 후에 줄의 위치가 서로 같으면(한 줄 내에서 단어단위 이동이 있으면)
	else
	{
		//11.1 단어 단위로 이동이 있었으면
		if (previousLetterIndex < currentLetterIndex)
		{
			//11.1.1 선택이 진행되고 있는 중으로 상태를 바꾼다.
			this->isSelecting = true;
			//11.1.2 선택이 시작되는 캐럿의 x좌표를 저장한다.
			this->selectedStartXPos = previousLetterIndex;
			//11.1.3 선택이 시작되는 캐럿의 y좌표를 저장한다.
			this->selectedStartYPos = previousRowIndex;
		}
	}
	//12. 선택이 진행 중이면
	if (this->isSelecting == true)
	{
		//12.1 텍스트를 선택한다.
		this->selectingTexts->DoNext(previousRowIndex, previousLetterIndex,
			currentRowIndex, currentLetterIndex);
		//12.2 선택영역이 생겼기 때문에 메뉴버튼에서 복사하기, 잘라내기, 삭제메뉴를 활성화시킨다.
		this->menu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
		this->menu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
		this->menu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
		this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
		this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
		this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
		//12.3 변경사항을 옵저버들에게 알린다.
		this->Notify();
		this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}	
}

//오른쪽 마우스 버튼을 클릭했을 때
void NotepadForm::OnRButtonDown(UINT nFlags, CPoint point)
{
	CMenu* MyMenu;
	CPoint pt;
	GetCursorPos(&pt);
	MyMenu = this->mouseRButtonMenu.GetSubMenu(0);
	MyMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
}

//마우스를 움직일 때
void NotepadForm::OnMouseMove(UINT nFlags, CPoint point)
{
	//1. shift키가 눌러져 있는지 확인한다.
	//Long shiftPressedCheck = GetKeyState(VK_SHIFT);
	//마우스 왼쪽 버튼이 눌러져 있거나 또는 shift키를 누른 채로 마우스 왼쪽 버튼을 눌렀으면
	if (this->isMouseLButtonClicked == true
		|| nFlags == MK_SHIFT && this->isMouseLButtonClicked == true)
	{
		//1. 마우스가 이동하기 전에 줄의 위치와 글자 위치를 구한다.
		Long previousRowIndex = this->note->GetCurrent();
		Long previousLetterIndex = this->current->GetCurrent();
		//2. 수직스크롤의 현재 위치를 구한다.
		Long currentScrollYPos = this->GetScrollPos(SB_VERT);
		//3. 마우스가 드래그한 곳의 y좌표를 구한다.
		Long mouseDragYPos = point.y + currentScrollYPos;
		//4. 노트의 첫 줄부터 길이를 세서 마우스가 드래그한 곳의 y좌표보다 작거나 같은동안
		//그리고 마지막 줄의 위치보다 작은동안 반복한다.
		Long currentRowIndex = 0;
		Long currentRowPos = currentRowIndex * this->textExtent->GetHeight()
			+ this->textExtent->GetHeight();
		Long lastRowIndex = this->note->GetLength() - 1;
		while (currentRowPos < mouseDragYPos && currentRowIndex < lastRowIndex)
		{
			//4.1 줄의 위치를 증가시킨다.
			currentRowIndex++;
			//4.2 줄의 길이를 구한다.
			currentRowPos = currentRowIndex * this->textExtent->GetHeight()
				+ this->textExtent->GetHeight();
		}
		//5. 줄의 위치를 이동시킨다.
		currentRowIndex = this->note->Move(currentRowIndex);
		this->current = this->note->GetAt(currentRowIndex);
		//6. 수평스크롤의 현재 위치를 구한다.
		Long currentScrollXPos = this->GetScrollPos(SB_HORZ);
		//7. 마우스가 드래그한 곳의 x좌표를 구한다.
		Long mouseDragXPos = point.x + currentScrollXPos;
		//8. 줄의 첫 글자부터 너비를 세서 마우스가 드래그한 곳의 x좌표보다 작거나 같은동안
		//그리고 글자위치가 줄의 글자개수보다 작은동안 반복한다.
		Long currentLetterIndex = 0;
		Long currentLetterPos = this->textExtent->GetTextWidth(this->current
			->GetPartOfContent(currentLetterIndex));
		while (currentLetterPos < mouseDragXPos
			&& currentLetterIndex < this->current->GetLength())
		{
			//8.1 글자 위치를 증가시킨다.
			currentLetterIndex++;
			//8.2 글자 너비를 구한다.
			currentLetterPos = this->textExtent->GetTextWidth(this->current
				->GetPartOfContent(currentLetterIndex));
		}
		//10. 이전 글자까지 줄의 텍스트 너비를 구한다.
		Long previousLetterPos = this->textExtent->
			GetTextWidth(this->current->GetPartOfContent(currentLetterIndex - 1));
		//11. 마우스클릭 x좌표에서 이전 글자까지 줄의 텍스트 너비의 차를 구한다.
		Long previousDifference = mouseDragXPos - previousLetterPos;
		//12. 현재 글자까지 줄의 텍스트 너비에서 마우스클릭 x좌표의 차를 구한다.
		Long currentDifference = currentLetterPos - mouseDragXPos;
		//13. 이전 차가 현재 차보다 작거나 같으면
		if (previousDifference <= currentDifference)
		{
			//13.1 이전 글자 위치로 이동시킨다.
			currentLetterIndex = this->current->Move(currentLetterIndex - 1);
		}
		//14. 이전 차가 현재 차보다 크면
		else
		{
			//14.1 글자 위치를 이동시킨다.
			currentLetterIndex = this->current->Move(currentLetterIndex);
		}
		//10. 마우스가 실질적으로 이동했으면
		if (previousRowIndex != currentRowIndex || previousLetterIndex != currentLetterIndex)
		{
			//10.1 선택이 진행 중이 아니면
			if (this->isSelecting == false)
			{
				//10.1.1 선택이 진행되고 있는 중으로 상태를 바꾼다.
				this->isSelecting = true;
				//10.1.2 선택이 시작되는 캐럿의 x좌표를 저장한다.
				this->selectedStartXPos = previousLetterIndex;
				//10.1.3 선택이 시작되는 캐럿의 y좌표를 저장한다.
				this->selectedStartYPos = previousRowIndex;
				//10.1.4 선택영역이 생겼기 때문에 메뉴버튼에서 복사하기, 잘라내기, 삭제메뉴를 활성화시킨다.
				this->menu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
				this->menu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
				this->menu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
				this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
				this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
				this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
			}
			//10.2 선택이 진행 중이면
			else
			{
				//1.1. 선택된 텍스트를 선택해제한다.(선택을 끝낸다.)
				this->selectingTexts->Undo();
			}
			//10.2 선택이 시작되는 줄보다 현재 줄의 위치가 더크면
			if (this->selectedStartYPos < currentRowIndex)
			{
				//10.2.1 텍스트를 다음으로 선택한다.
				this->selectingTexts->DoNext(this->selectedStartYPos, this->selectedStartXPos,
					currentRowIndex, currentLetterIndex);
			}
			//10.3 선택이 시작되는 줄의 위치와 현재 줄의 위치가 같으면 
			else if (this->selectedStartYPos == currentRowIndex)
			{
				//10.3.1 선택이 시작되는 글자 위치보다 현재 글자 위치가 더크면
				if (this->selectedStartXPos < currentLetterIndex)
				{
					//10.3.1.1 텍스트를 다음으로 선택한다.
					this->selectingTexts->DoNext(this->selectedStartYPos, this->selectedStartXPos,
						currentRowIndex, currentLetterIndex);
				}
				//10.3.2 선택이 시작되는 글자 위치가 현재 글자 위치보다 더 크면
				else
				{
					//10.3.2.1 텍스트를 이전으로 선택한다.
					this->selectingTexts->DoPrevious(this->selectedStartYPos,
						this->selectedStartXPos, currentRowIndex, currentLetterIndex);
				}
			}
			//10.4 선택이 시작되는 줄의 위치가 현재 줄의 위치보다 더 크면
			else if (this->selectedStartYPos > currentRowIndex)
			{
				//10.4.1 텍스트를 이전으로 선택한다.
				this->selectingTexts->DoPrevious(this->selectedStartYPos,
					this->selectedStartXPos, currentRowIndex, currentLetterIndex);
			}
			//10.5 변경사항을 옵저버들에게 알린다.
			this->Notify();
			this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		}
	}
	
}

//왼쪽 마우스 버튼을 땠을 때
void NotepadForm::OnLButtonUp(UINT nFlags, CPoint point)
{
	::ReleaseCapture();
	//마우스 왼쪽 버튼을 땠다고 표시한다.
	this->isMouseLButtonClicked = false;
}

//메모장에서 화면의 크기가 변경될 때
void NotepadForm::OnSize(UINT nType, int cx, int cy)
{
	//현재 메모장의 창의 상태(최소화, 최대화, 이전 크기로 복원)와 가로 길이와 세로 길이를 입력받는다.
	CFrameWnd::OnSize(nType, cx, cy);
	//1. 현재 화면의 크기를 구한다.
	CRect rect;
	this->GetClientRect(&rect);
	cx = rect.Width();
	//2. 현재 메모장의 상태가 최소화가 아니면(최소화이면 cx와 cy 값이 둘다 0이 되고, 
	//cx가 0이면 아래에서 cx크기로 반복을 돌리는데 무한반복이 발생해서 최소화버튼을 누르면 뻑이남!) 
	if (nType != SIZE_MINIMIZED)
	{
		//2.1 자동줄바꿈이 진행중이면
		if (this->isRowAutoChanging == true)
		{
			//2.1.1 RowAutoChange를 생성한다.(힙에 할당하면 나중에 따로 할당해제를 해줘야함
			//그러나 주소없이 스택에 할당하면 이 함수 스택이 종료되면 자동으로 같이 사라짐.)
			//여기서는 스택에서만 RowAutoChange의 연산을 쓰기 위한것이기 때문에 스택에 할당하는게 효율적임!
			RowAutoChange rowAutoChange(this);
			//2.1.2. 자동개행 전의 원래 줄과 캐럿의 위치를 구한다.
			Long changedRowPos = this->note->GetCurrent();
			Long changedLetterPos = this->current->GetCurrent();
			Long originRowPos = 0;
			Long originLetterPos = 0;
			rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
				&originRowPos);
			//2.1.3 메모장의 현재 화면 크기가 바뀌었으면
			if (this->previousPageWidth != cx)
			{
				//2.1.3.1 자동개행을 취소한다.
				rowAutoChange.UndoAllRows();
				//2.1.3.2 화면크기 변경에 따라 다시 자동개행을 해준다.
				rowAutoChange.DoAllRows();
				//2.1.3.3 메모장의 현재 화면의 가로 길이가 바뀌었기 때문에 이를 갱신해준다.
				this->previousPageWidth = cx;
			}
			//2.1.4 메모장의 현재 화면 크기가 바뀌지 않았으면
			else
			{
				//2.1.4.1 자동개행을 취소한다.
				rowAutoChange.UndoRow();
				//2.1.4.2 화면크기 변경에 따라 다시 자동개행을 해준다.
				rowAutoChange.DoRow();
			}
			//2.1.5 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
			rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
				&changedRowPos);
			//2.1.6 현재 줄의 위치와 글자의 위치를 조정한다.
			Long currentRowIndex = this->note->Move(changedRowPos);
			this->current = this->note->GetAt(currentRowIndex);
			Long currentLetterIndex = this->current->Move(changedLetterPos);
			
		}
		//2.3 캐럿의 위치와 스크롤 정보가 변경되었음을 알린다.
		this->Notify();
		//2.4 변경사항을 갱신한다.
		//Invalidate하면 변경을 예약하는거라서 실시간 반응이 늦는데 
		//RedrawWindow를 이용하면 실시간으로 반응해서 마우스 커서 이동이 훨씬 빠르다
		this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}
}

//메뉴버튼을 클릭했을 때
void NotepadForm::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu)
{
	//1. 외부클립보드에 문자열이 저장되어 있으면
	unsigned int priority_list = CF_TEXT;
	if (GetPriorityClipboardFormat(&priority_list, 1) == CF_TEXT)
	{
		//1.1 붙여넣기 메뉴를 활성화시켜준다.
		this->menu.EnableMenuItem(IDM_NOTE_PASTE, MF_BYCOMMAND | MF_ENABLED);
		this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_PASTE, MF_BYCOMMAND | MF_ENABLED);
	}
	//2. 외부클립보드에 문자열이 저장되어 있지 않으면
	else if (GetPriorityClipboardFormat(&priority_list, 1) == NULL)
	{
		//2.1 붙여넣기 메뉴를 비활성화시킨다.
		this->menu.EnableMenuItem(IDM_NOTE_PASTE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_PASTE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
}

BOOL NotepadForm::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

//메모장에서 닫기버튼을 클릭했을 떄
void NotepadForm::OnClose()
{
	//1. file을 생성한다.
	File file;
	int messageBoxButton = IDCANCEL;
	//2. 메모장에 변경 사항이 있으면
	if (this->isDirty == true)
	{
		//2.1 메세지박스의 메세지를 생성한다.
		string message = "제목 없음";
		//2.2 파일경로가 정해져 있으면
		if (this->filePath != "")
		{
			//2.2.1 메세지를 변경한다.
			message = this->filePath;
		}
		//2.3 SaveBox 메세지 내용을 만든다.
		message.insert(0, "변경 내용을 ");
		message += "에 저장하시겠습니까?";
		//2.4 Save메세지박스를 출력한다.
		messageBoxButton = SaveMessageBox(this->GetSafeHwnd(), message.c_str(), 
			"메모장", MB_YESNOCANCEL);
		//2.5 Save메세지박스에서 Yes를 선택했으면
		if (messageBoxButton == IDYES)
		{
			//2.5.1 노트가 이미 있던 파일이면
			if (this->filePath != "")
			{
				//2.5.1.1 메모장을 저장한다.
				file.Save(this, this->filePath);
				//2.5.1.2 메모장에 변경사항이 없음을 저장한다.
				this->isDirty = false;
				//2.5.1.3 메모장 제목을 바꾼다.
				string name = this->fileName;
				name += " - 메모장";
				SetWindowText(CString(name.c_str()));
			}
			//2.5.2 노트가 처음 생성된 파일이면
			else
			{
				//2.5.2.1 파일공통 대화상자를 생성한다.
				LPCTSTR  filesFilter = _T("텍스트 문서(*.txt) | *.txt; | 모든 파일 | *.*;  ||");
				CFileDialog fileDialog(FALSE, _T("txt"), "*.txt", 0, filesFilter, this, 0, 1);
				//2.5.2.2 파일공통 대화상자를 출력한다.
				if (fileDialog.DoModal() == IDOK)
				{
					//2.5.2.2.1 저장할 파일의 이름을 구한다.
					this->fileName = string(fileDialog.GetFileTitle());
					//2.5.2.2.2 저장할 파일의 경로를 구한다.
					this->filePath = string(fileDialog.GetPathName());
				}
				//2.5.2.3 선택한 메모장을 저장한다.
				file.Save(this, this->filePath);
				//2.5.2.4 메모장에 변경사항이 없음을 저장한다.
				this->isDirty = false;
				//2.5.2.5 메모장 제목을 바꾼다.
				string name = this->fileName;
				name += " - 메모장";
				SetWindowText(CString(name.c_str()));
			}
		}
	}
	//3. 메세지박스에서 CANCEL을 선택하지 않았거나 변경된 사항이 없으면
	if (messageBoxButton != IDCANCEL || this->isDirty == false)
	{
		//프로그램을 종료하기 전에 지금 자동개행이 진행 중인지 아닌지 여부를 레지스트리에 저장함.
		AfxGetApp()->WriteProfileInt("NotepadSection", "IsRowAutoChanging",
			!this->isRowAutoChanging);
		//프로그램을 종료하기 전에 지금 글꼴 정보를 레지스트리에 저장함.
		AfxGetApp()->WriteProfileInt("NotepadSection", "FontSize", this->font.GetSize());
		string style = this->font.GetLogFont().lfFaceName;
		AfxGetApp()->WriteProfileString("NotepadSection", "FontStyle", style.c_str());
		//3.1 메모장을 지운다.
		if (this->note != NULL)
		{
			delete this->note;
			//this->note를 소멸시키면 note에 있는 Row와 letter들도 다 소멸된다.
			//this->current는 Row인데 이미 this->note를 소멸시키면서 Row들이 다 소멸되었는데
			//또 Row를 소멸하라고 하면 소멸할게 없는데 소멸하라고 했기때문에 뻑이난다.!!!!!
			//delete this->current;
		}
		//3.2 클립보드를 지운다.
		if (this->clipboard != NULL)
		{
			delete this->clipboard;
		}
		//3.3 TextExtent를 할당해제한다.
		if (this->textExtent != NULL)
		{
			delete this->textExtent;
		}
		//3.4 SelectingTexts를 할당해제한다.
		if (this->selectingTexts != NULL)
		{
			delete this->selectingTexts;
		}
		//3.5 CFindReplaceDialog를 지운다.
		if (this->findReplaceDialog != 0)
		{
			this->findReplaceDialog->DestroyWindow();
		}
		//3.6 CommandHistory를 할당해제한다.
		if (this->commandHistory != 0)
		{
			delete this->commandHistory;
		}
		//3.7 PreviewForm을 지운다.
		if (this->previewForm != 0)
		{
			this->previewForm->SendMessage(WM_CLOSE);
			delete this->previewForm;
		}
		//3.8 프린트정보를 할당해제한다.
		if (this->printInformation != 0)
		{
			delete this->printInformation;
		}
		if (this->pageSetUpInformation != 0)
		{
			delete this->pageSetUpInformation;
		}
		// 메모장을 닫는다.
		CFrameWnd::OnClose();
	}
}

//Save메세지박스
LRESULT CALLBACK SaveMessageBoxProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	HWND hChildWnd;

	CString msg;// = TEXT("");

	if (nCode == HCBT_ACTIVATE)
	{
		hChildWnd = (HWND)wParam;
		if (GetDlgItem(hChildWnd, IDYES) != NULL)
		{
			msg = "저장(&S)";
			SetWindowText(GetDlgItem(hChildWnd, IDYES), msg);
		}

		if (GetDlgItem(hChildWnd, IDNO) != NULL)
		{
			msg = "저장 안 함(&N)";
			SetWindowText(GetDlgItem(hChildWnd, IDNO), msg);
		}
		UnhookWindowsHookEx(hSaveMessageBoxHook);
	}
	return 0;
}

int SaveMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT nType)
{
	hSaveMessageBoxHook = SetWindowsHookEx(WH_CBT, &SaveMessageBoxProc, 0,
		GetCurrentThreadId());

	return MessageBox(hWnd, lpText, lpCaption, nType);
}