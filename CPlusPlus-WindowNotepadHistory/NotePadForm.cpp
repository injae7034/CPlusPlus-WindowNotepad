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

HHOOK hSaveMessageBoxHook;//전역변수 선언

BEGIN_MESSAGE_MAP(NotepadForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_MESSAGE(WM_IME_COMPOSITION, OnComposition)
	ON_MESSAGE(WM_IME_CHAR, OnImeChar)
	ON_MESSAGE(WM_IME_STARTCOMPOSITION, OnStartCompostion)
	//해당범위(IDM_FILE_OPEN ~ IDM_FONT_CHANGE)의 id들을 클릭하면 OnCommand함수실행
	ON_COMMAND_RANGE(IDM_FILE_OPEN, IDM_ROW_AUTOCHANGE, OnCommand)
	ON_WM_KEYDOWN()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

//NotepadForm생성자
NotepadForm::NotepadForm()
	:CFrameWnd()//여기서 콜론초기화로 따로지정안하면 Font()와 Caret()의 기본생성자가 호출됨
	//왜냐하면 NotepadForm이 멤버로 font와 caret을 가지고 있기때문에 notepadForm이 생성되면서
	//font와 caret의 기본생성자가 호출되어 생성됨. 그렇기 때문에 Font와 Caret의
	//기본생성자 Font()와 Caret()이 필요함.
{
	this->note = NULL;//NULL로 초기화시킴.
	this->current = NULL;//NULL로 초기화시킴.
	this->IsComposing = false;//false로 초기화시킴
	this->IsDirty = false;//false로 초기화시킴
	//this->IsOnScroll = false;//처음생성될때는 스크롤을 이용한 이동이 없기 때문에 false로 초기화함.
	this->fileName = "제목 없음";
	this->filePath = "";
	this->previousPageWidth = 0;
	//CFont에서 사용하고자 하는 글자크기와 글자체로 초기화시킴.
	//기본생성자로 생성된 this->font에 매개변수 5개생성자로 치환(=)시킴
	LOGFONT logFont;
	memset(&logFont, 0, sizeof(LOGFONT));
	logFont.lfHeight = 100;
	wsprintf(logFont.lfFaceName, "궁서체");
	this->font = Font(logFont, RGB(0, 0, 0));
	this->textExtent = NULL;
}

//메모장 윈도우가 생성될 때
int NotepadForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//1. glyphCreator를 만든다.
	GlyphCreator glyphCreator;
	//2. 노트를 만든다.
	this->note = glyphCreator.Create((char*)"\0");
	//3. 줄을 만든다.
	Glyph* row = glyphCreator.Create((char*)"\n");
	//4. 줄을 메모장에 추가한다.
	Long rowIndex;
	rowIndex = this->note->Add(row);
	//5. 현재 줄의 위치를 저장한다.
	this->current = this->note->GetAt(rowIndex);
	//6. 현재 화면의 가로 길이를 저장한다.
	CRect rect;
	this->GetClientRect(&rect);
	this->previousPageWidth = rect.Width();
	//7. CMenu를 notepadForm에 연결한다.
	this->menu.LoadMenu(IDR_MENU1);
	SetMenu(&this->menu);
	//8. textExtent를 힙에 할당한다.
	this->textExtent = new TextExtent(this);
	//9. 선택한 메모장의 노트(내용)를 불러온다.
	File file;
	string path = "test.txt";
	file.Load(this, path);
	//10. 처음 만들어지는 메모장 이름을 정한다.
	string name = this->fileName;
	name += " - 메모장";
	SetWindowText(CString(name.c_str()));
	//11. 캐럿의 현재 세로 위치를 제일 처음으로 보낸다.
	rowIndex = this->note->First();
	//12. 현재 줄의 위치를 다시 저장한다.
	this->current = this->note->GetAt(rowIndex);
	//13. 캐럿의 현재 가로 위치를 제일 처음으로 보낸다.
	Long letterIndex = this->current->First();
	//14. scrollController를 생성한다.
	this->scrollController = new ScrollController(this);
	//15. pageMoveController를 생성한다.
	this->pageMoveController = new PageMoveController(this);

	return 0;
}

