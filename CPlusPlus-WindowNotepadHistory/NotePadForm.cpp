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
	ON_COMMAND_RANGE(IDM_FILE_OPEN, IDM_FONT_CHANGE, OnCommand)
	ON_WM_KEYDOWN()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
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
	this->fileName = "제목 없음";
	this->filePath = "";
	//CFont에서 사용하고자 하는 글자크기와 글자체로 초기화시킴.
	//기본생성자로 생성된 this->font에 매개변수 5개생성자로 치환(=)시킴
	LOGFONT logFont;
	memset(&logFont, 0, sizeof(LOGFONT));
	logFont.lfHeight = 100;
	wsprintf(logFont.lfFaceName, "맑은 고딕");
	this->font = Font(logFont, RGB(0, 0, 0));
	this->textExtent = NULL;
}

//메모장 윈도우가 생성될 때
int NotepadForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//this->ModifyStyle(0, WS_OVERLAPPEDWINDOW, SWP_DRAWFRAME);
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
	//6. CMenu를 notepadForm에 연결한다.
	this->menu.LoadMenu(IDR_MENU1);
	SetMenu(&this->menu);
	//7. textExtent를 힙에 할당한다.
	this->textExtent = new TextExtent(this);
	//8. 선택한 메모장의 노트(내용)를 불러온다.
	File file;
	string path = "test.txt";
	file.Load(this, path);
	//9. 처음 만들어지는 메모장 이름을 정한다.
	string name = this->fileName;
	name += " - 메모장";
	SetWindowText(CString(name.c_str()));
	//10. 캐럿의 현재 세로 위치를 제일 처음으로 보낸다.
	rowIndex = this->note->First();
	//11. 현재 줄의 위치를 다시 저장한다.
	this->current = this->note->GetAt(rowIndex);
	//12. 캐럿의 현재 가로 위치를 제일 처음으로 보낸다.
	this->current->First();
	//13. scrollController를 생성한다.
	this->scrollController = new ScrollController(this);

	return 0;
}

//키보드에 글자를 입력할 때
void NotepadForm::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. glyphCreator를 생성한다.
	GlyphCreator glyphCreator;
	//2. glyph를 생성한다.
	Glyph* glyph = glyphCreator.Create((char*)&nChar);
	Long caretIndex;
	Long rowIndex;
	//3. 입력받은 문자가 개행문자가 아니면
	if (nChar != '\n' && nChar != '\r')
	{
		//3.1 현재 줄의 캐럿의 가로 위치를 구한다.
		caretIndex = this->current->GetCurrent();
		//3.2 FileSaveCommand가 현재 줄의 length와 같으면
		if (caretIndex == this->current->GetLength())
		{
			//3.2.1 현재 줄의 마지막 글자 뒤에 새로운 글자를 추가한다.
			caretIndex = this->current->Add(glyph);
		}
		//3.3 index가 현재 줄의 length와 다르면
		else
		{
			//3.3.1 현재 줄의 index번째에 새로운 글자를 끼워 쓴다.
			caretIndex = this->current->Add(caretIndex, glyph);
		}
		
	}
	//4. 입력받은 문자가 개행문자이면
	else if (nChar == '\n' || nChar == '\r')
	{
		//4.1 현재 줄의 위치를 구한다.
		rowIndex = this->note->GetCurrent();
		//4.2 현재 줄의 캐럿의 위치를 구한다.
		caretIndex = this->current->GetCurrent();
		//4.3. 현재 줄에서 현재 캐럿 다음 위치에 있는 글자들을 떼어낸다.
		glyph = this->current->Split(caretIndex);
		//4.4 rowIndex가 노트의 줄의 개수-1 과 같고(현재 줄의 위치가 마지막 줄이면)
		if (rowIndex == this->note->GetLength() - 1)
		{
			//4.4.1 새로운 줄을 마지막 줄 다음에 추가한다.
			rowIndex = this->note->Add(glyph);
		}
		//4.5 그게 아니면
		else
		{
			//4.5.1 새로운 줄을 현재 줄의 다음 위치에 끼워넣는다.
			rowIndex = this->note->Add(rowIndex + 1, glyph);
		}
		//4.4 현재 줄의 위치를 새로 저장한다.
		this->current = this->note->GetAt(rowIndex);
		//4.5 현재 줄의 캐럿의 위치를 처음으로 이동시킨다.
		this->current->First();
	}
	//5. 캐럿의 위치와 크기가 변경되었음을 알린다.
	this->Notify();
	//6. isComposing을 false로 바꾼다.
	this->IsComposing = false;
	//7. 메모장 제목에 *를 추가한다.
	string name = this->fileName;
	name.insert(0, "*");
	name += " - 메모장";
	SetWindowText(CString(name.c_str()));
	//8. 메모장에 변경사항이 있음을 저장한다.
	this->IsDirty = true;
	//9. 갱신한다.
	Invalidate(TRUE);
}

