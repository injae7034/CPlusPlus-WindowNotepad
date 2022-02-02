#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandCreator.h"
#include "Command.h"
#include "Glyph.h"
#include "CaretManager.h"
#include "File.h"
#include "afxdlgs.h"//CFileDialog헤더파일

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
	wsprintf(logFont.lfFaceName, "나눔바른펜");
	this->font = Font(logFont, RGB(0, 0, 0));
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
	//Menu가 notepadForm의 멤버여야 OnCreate이 종료되어도 정보가 계속 저장되어 있기때문에 뻑이 안남
	//만약에 임시변수로 OnCreate에서만 Menu가 있으면 OnCreate이 종료될때 Menu의 모든정보도 같이 소멸됨
	//6. CMenu를 notepadForm에 연결한다.
	this->menu.LoadMenu(IDR_MENU1);
	SetMenu(&this->menu);
	//7. 처음 만들어지는 메모장 이름을 정한다.
	string name = this->fileName;
	name += " - 메모장";
	SetWindowText(CString(name.c_str()));

	return 0;
}

//키보드에 글자를 입력할 때
void NotepadForm::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. glyphCreator를 생성한다.
	GlyphCreator glyphCreator;
	//2. glyph를 생성한다.
	Glyph* glyph = glyphCreator.Create((char*)&nChar);
	Long index;
	//3. 입력받은 문자가 개행문자가 아니면
	if (nChar != '\n' && nChar != '\r')
	{
		//3.1 현재 줄에 글자를 추가한다.
		index = this->current->Add(glyph);
	}
	//4. 입력받은 문자가 개행문자이면
	else if (nChar == '\n' || nChar == '\r')
	{
		//4.1 새로운 줄을 추가한다.
		index = this->note->Add(glyph);
		//4.2 현재 줄의 위치를 새로 저장한다.
		this->current = this->note->GetAt(index);
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
	dc.SetBkMode(TRANSPARENT);
	//3. 텍스트의 색깔을 정함.
	dc.SetTextColor(this->font.GetColor());
	//4. 왼쪽을 기준선으로 정함.
	dc.SetTextAlign(TA_LEFT);
	//5. CFont를 생성한다.
	CFont font;
	//6. 글씨크기와 글씨체를 정하다.
	//font.CreatePointFont(this->font.GetSize(), this->font.GetFaceName().c_str());
	font.CreateFontIndirect(&this->font.GetLogFont());
	//7. 폰트를 dc에 지정한다.
	dc.SelectObject(font);
	//8. TEXTMETRIC을 생성한다.
	TEXTMETRIC text;
	//9. 글꼴의 정보를 얻는다.
	dc.GetTextMetrics(&text);
	//10. note에 저장된 글자들을 출력한다.
	Long i = 0;
	CString content;
	//11. 줄단위의 반복구조를 통해서 줄을 나눠서 줄개수만큼 출력하도록 함.
	while (i < this->note->GetLength())
	{
		content = CString(this->note->GetAt(i)->GetContent().c_str());
		//11.1 텍스트 시작 위치설정 처음줄은 (0,0)에서 시작하고 두번째줄은 (0, 150)에서 시작함.
		dc.TextOut(0, i * text.tmHeight, content);
		i++;
	}
	//font가 폰트공통대화상자에서 변경되었을때 기존 font를 지워야 새로 변경된 font로 적용할 수 있음. 
	font.DeleteObject();
}

//한글을 입력받을 때
LRESULT NotepadForm::OnComposition(WPARAM wParam, LPARAM lParam)
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
	//3. IsComposing값이 '참'이면
	if (this->IsComposing == true)
	{
		//3.1 현재 줄의 기존 한글을 지운다.
		this->current->Remove(this->current->GetLength() - 1);
	}
	//4. isComposing값이 '거짓'이면
	else
	{
		//4.1 isComposing값을 '참'으로 바꾼다.
		this->IsComposing = true;
	}
	//5. 새로 만든 DoubleByteLetter를 현재 줄에 추가한다.
	Long letterIndex = this->current->Add(doubleByteLetter);
	//6. 캐럿의 위치와 크기가 변경되었음을 알린다.
	this->Notify();
	//7. 메모장 제목에 *를 추가한다.
	string name = this->fileName;
	name.insert(0, "*");
	name += " - 메모장";
	SetWindowText(CString(name.c_str()));
	//8. 메모장에 변경사항이 있음을 저장한다.
	this->IsDirty = true;
	//9. 갱신한다.
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
	//3. 기존에 조립중이던 한글을 지운다.
	this->current->Remove(this->current->GetLength() - 1);
	//4. 현재줄에 완성된 한글을 추가한다.
	this->current->Add(doubleByteLetter);
	//5. IsComposing을 false로 바꾼다.
	this->IsComposing = false;
	//6. 캐럿이 변경되었음을 알린다.
	this->Notify();
	//7. 메모장 제목에 *를 추가한다.
	string name = this->fileName;
	name.insert(0, "*");
	name += " - 메모장";
	SetWindowText(CString(name.c_str()));
	//8. 메모장에 변경사항이 있음을 저장한다.
	this->IsDirty = true;
	//9. 갱신한다.
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
	CaretManager* caretManager = new CaretManager(this);
	//2. 캐럿이 변경되었음을 옵저버들에게 알린다.
	this->Notify();
}

//메모장이 Focus를 잃을 때
void NotepadForm::OnKillFocus(CWnd* pNewWnd)
{
	//CaretEditor의 소멸자에서 캐럿을 숨기고 파괴한다.
	//1. 캐럿을 숨긴다.
	//HideCaret();
	//2. 캐럿을 파괴한다.
	//::DestroyCaret();
	//3. CaretMaker를 할당해제한다.
	//메모리맵 그려보기
	//observer주소배열에서 CaretManager를 찾을 때까지 반복한다.
	Observer* observer = 0;
	Long i = this->length - 1;
	while (i >= 0 && dynamic_cast<CaretManager*>(observer) != 0)
	{
		observer = this->observers.GetAt(i);
		i--;
	}
	if (dynamic_cast<CaretManager*>(observer))
	{
		delete observer;//힙에서 내용 할당해제
		this->observers.Delete(i);//힙에서 내용을 가지고 있던 주소 할당해제
		//배열요소를 한개 제거했으니 숫자감소 반영
		this->capacity--;
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
	//3. ConcreteCommand의 execute를 실행한다.
	command->Execute();
	//4. command를 할당해제한다.
	delete command;
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
		//3.2 메모장을 닫는다.
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