//키보드에 글자를 입력할 때
void NotepadForm::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//백스페이스가 아니면(백스페이키는 OnKeyDown을 먼저 실행하고 OnChar로 들어온다)
	//백스페이스인경우 OnChar에서 아무 처리도 안해주고 바로 나가면 된다!
	if (nChar != VK_BACK)
	{
		//1. Ctrl키가 눌러졌는지 안눌러졌는지 상태를 체크해서 저장한다.
		Long ctrlPressedCheck = GetKeyState(VK_CONTROL);
		//2. Ctrl키가 안눌러져 있으면
		if (ctrlPressedCheck >= 0)
		{
			//2.1 glyphCreator를 생성한다.
			GlyphCreator glyphCreator;
			//2.2 glyph를 생성한다.
			Glyph* glyph = glyphCreator.Create((char*)&nChar);
			Long letterIndex;
			Long rowIndex;
			//2.3 입력받은 문자가 개행문자가 아니면
			if (nChar != '\n' && nChar != '\r')
			{
				//2.3.1 현재 줄의 캐럿의 가로 위치를 구한다.
				letterIndex = this->current->GetCurrent();
				//2.3.2 FileSaveCommand가 현재 줄의 length와 같으면
				if (letterIndex == this->current->GetLength())
				{
					//2.3.2.1 현재 줄의 마지막 글자 뒤에 새로운 글자를 추가한다.
					letterIndex = this->current->Add(glyph);
				}
				//2.3.3 index가 현재 줄의 length와 다르면
				else
				{
					//2.3.3.1 현재 줄의 index번째에 새로운 글자를 끼워 쓴다.
					letterIndex = this->current->Add(letterIndex, glyph);
				}

			}
			//2.4 입력받은 문자가 개행문자이면
			else if (nChar == '\n' || nChar == '\r')
			{
				//2.4.1 현재 줄의 위치를 구한다.
				rowIndex = this->note->GetCurrent();
				//2.4.2 현재 줄의 캐럿의 위치를 구한다.
				letterIndex = this->current->GetCurrent();
				//2.4.3. 현재 줄에서 현재 캐럿 다음 위치에 있는 글자들을 떼어낸다.
				glyph = this->current->Split(letterIndex);
				//2.4.4 rowIndex가 노트의 줄의 개수-1 과 같고(현재 줄의 위치가 마지막 줄이면)
				if (rowIndex == this->note->GetLength() - 1)
				{
					//2.4.4.1 새로운 줄을 마지막 줄 다음에 추가한다.
					rowIndex = this->note->Add(glyph);
				}
				//2.4.5 그게 아니면
				else
				{
					//2.4.5.1 새로운 줄을 현재 줄의 다음 위치에 끼워넣는다.
					rowIndex = this->note->Add(rowIndex + 1, glyph);
				}
				//2.4.4 현재 줄의 위치를 새로 저장한다.
				this->current = this->note->GetAt(rowIndex);
				//2.4.5 현재 줄의 캐럿의 위치를 처음으로 이동시킨다.
				this->current->First();
				//2.4.6 자동 줄 바꿈 메뉴가 체크되었는지 확인한다.
				UINT state = this->GetMenu()->
					GetMenuState(IDM_ROW_AUTOCHANGE, MF_BYCOMMAND);
				//2.4.7 자동 줄 바꿈 메뉴가 체크되어 있으면
				if (state == MF_CHECKED)
				{
					//2.4.7.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
					//OnSize에서 부분자동개행을 하도록 한다. 
					this->SendMessage(WM_SIZE);
				}
			}
			//2.5 캐럿의 위치와 크기가 변경되었음을 알린다.
			this->Notify();
			//2.6 isComposing을 false로 바꾼다.
			this->IsComposing = false;
			//2.7 메모장 제목에 *를 추가한다.
			string name = this->fileName;
			name.insert(0, "*");
			name += " - 메모장";
			SetWindowText(CString(name.c_str()));
			//2.8 메모장에 변경사항이 있음을 저장한다.
			this->IsDirty = true;
			//2.9 갱신한다.
			Invalidate(TRUE);
		}
	}
}

