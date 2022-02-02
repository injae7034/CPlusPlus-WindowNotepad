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
	ON_WM_KEYDOWN()
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
	//3.2.9 선택한 메모장의 노트(내용)를 불러온다.
	File file;
	string path = "test.txt";
	file.Load(this, path);
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

//메모장에서 키보드로 이동하기
void NotepadForm::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. Ctrl키가 입력되면 Ctrl키의 상태를 저장한다.
	Long ctrlPressedCheck = GetKeyState(VK_CONTROL);
	//2. CClientDC를 생성한다.
	CClientDC dc(this);
	//3. CFont를 생성한다.
	CFont font;
	//4. 글씨크기와 글씨체를 정하다.
	font.CreateFontIndirect(&this->font.GetLogFont());
	//5. 폰트를 dc에 지정한다.
	dc.SelectObject(font);
	//6. TEXTMETRIC을 생성한다.
	TEXTMETRIC textmetric;
	//7. 글꼴의 정보를 얻는다.
	dc.GetTextMetrics(&textmetric);
	//8. 오른쪽 방향키버튼을 눌렀으면
	if (nChar == VK_RIGHT)
	{
		//8.1 Ctrl키를 같이 눌렀으면
		if (ctrlPressedCheck & 0x8000)
		{
			//8.1.1 Note의 NextWord를 호출한다.
			Long currentCaretindex = this->note->NextWord();
			//8.1.2 현재줄을 변경한다.
			this->current = this->note->GetAt(this->note->GetCurrent());
			//8.1.3 현재줄의 캐럿의 가로 위치를 변경한다.
			Long i = this->current->First();
			while (i < currentCaretindex)
			{
				i = this->current->Next();
			}
		}
		//8.2 Ctrl키를 안눌렀으면
		else
		{
			//8.2.1 이전으로 이동하기 전에 캐럿의 현재 가로위치를 저장한다.
			Long previousIndex = this->current->GetCurrent();
			//8.2.2 다음으로 이동하고 현재 캐럿의 가로위치를 반환받는다.
			Long currentIndex = this->current->Next();
			//8.2.3 캐럿의 이전 가로위치와 캐럿의 현재 가로위치가 같으면
			if (previousIndex == currentIndex)
			{
				//8.2.3.1 다음으로 이동하기 전에 캐럿의 현재 세로위치를 저장한다.
				Long previousRowIndex = this->note->GetCurrent();
				//8.2.3.2 캐럿의 현재 세로 위치를 다음 줄로 이동시킨다.
				Long currentRowIndex = this->note->Next();
				//8.2.3.3 캐럿의 이전 세로 위치와 캐럿의 현재 세로 위치가 서로 다르면
				if (previousRowIndex != currentRowIndex)
				{
					//8.2.3.3.1 캐럿의 현재 줄을 변경한다.
					this->current = this->note->GetAt(currentRowIndex);
					//8.2.3.3.2 캐럿의 현재 가로 위치를 변경한다.
					this->current->First();
				}
			}
		}

	}
	else if (nChar == VK_LEFT)
	{
		//Ctrl이 눌려져있으면
		if (ctrlPressedCheck & 0x8000)
		{
			//Note의 PreviousWord를 호출한다.
			Long currentCaretindex = this->note->PreviousWord();
			//현재줄을 변경한다.
			this->current = this->note->GetAt(this->note->GetCurrent());
			//현재줄의 캐럿의 가로 위치를 변경한다.
			Long i = this->current->First();
			while (i < currentCaretindex)
			{
				i = this->current->Next();
			}
		}
		else
		{
			//이전으로 이동하기 전에 캐럿의 현재 가로위치를 저장한다.
			Long previousIndex = this->current->GetCurrent();
			//이전으로 이동하고 현재 캐럿의 가로위치를 반환받는다.
			Long currentIndex = this->current->Previous();
			//캐럿의 이전 가로위치와 캐럿의 현재 가로위치가 같으면
			if (previousIndex == currentIndex)
			{
				//이전으로 이동하기 전에 캐럿의 현재 세로 위치를 저장한다.
				Long previousRowIndex = this->note->GetCurrent();
				//캐럿의 현재 세로 위치를 이전 줄로 이동시킨다.
				Long currentRowIndex = this->note->Previous();
				//캐럿의 이전 세로 위치와 캐럿의 현재 새로 위치가 서로 다르면
				if (previousRowIndex != currentRowIndex)
				{
					//캐럿의 현재 줄을 변경한다.
					this->current = this->note->GetAt(currentRowIndex);
					//캐럿의 현재 가로 위치를 변경한다.
					this->current->Last();
				}
			}
		}
	}
	else if (nChar == VK_DOWN)
	{
		//1. 다음으로 이동하기 전에 캐럿의 현재 세로 위치를 저장한다.
		Long previousRowIndex = this->note->GetCurrent();
		//2. 다음으로 이동하기 전에 캐럿의 현재 가로 위치를 저장한다.
		Long previousCaretIndex = current->GetCurrent();
		//3. 캐럿의 현재 세로 위치를 다음 줄로 이동시킨다.
		Long currentRowIndex = this->note->Next();
		//4. 이동하기 전 캐럿의 세로 위치와 이동한 후 캐럿의 세로 위치가 서로 다르면(실질적으로 이동을 했으면)
		if (previousRowIndex != currentRowIndex)
		{
			//4.1 현재 줄을 이동한 후 캐럿의 세로 위치가 있는 줄로 변경한다.
			this->current = this->note->GetAt(currentRowIndex);
			//4.2 이동하기 전 캐럿의 가로 위치가 0이 아니고 이동한 후 현재줄의 글자개수가 0이 아니면
			if (previousCaretIndex != 0 && this->current->GetLength() != 0)
			{
				//4.2.1 이동하기 전 줄의 텍스트를 구한다.
				CString previousRowText = CString(this->note->GetAt(previousRowIndex)
					->GetPartOfContent(previousCaretIndex).c_str());
				//4.2.2 이동하기 전 줄의 텍스트 크기를 구한다.
				CSize previousRowTextSize = dc.GetTextExtent(previousRowText);
				//4.2.3 캐럿의 현재 가로 위치를 처음으로 이동시킨다.
				this->current->First();//캐럿의 가로 위치를 맨 처음(0)으로 보냄
				Long i = this->current->Next();//첫번째 글자를 읽기 위해 캐럿을 1 증가시킴.
				//4.2.4 현재 줄의 텍스트를 구한다. (첫번째 글자를 읽음)
				CString currentRowText = CString(this->current->GetPartOfContent(i).c_str());
				//4.2.5 현재 줄의 텍스트의 크기를 구한다.(첫번째 글자의 텍스트 크기를 구함)
				CSize currentRowTextSize = dc.GetTextExtent(currentRowText);
				//4.2.6 i(캐럿의 현재 가로위치)가 length(현재줄의 글자개수)보다 작고
				//현재 줄의 텍스트 크기가 이동하기 전 줄의 텍스트 크기보다 작은동안 반복한다.
				while (i < this->current->GetLength()
					&& currentRowTextSize.cx < previousRowTextSize.cx)
				{
					//4.2.6.1 i(캐럿의 현재 가로위치)를 증가시킨다(캐럿의 가로 위치를 다음으로 이동시킨다.).
					i = this->current->Next();
					//4.2.6.2 현재 줄의 텍스트를 구한다.(제일 처음부터 현재 캐럿의 가로위치까지)
					currentRowText = CString(this->current->GetPartOfContent(i).c_str());
					//4.2.6.3 현재 줄의 텍스트 크기를 구한다.
					currentRowTextSize = dc.GetTextExtent(currentRowText);
				}
				//4.2.7 현재줄의 텍스트크기에서 이전줄의 텍스트크기를 뺀다.
				Long difference = currentRowTextSize.cx - previousRowTextSize.cx;
				//4.2.8 차이가 0보다 크면(현재줄의 텍스트를 다 읽지 못한 경우)
				if (difference > 0)
				{
					//4.2.8.1 i-1번째 글자를 구한다. 현재 i는 캐럿의 가로 위치를 담고 있고
					CString letter = CString(this->current->GetAt(i - 1)->GetContent().c_str());
					CSize letterSize = dc.GetTextExtent(letter);
					Long halfLetterSize = letterSize.cx / 2;
					//4.2.8.2 차이가 i번째 글자크기의 절반보다 같거나 크면
					if (difference >= halfLetterSize)
					{
						//4.2.8.2.1 캐럿의 현재 가로 위치를 이전으로 이동한다.
						this->current->Previous();
					}
				}
			}
			//4.3 캐럿의 이전 가로 위치가 0 또는 캐럿의 현재 줄의 글자개수가 0개이면
			else
			{
				//4.3.1 현재 캐럿의 가로 위치를 0으로 이동시킨다.
				this->current->First();
			}
		}

	}
	else if (nChar == VK_UP)
	{
		//1. 이전으로 이동하기 전에 캐럿의 현재 세로 위치를 저장한다.
		Long previousRowIndex = this->note->GetCurrent();
		//2. 이전으로 이동하기 전에 캐럿의 현재 가로 위치를 저장한다.
		Long previousCaretIndex = current->GetCurrent();
		//3. 캐럿의 현재 세로 위치를 이전 줄로 이동시킨다.
		Long currentRowIndex = this->note->Previous();
		//4. 이동하기 전 캐럿의 세로 위치와 이동한 후 캐럿의 세로 위치가 서로 다르면(실질적으로 이동을 했으면)
		if (previousRowIndex != currentRowIndex)
		{
			//4.1 현재 줄을 이동한 후 캐럿의 세로 위치가 있는 줄로 변경한다.
			this->current = this->note->GetAt(currentRowIndex);
			//4.2 이동하기 전 캐럿의 가로 위치가 0이 아니고 이동한 후 현재줄의 글자개수가 0이 아니면
			//(이동하기 전 캐럿의 가로 위치가 0이면 이동한 후의 현재 캐럿의 가로 위치도 무조건 0이고,
			//이동한 후 현재줄의 글자개수가 0이어도 현재 캐럿의 가로 위치는 무조건 0임,
			//따라서 그 이외의 경우의 수를 if안에서 처리하고 else에서 위의 2경우를 first로 처리함!)
			if (previousCaretIndex != 0 && this->current->GetLength() != 0)
			{
				//4.2.1 이동하기 전 줄의 텍스트를 구한다.(이동하기 전에 미리 저장한 캐럿의 가로 위치와
				//이동하기 전 캐럿의 세로 위치를 통해서 구함)
				CString previousRowText = CString(this->note->GetAt(previousRowIndex)
					->GetPartOfContent(previousCaretIndex).c_str());
				//4.2.2 이동하기 전 줄의 텍스트 크기를 구한다.
				CSize previousRowTextSize = dc.GetTextExtent(previousRowText);
				//4.2.3 캐럿의 현재 가로 위치를 처음으로 이동시킨다.(현재 캐럿의 가로 위치가 어딘지 모르기때문에)
				//i가 0이 되면 GetPartOfContent에서 읽는 텍스트가 없기 때문에 i의 최소값은 1이 되어야함.
				//GetPartOfContent는 캐럿의 위치까지 있는 글자들을 읽는것임!
				//위 if 조건식에서 이미 현재 줄의 글자가 하나도 없는 경우는 걸러져서 들어왔기 때문에
				//현재줄의 글자수는 최소 1개이상은 있는 경우에 대해서 처리함.
				this->current->First();//캐럿의 가로 위치를 맨 처음(0)으로 보냄
				Long i = this->current->Next();//첫번째 글자를 읽기 위해 캐럿을 1 증가시킴.
				//4.2.4 현재 줄의 텍스트를 구한다. (첫번째 글자를 읽음)
				CString currentRowText = CString(this->current->GetPartOfContent(i).c_str());
				//4.2.5 현재 줄의 텍스트의 크기를 구한다.(첫번째 글자의 텍스트 크기를 구함)
				CSize currentRowTextSize = dc.GetTextExtent(currentRowText);
				//4.2.6 i(캐럿의 현재 가로위치)가 length(현재줄의 글자개수)보다 작고
				//현재 줄의 텍스트 크기가 이동하기 전 줄의 텍스트 크기보다 작은동안 반복한다.
				while (i < this->current->GetLength()
					&& currentRowTextSize.cx < previousRowTextSize.cx)
				{
					//4.2.6.1 i(캐럿의 현재 가로위치)를 증가시킨다(캐럿의 가로 위치를 다음으로 이동시킨다.).
					i = this->current->Next();
					//4.2.6.2 현재 줄의 텍스트를 구한다.(제일 처음부터 현재 캐럿의 가로위치까지)
					currentRowText = CString(this->current->GetPartOfContent(i).c_str());
					//4.2.6.3 현재 줄의 텍스트 크기를 구한다.
					currentRowTextSize = dc.GetTextExtent(currentRowText);
				}

				//4.2.7 i(캐럿의 현재 가로위치)가 length(현재줄의 글자개수)보다 작거나 같으면
				//if (i <= this->current->GetLength()) 이 조건문이 아무 의미가 없음 ㅆㅂ
				//{

				//4.2.7 현재줄의 텍스트크기에서 이전줄의 텍스트크기를 뺀다.
				Long difference = currentRowTextSize.cx - previousRowTextSize.cx;					
				//(현재줄과 이전줄의 텍스트크기 차이가 0이면 텍스트 크기가 같다는 의미이기때문에
				//현재 캐럿의 가로 위치는 지금 위치 그대로에 있으면 됨)
				//(현재줄과 이전줄의 텍스트크기 차이가 음수이면 
				//currentRowTextSize.cx < previousRowTextSize.cx이라는 뜻인데 그러면 위의
				//반복문 조건에서 빠져나오려면 결국 i가 현재줄의 글자개수(length)와 같아서 빠져나왔다는 
				//의미이고 그렇다면 i는 현재 줄의 마지막 캐럿 위치라는 뜻이고 
				//이동하기전 줄의 텍스트가 현재 줄의 전체 텍스트보다 더 큰 경우는 현재줄의 캐럿가로위치는
				//언제나 현재줄의 마지막에 있어야 하고 현재 마지막 위치에 있기 때문에 그대로 두면 된다.
				//4.2.8 차이가 0보다 크면(현재줄의 텍스트를 다 읽지 못한 경우)
				//현재줄의 전체 텍스트가 이전줄의 캐럿 가로 위치까지 읽은 텍스트보다 큰 경우
				if (difference > 0)
				{
					//4.2.8.1 i-1번째 글자를 구한다. 현재 i는 캐럿의 가로 위치를 담고 있고
					//캐럿의 가로 위치는 다음에 쓰여질 글자위치를 저장하고 있기 때문에
					//현재 글자의 위치를 구하기 위해서는 i-1번째 글자를 구해야함.
					CString letter = CString(this->current->GetAt(i - 1)->GetContent().c_str());
					CSize letterSize = dc.GetTextExtent(letter);
					Long halfLetterSize = letterSize.cx / 2;
					//4.2.8.2 차이가 i번째 글자크기의 절반보다 같거나 크면
					if (difference >= halfLetterSize)
					{
						//4.2.8.2.1 캐럿의 현재 가로 위치를 이전으로 이동한다.
						this->current->Previous();
					}
				}
				//}

#if 0
				//i가 length(글자개수)와 같으면
				else if (i == this->current->GetLength())
				{
					//3.7.1 현재줄의 텍스트크기에서 이전줄의 텍스트크기를 뺀다.
					difference = currentRowTextSize.cx - previousRowTextSize.cx;
					//3.7.3 차이가 0보다 크면
					if (difference > 0)
					{
						//3.7.3.1 i-1번째 글자를 구한다. 현재 i는 캐럿의 가로 위치를 담고 있고
						//캐럿의 가로 위치는 다음에 쓰여질 글자위치를 저장하고 있기 때문에
						//현재 글자의 위치를 구하기 위해서는 i-1번째 글자를 구해야함.
						CString letter = CString(this->current->GetAt(i - 1)->GetContent().c_str());
						CSize letterSize = dc.GetTextExtent(letter);
						Long halfLetterSize = letterSize.cx / 2;
						//3.7.3.2 차이가 i번째 글자크기의 절반보다 같거나 크면
						if (difference >= halfLetterSize)
						{
							//3.7.3.2.1 캐럿의 현재 가로 위치를 이전으로 이동한다.
							this->current->Previous();
						}
					}

					//3.8 i가 length(현재줄의 글자개수)보다 크거나 같으면
					//else
					//{
						//3.8.1 현재 캐럿의 가로 위치를 Last로 이동시킨다.
						//this->current->Last();
					//}

				}
#endif
			}
			//4.3 캐럿의 이전 가로 위치가 0 또는 캐럿의 현재 줄의 글자개수가 0개이면
			else
			{
				//4.3.1 현재 캐럿의 가로 위치를 0으로 이동시킨다.
				this->current->First();
			}
		}
	}
	else if (nChar == VK_HOME)
	{
		//Ctrl이 눌려져있으면
		if (ctrlPressedCheck & 0x8000)
		{
			//메모장의 제일 처음으로 캐럿의 세로 위치를 이동시킨다.
			Long firstIndex = this->note->First();
			//메모장의 현재 줄을 변경한다.
			this->current = this->note->GetAt(firstIndex);
			//현재 줄의 맨 처음으로 캐럿의 가로 위치를 이동시킨다.
			this->current->First();
			
		}
		//Ctrl이 안눌려져있으면
		else
		{
			//현재 줄의 맨 처음으로 캐럿의 가로 위치를 이동시킨다.
			this->current->First();
		}
	}
	else if (nChar == VK_END)
	{
		//Ctrl이 눌려져있으면
		if (ctrlPressedCheck & 0x8000)
		{
			//메모장의 제일 마지막으로 캐럿의 세로 위치를 이동시킨다.
			Long lastIndex = this->note->Last();
			//메모장의 현재 줄을 변경한다.
			this->current = this->note->GetAt(lastIndex);
			//현재 줄의 맨 마지막으로 캐럿의 가로 위치를 이동시킨다.
			this->current->Last();
		}
		//Ctrl이 안눌려져있으면
		else
		{
			//현재 줄의 맨 마지막으로 캐럿의 가로 위치를 이동시킨다.
			this->current->Last();
		}
	}
	this->Notify();
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