//메모장에 텍스트를 출력할 떄
void NotepadForm::OnPaint()
{
	//1. CPaintDC를 생성한다.
	CPaintDC dc(this);
	//2. 텍스트의 배경을 투명하게함.
	//3. 텍스트의 색깔을 정함.
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
	Long i = 0;
	Long currentXPos;
	Long currentYPos;
	CString content;
	//11. 줄단위의 반복구조를 통해서 줄을 나눠서 줄개수만큼 출력하도록 함.
	while (i < this->note->GetLength())
	{
		content = CString(this->note->GetAt(i)->GetContent().c_str());
		currentXPos = this->GetScrollPos(SB_HORZ);
		currentYPos = this->GetScrollPos(SB_VERT);
		//11.1 텍스트 시작 위치설정 처음줄은 (0,0)에서 시작하고 두번째줄은 (0, 글자평균높이)에서 시작함.
		//11.2 텍스트 시작위치는 고정되어고 화면만 이동하므로 이동한만큼 빼줘야함!
		dc.TextOut(0 - currentXPos, i * text.tmHeight - currentYPos, content);
		this->Notify();
		i++;
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
	//2. 키보드로부터 입력받을 정보를 바탕으로 한글을 저장한다.
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
		this->current->Remove(index - 1);
		//4.2 갱신된 current의 위치를 index에 저장한다.
		index = this->current->GetCurrent();

	}
	//5. 현재위치의 한글을 지웠기 때문에 한글이 조립중이 아님으로 상태를 변경한다.
	this->IsComposing = false;
	//6. 한글이 입력되었으면(한글 조립중에 글자를 다 지워버리면 '\0'문자로 OnComposition에 입력된다.)
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
	//7. 캐럿의 위치와 크기가 변경되었음을 알린다.
	this->Notify();
	//8. 메모장 제목에 *를 추가한다.
	string name = this->fileName;
	name.insert(0, "*");
	name += " - 메모장";
	SetWindowText(CString(name.c_str()));
	//9. 메모장에 변경사항이 있음을 저장한다.
	this->IsDirty = true;
	//10. 갱신한다.
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
	CaretController* caretController = new CaretController(this);
	//2. 캐럿이 변경되었음을 옵저버들에게 알린다.
	this->Notify();
}

//메모장이 Focus를 잃을 때
void NotepadForm::OnKillFocus(CWnd* pNewWnd)
{
	//observer주소배열에서 CaretManager를 찾을 때까지 반복한다.
	Long i = this->length - 1;
	//1. 옵저버 리스트에서 옵저버를 구한다.
	Observer* observer = this->observers.GetAt(i);
	//2. i가 0보다 크거나 같은 동안 옵저버가 캐럿매니저가 아닌동안 반복한다.
	while (i >= 0 && dynamic_cast<CaretController*>(observer) != observer)
	{
		//2.1 옵저버 리스트에서 옵저버를 구한다.
		observer = this->observers.GetAt(i);
		//2.2 i를 감소시킨다.
		i--;
	}
	//3. 옵저버가 CaretManager이면
	if (dynamic_cast<CaretController*>(observer))
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
	}
	//4. 변경사항을 옵저버리스트에게 알린다.
	this->Notify();
}

//메모장에서 세로 스크롤을 클릭할 때
void NotepadForm::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. scrollActionCreator를 생성한다.
	ScrollActionCreator scrollActionCreator(this);
	//2. concreteScrollAction을 생성한다.
	ScrollAction* scrollAction = scrollActionCreator.Create(nSBCode);
	//3. scrollAction이 NULL이 아니면
	if (scrollAction != NULL)
	{
		//3.1 ConcreteScrollAction의 OnVScroll 함수를 실행한다.
		scrollAction->OnVScroll(nSBCode, nPos, pScrollBar);
		//3.2 scrollAction을 할당해제한다.
		delete scrollAction;
	}
	//4. 변경사항을 옵저버들에게 알린다.
	this->Notify();
	this->Invalidate();	
}

//메모장에서 가로 스크롤을 클릭할 때
void NotepadForm::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. scrollActionCreator를 생성한다.
	ScrollActionCreator scrollActionCreator(this);
	//2. concreteScrollAction을 생성한다.
	ScrollAction* scrollAction = scrollActionCreator.Create(nSBCode);
	//3. scrollAction이 NULL이 아니면
	if (scrollAction != NULL)
	{
		//3.1 ConcreteScrollAction의 OnHScroll 함수를 실행한다.
		scrollAction->OnHScroll(nSBCode, nPos, pScrollBar);
		//3.2 scrollAction을 할당해제한다.
		delete scrollAction;
	}
	//4. 변경사항을 옵저버들에게 알린다.
	this->Notify();
	this->Invalidate();
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
		messageBoxButton = SaveMessageBox(this->GetSafeHwnd(), message.c_str(), "메모장", MB_YESNOCANCEL);
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
		//ScrollController는 NotepadForm이 생성될 때 힙에 한전 할당되고 NotepadForm이 소멸될 때
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

	if (nCode == HCBT_ACTIVATE) {
		hChildWnd = (HWND)wParam;
		if (GetDlgItem(hChildWnd, IDYES) != NULL) {
			msg = "저장(&S)";
			SetWindowText(GetDlgItem(hChildWnd, IDYES), msg);
		}

		if (GetDlgItem(hChildWnd, IDNO) != NULL) {
			msg = "저장 안 함(&N)";
			SetWindowText(GetDlgItem(hChildWnd, IDNO), msg);
		}
		UnhookWindowsHookEx(hSaveMessageBoxHook);
}

	return 0;
}

int SaveMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT nType) {
	hSaveMessageBoxHook = SetWindowsHookEx(WH_CBT, &SaveMessageBoxProc, 0, GetCurrentThreadId());

	return MessageBox(hWnd, lpText, lpCaption, nType);
}