//메모장에 텍스트를 출력할 떄//출력시 Visitor패턴적용
void NotepadForm::OnPaint()
{
	//1. CPaintDC를 생성한다.
	CPaintDC dc(this);
	//2. 텍스트의 배경을 투명하게함.
	dc.SetTextColor(this->font.GetColor());
	//4. 왼쪽을 기준선으로 정함.
	dc.SetTextAlign(TA_LEFT);
	//5. CFont를 생성한다.
	CFont font;
	//6. 글씨크기와 글씨체를 정하다.
	font.CreateFontIndirect(&this->font.GetLogFont());
	//7. 폰트를 dc에 지정한다.
	HFONT oldFont;
	oldFont = (HFONT)dc.SelectObject(font);
	//8. TEXTMETRIC을 생성한다.
	TEXTMETRIC text;
	//9. 글꼴의 정보를 얻는다.
	dc.GetTextMetrics(&text);
	//10. note에 저장된 글자들을 출력한다.
	Long rowIndex = 0;
	Long currentXPos;
	Long currentYPos;
	CString content;

	Long currentWidth = 0;
	Long letterWidth = 0;
	Long letterCount = 0;
	Long letterIndex = 0;
	Glyph* row = 0;
	Glyph* letter = 0;

	//11. 줄단위의 반복구조를 통해서 줄을 나눠서 줄개수만큼 출력하도록 함.
	while (rowIndex < this->note->GetLength())
	{
		//11.1 현재 줄을 구한다.
		row = this->note->GetAt(rowIndex);
		//11.2 스크롤의 위치를 구한다.
		currentXPos = this->GetScrollPos(SB_HORZ);
		currentYPos = this->GetScrollPos(SB_VERT);
		//11.3 현재줄의 첫 글자를 구한다.
		letterIndex = 0;
		currentWidth = 0;
		letterWidth = 0;
		//11.4 현재줄의 개수를 구한다.
		letterCount = row->GetLength();
		//11.5 현재글자 위치가 줄의 글자개수보다 작은동안 반복한다.
		while (letterIndex < letterCount)
		{
			//11.5.1 현재 글자를 구한다.
			letter = row->GetAt(letterIndex);
			//11.5.2 현재 글자의 내용을 구한다.
			content = CString(letter->GetContent().c_str());
			//11.5.3 글자의 너비를 구한다.
			//처음엔 0에서 시작해야하기 때문에 currentWidth += letterWidth가 
			//letterWidth = this->textExtent->GetTextWidth((string)content) 보다 앞에 있어야함.
			//그래서 현재 글자 너비를 구해서 다음 글자의 시작점으로 삼아주면 됨
			//첫글자의 시작점은 0이고 다음 글자의 시작점은 첫글자의 너비이다!!!
			currentWidth += letterWidth;
			letterWidth = this->textExtent->GetTextWidth((string)content);
			//만약에 글자가 탭문자이면 내용을 띄어쓰기 8개로 바꿔준다.
			if (content == "\t")
			{
				content = "        ";
			}
			//11.5.3. 현재 글자가 선택이 안되어있으면
			if (letter->IsSelected() == false)
			{
				//11.5.3.1 글자를 화면에 출력한다.
				dc.SetBkColor(RGB(255, 255, 255));
				//텍스트의 색깔을 정함. 이렇게해야 나중에 글꼴상자에서 색깔을 바꾸면 반영할 수 있음.
				dc.SetTextColor(this->font.GetColor());
				//dc.SetTextColor(RGB(0, 0, 0));//이렇게하면 나중에 글꼴에서 글자색을 바꿀수없음
				dc.TextOut(currentWidth -currentXPos, rowIndex * text.tmHeight
					- currentYPos, content);
			}
			//11.5.4 현재 글자가 선택이 되어있으면
			else
			{
				//11.5.4.1
				dc.SetBkColor(GetSysColor(COLOR_HIGHLIGHT));//red, green, blue 세개 색깔 
				dc.SetTextColor(RGB(255, 255, 255));//이렇게하면 나중에 글꼴에서 글자색을 바꿀수 없음
				dc.TextOut(currentWidth -currentXPos, rowIndex * text.tmHeight
					- currentYPos, content);
			}
			letterIndex++;
		}
		//11.3 텍스트 시작위치는 고정되어고 화면만 이동하므로 이동한만큼 빼줘야함!
		//그럼 원래 화면은 처음 시작점에 고정되어 있는데 -해줌으로써 화면이 움직이는 것처럼 보임.
		//dc.TextOut(0 - currentXPos, rowIndex * text.tmHeight - currentYPos, content);
		rowIndex++;
	}
	dc.SelectObject(oldFont);
	//font가 폰트공통대화상자에서 변경되었을때 기존 font를 지워야 새로 변경된 font로 적용할 수 있음. 
	font.DeleteObject();

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
	if (this->IsComposing == true)
	{
		//4.1. 현재 줄의 캐럿의 가로 위치 바로 앞에 있는 기존 한글을 지운다.
		//그러기 위해서는 캐럿의 현재 가로 위치에 1감소한 값을 넣어주면 된다.
		//기존 한글을 지워야 새로 입력 받은 한글을 대체할 수 있다.
		this->current->Remove(index - 1);
		//4.2 갱신된 current의 위치를 index에 저장한다.
		index = this->current->GetCurrent();
	}
	//5. 현재위치의 한글을 지웠기 때문에 한글이 조립중이 아님으로 상태를 변경한다.
	this->IsComposing = false;
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
		//6.3. index가 현재 줄의 length와 다르면
		else
		{
			//6.3.1 현재 줄의 index번째에 새로운 한글을 끼워 쓴다.
			index = this->current->Add(index, doubleByteLetter);
		}
		//6.4 한글을 현재 위치에 추가했기때문에 한글이 조립중인 상태로 변경한다.
		this->IsComposing = true;
	}
	//7. 한글 조립중에 백스페이스키룰 눌러서 조립 중인 한글을 지워버리면
	else
	{
		//BackSpace와 Delete키와 별도로 한글조립중에 지우는 경우도 OnSize로 보내줘야 한다.
		//7.1 자동 줄 바꿈 메뉴가 체크되었는지 확인한다.
		UINT state = this->GetMenu()->
			GetMenuState(IDM_ROW_AUTOCHANGE, MF_BYCOMMAND);
		//7.2 자동 줄 바꿈 메뉴가 체크되어 있으면
		if (state == MF_CHECKED)
		{
			//7.2.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
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
	this->IsDirty = true;
	//11. 갱신한다.
	Invalidate(TRUE);

	return ::DefWindowProc(this->m_hWnd, WM_IME_COMPOSITION, wParam, lParam);
}

//완성된 한글을 입력받았을 때
LRESULT NotepadForm::OnImeChar(WPARAM wParam, LPARAM lParam)
{
	//1. glyphCreator를 생성한다.
	GlyphCreator glyphCreator;
	WORD word = LOWORD(wParam);
	char koreanLetter[3];
	koreanLetter[0] = HIBYTE(word);
	koreanLetter[1] = LOBYTE(word);
	koreanLetter[2] = '\0';
	//2. doubleByteLetter를 생성한다.
	Glyph* doubleByteLetter = glyphCreator.Create((char*)koreanLetter);
	//3. 현재 줄의 캐럿의 가로 위치를 구한다.
	Long index = this->current->GetCurrent();
	//4. 현재 줄의 캐럿의 가로 위치 바로 앞에 있는 기존 한글을 지운다.
	// 그러기 위해서는 캐럿의 현재 가로 위치에 1감소한 값을 넣어주면 된다.
	this->current->Remove(index - 1);
	//5. 기존에 있던 글자가 지워졌기 때문에 캐럿의 현재 가로 위치는 1감소했기때문에
	//캐럿의 현재 가로 위치값을 나타내는 index역시 1 감소시켜준다.
	//index--;
	//갱신된 current의 위치를 index에 저장한다.
	index = this->current->GetCurrent();
	//5. index가 현재 줄의 length와 같으면
	if (index == this->current->GetLength())
	{
		//5.1 현재 줄의 마지막 글자 뒤에 새로운 한글을 추가한다.
		index = this->current->Add(doubleByteLetter);
	}
	//6. index가 현재 줄의 length와 다르면
	else
	{
		//6.1 현재 줄의 index번째에 새로운 한글을 끼워 쓴다.
		index = this->current->Add(index, doubleByteLetter);
	}
	//7. IsComposing을 false로 바꾼다.
	this->IsComposing = false;
	//8. 캐럿이 변경되었음을 알린다.
	this->Notify();
	//9. 메모장 제목에 *를 추가한다.
	string name = this->fileName;
	name.insert(0, "*");
	name += " - 메모장";
	SetWindowText(CString(name.c_str()));
	//10. 메모장에 변경사항이 있음을 저장한다.
	this->IsDirty = true;
	//11. 갱신한다.
	Invalidate(TRUE);

	return 0;
}

//한글 조립 시작을 알림
LRESULT NotepadForm::OnStartCompostion(WPARAM wParam, LPARAM lParam)
{
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
}

//Command패턴
void NotepadForm::OnCommand(UINT nId)
{
	//1. CommandCreator를 생성한다.
	CommandCreator commandCreator(this);
	//2. ConcreteCommand를 생성한다.
	Command* command = commandCreator.Create(nId);
	//3. command가 NULL이 아니면
	if (command != NULL)
	{
		//3.1 ConcreteCommand의 execute 함수를 실행한다.
		command->Execute();
		//3.2 command를 할당해제한다.
		delete command;
	}
}

//메모장에서 키보드로 이동하기
void NotepadForm::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. KeyActionCreator를 생성한다.
	KeyActionCreator keyActionCreator(this);
	//2. ConcreteKeyAction을 생성한다.
	KeyAction* keyAction = keyActionCreator.Create(nChar);
	//3. keyAction이 NULL이 아니면
	if (keyAction != NULL)
	{
		//3.1 ConcreteKeyAction의 OnKeyDown 함수를 실행한다.
		keyAction->OnKeyDown(nChar, nRepCnt, nFlags);
		//3.2 keyAction을 할당해제한다.
		delete keyAction;
		//3.3 변화를 메모장에 갱신한다.
		//if 구조안에서 Notify를 해줘야 Ctrl이나 Shift, Alt Capslock과 같은 특수기능키가 눌렸을 때
		//Notify를 호출해 캐럿이 있는 곳으로 스크롤이 이동하지 않는다. OnKeyDown은 키보드키 중 어떠한
		//키가 눌려져도 호출되기 때문에 원하는 keyAction이 아닌경우 Notify가 실행되지 않게 해야한다!
		this->Notify();
		this->Invalidate();
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
		//4.1 ConcreteScrollAction의 OnVScroll 함수를 실행한다.
		scrollAction->OnVScroll(nSBCode, nPos, pScrollBar);
		//4.2 scrollAction을 할당해제한다.
		delete scrollAction;
	}
	//5. 변경사항을 옵저버들에게 알린다.
	this->Notify();
	this->Invalidate();	
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
		//4.1 ConcreteScrollAction의 OnHScroll 함수를 실행한다.
		scrollAction->OnHScroll(nSBCode, nPos, pScrollBar);
		//4.2 scrollAction을 할당해제한다.
		delete scrollAction;
	}
	//5. 변경사항을 옵저버들에게 알린다.
	this->Notify();
	this->Invalidate();
	//6. pageMoveController가 할당해제되어있으면
	if (this->pageMoveController == 0)
	{
		//6.1 pageMoveController를 다시 할당해준다.
		this->pageMoveController = new PageMoveController(this);
	}
}

//메모장에서 화면의 크기가 변경될 때
void NotepadForm::OnSize(UINT nType, int cx, int cy)
{
	//1. 현재 메모장의 창의 상태(최소화, 최대화, 이전 크기로 복원)와 가로 길이와 세로 길이를 입력받는다.
	CFrameWnd::OnSize(nType, cx, cy);
	//2. 현재 메모장의 상태가 최소화가 아니면(최소화이면 cx와 cy 값이 둘다 0이 되고, 
	//cx가 0이면 아래에서 cx크기로 반복을 돌리는데 무한반복이 발생해서 최소화버튼을 누르면 뻑이남!) 
	if (nType != SIZE_MINIMIZED)
	{
		//2.1 자동 줄 바꿈 메뉴가 체크되었는지 확인한다.
		UINT state = this->GetMenu()->
			GetMenuState(IDM_ROW_AUTOCHANGE, MF_BYCOMMAND);
		//2.2 자동 줄 바꿈 메뉴가 체크되어 있으면
		if (state == MF_CHECKED)
		{
			//2.2.1 RowAutoChange를 생성한다.(힙에 할당하면 나중에 따로 할당해제를 해줘야함
			//그러나 주소없이 스택에 할당하면 이 함수 스택이 종료되면 자동으로 같이 사라짐.)
			//여기서는 스택에서만 RowAutoChange의 연산을 쓰기 위한것이기 때문에 스택에 할당하는게 효율적임!
			RowAutoChange rowAutoChange(this);
			//2.2.2 메모장의 현재 화면 크기가 바뀌었으면
			if (this->previousPageWidth != cx)
			{
				//2.2.2.3 자동개행 전의 원래 줄과 캐럿의 위치를 구한다.
				Long changedRowPos = this->note->GetCurrent();
				Long changedLetterPos = this->current->GetCurrent();
				Long originRowPos = 0;
				Long originLetterPos = 0;
				rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
					&originRowPos);
				//2.2.2.4 자동개행을 취소한다.
				rowAutoChange.UndoAllRows();
				//2.2.2.5 화면크기 변경에 따라 다시 자동개행을 해준다.
				rowAutoChange.DoAllRows();
				//2.2.2.6 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
				rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
					&changedRowPos);
				//2.2.2.7 현재 줄의 위치와 글자의 위치를 조정한다.
				this->note->Move(changedRowPos);
				this->current = this->note->
					GetAt(this->note->GetCurrent());
				this->current->Move(changedLetterPos);
				//2.2.2.8 캐럿의 위치가 변경되었음을 알린다.
				this->Notify();
				//2.2.2.9 변경사항을 갱신한다.
				this->Invalidate(TRUE);
				//2.2.2.10 메모장의 현재 화면의 가로 길이가 바뀌었기 때문에 이를 갱신해준다.
				this->previousPageWidth = cx;
			}
			//2.2.3 메모장의 현재 화면 크기가 바뀌지 않았으면
			else
			{
				//2.2.3.1 자동개행 전의 원래 줄과 캐럿의 위치를 구한다.
				Long changedRowPos = this->note->GetCurrent();
				Long changedLetterPos = this->current->GetCurrent();
				Long originRowPos = 0;
				Long originLetterPos = 0;
				rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
					&originRowPos);
				//2.2.3.2 자동개행을 취소한다.
				rowAutoChange.UndoRow();
				//2.2.3.3 화면크기 변경에 따라 다시 자동개행을 해준다.
				rowAutoChange.DoRow();
				//2.2.3.4 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
				rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
					&changedRowPos);
				//2.2.3.5 현재 줄의 위치와 글자의 위치를 조정한다.
				this->note->Move(changedRowPos);
				this->current = this->note->
					GetAt(this->note->GetCurrent());
				this->current->Move(changedLetterPos);
				//2.2.3.6 캐럿의 위치가 변경되었음을 알린다.
				this->Notify();
				//2.2.3.7 변경사항을 갱신한다.
				this->Invalidate(TRUE);
			}
		}
	}
	
}

//메모장에서 닫기버튼을 클릭했을 떄
void NotepadForm::OnClose()
{
	//1. file을 생성한다.
	File file;
	int messageBoxButton = IDCANCEL;
	//2. 메모장에 변경 사항이 있으면
	if (this->IsDirty == true)
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
				this->IsDirty = false;
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
				this->IsDirty = false;
				//2.5.2.5 메모장 제목을 바꾼다.
				string name = this->fileName;
				name += " - 메모장";
				SetWindowText(CString(name.c_str()));
			}
		}
	}
	//3. 메세지박스에서 CANCEL을 선택하지 않았거나 변경된 사항이 없으면
	if (messageBoxButton != IDCANCEL || this->IsDirty == false)
	{
		//3.1 메모장을 지운다.
		if (this->note != NULL)
		{
			delete this->note;
			//this->note를 소멸시키면 note에 있는 Row와 letter들도 다 소멸된다.
			//this->current는 Row인데 이미 this->note를 소멸시키면서 Row들이 다 소멸되었는데
			//또 Row를 소멸하라고 하면 소멸할게 없는데 소멸하라고 했기때문에 뻑이난다.!!!!!
			//delete this->current;
		}
		//3.2 TextExtent를 할당해제한다.
		if (this->textExtent != NULL)
		{
			delete this->textExtent;
		}
		//NotepadForm는 Subject의 상속을 받았기 때문에 NotepadForm이 소멸될 때
		//Subject의 소멸자가 호출되면 ScrollController를 알아서 할당해제시켜준다.
		//ScrollController는 NotepadForm이 생성될 때 힙에 한번 할당되고 NotepadForm이 소멸될 때
		//같이 소멸되기 때문에 따로 ScrollController를 할당해제시켜줄 필요가 없다.
		//CaretController같은 경우는 OnSetFoucs될때마다 생성되고 OnKillFoculs될때마다 할당해제되는데
		//이 때 NotepadForm은 소멸되지않기 때문에 반드시 OnKillFoucs에서 별도로 할당해제를 해줘야한다.
		/*
		//observer주소배열에서 ScrollController를 찾을 때까지 반복한다.
		Long i = this->length - 1;
		//1. 옵저버 리스트에서 옵저버를 구한다.
		Observer* observer = this->observers.GetAt(i);
		//2. i가 0보다 크거나 같은 동안 옵저버가 캐럿매니저가 아닌동안 반복한다.
		while (i >= 0 && dynamic_cast<ScrollController*>(observer) != 0)
		{
			//2.1 옵저버 리스트에서 옵저버를 구한다.
			observer = this->observers.GetAt(i);
			//2.2 i를 감소시킨다.
			i--;
		}
		//3. 옵저버가 CaretManager이면
		if (dynamic_cast<ScrollController*>(observer))
		{
			//3.1 힙에 할당된 옵저버의 내용을 할당해제한다.
			delete observer;//힙에서 내용 할당해제
			i++;//반복문에서 i를 한번 더 -1해줬기 때문에 원상태로 돌리기 위해 +1을 해줌.
			//3.2 옵저버리스트들 중 이전에 힙에서 할당해제된 내용의 주소를 가지고 있는
			//멤버를 할당해제한다.
			this->observers.Delete(i);//힙에서 내용을 가지고 있던 주소 할당해제
			//3.3 배열요소(주소를 저장)를 한개 할당해제했으니 할당량을 감소시킨다.
			this->capacity--;
			//3.4 사용량을 감소시킨다.
			this->length--;
		}
		*/
		//3.3 메모장을 닫는다.
		CFrameWnd::OnClose();
	}
}

//Save메세지박스
LRESULT CALLBACK SaveMessageBoxProc(int nCode, WPARAM wParam, LPARAM lParam) {
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