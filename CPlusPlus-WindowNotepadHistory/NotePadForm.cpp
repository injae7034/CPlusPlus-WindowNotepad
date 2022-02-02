#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandCreator.h"
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
	//해당범위(IDM_FILE_OPEN ~ IDM_FILE_SAVEDIFFERENTNAME)의 id들을 클릭하면 OnCommand함수실행
	ON_COMMAND_RANGE(IDM_FILE_OPEN, IDM_FILE_SAVEDIFFERENTNAME, OnCommand)
	//ON_COMMAND(IDM_FILE_OPEN, OnFileOpenClicked)
	//ON_COMMAND(IDM_FILE_SAVE, OnFileSaveClicked)
	//ON_COMMAND(IDM_FILE_SAVEDIFFERENTNAME, OnFileSaveDifferentNameClicked)
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
	//기본생성자로 생성된 this->font에 매개변수 2개생성자로 치환(=)시킴
	this->font = Font(300, "궁서체");
	//기본생성자로 생성된 this->caret에 매개변수 1개생성자로 치환(=)시킴.
	this->caret = Caret(this);
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
	//1. CClientDC를 생성한다.
	CClientDC dc(this);
	//2. glyphCreator를 생성한다.
	GlyphCreator glyphCreator;
	//3. glyph를 생성한다.
	Glyph* glyph = glyphCreator.Create((char*)&nChar);
	Long index;
	//4. CFont를 생성한다.
	CFont font;
	//5. 글씨크기와 글씨체를 정하다.
	font.CreatePointFont(this->font.Getsize(), this->font.GetStyle().c_str());
	//6. 폰트를 dc에 지정한다.
	dc.SelectObject(font);
	//7. TEXTMETRIC을 생성한다.
	TEXTMETRIC text;
	//8. 글꼴의 정보를 얻는다.
	dc.GetTextMetrics(&text);
	//9. 캐럿을 생성한다.
	this->caret.Create(0, text.tmHeight);
	//10. 입력받은 문자가 개행문자가 아니면
	if (nChar != '\n' && nChar != '\r')
	{
		//10.1 현재 줄에 글자를 추가한다.
		index = this->current->Add(glyph);
		//10.2 추가한 글자의 너비를 구한다.
		CString letter = CString(this->current->GetContent().c_str());
		CSize letterSize = dc.GetTextExtent(letter);
		//10.3 캐럿을 이동시킨다.
		this->caret.Move(letterSize.cx, (this->note->GetCurrent() - 1) * text.tmHeight);
	}
	//11. 입력받은 문자가 개행문자이면
	else if (nChar == '\n' || nChar == '\r')
	{
		//11.1 새로운 줄을 추가한다.
		index = this->note->Add(glyph);
		//11.2 현재 줄의 위치를 새로 저장한다.
		this->current = this->note->GetAt(index);
		//11.3 캐럿을 이동시킨다.
		this->caret.Move(0, (this->note->GetCurrent() - 1) * text.tmHeight);
	}
	//12. 캐럿을 보이게 한다.
	this->caret.Show();
	//13. isComposing을 false로 바꾼다.
	this->IsComposing = false;
	//14. 메모장 제목에 *를 추가한다.
	string name = this->fileName;
	name.insert(0, "*");
	name += " - 메모장";
	SetWindowText(CString(name.c_str()));
	//15. 메모장에 변경사항이 있음을 저장한다.
	this->IsDirty = true;
	//16. 갱신한다.
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
	dc.SetTextColor(RGB(0, 0, 0));
	//4. 왼쪽을 기준선으로 정함.
	dc.SetTextAlign(TA_LEFT);
	//5. CFont를 생성한다.
	CFont font;
	//6. 글씨크기와 글씨체를 정하다.
	font.CreatePointFont(this->font.Getsize(), this->font.GetStyle().c_str());
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
}

//한글을 입력받을 때
LRESULT NotepadForm::OnComposition(WPARAM wParam, LPARAM lParam)
{
	//1. CClientDC를 생성한다.
	CClientDC dc(this);
	//2. glyphCreator를 생성한다.
	GlyphCreator glyphCreator;
	WORD word = LOWORD(wParam);
	char koreanLetter[3];
	koreanLetter[0] = HIBYTE(word);
	koreanLetter[1] = LOBYTE(word);
	koreanLetter[2] = '\0';
	//3. doubleByteLetter를 생성한다.
	Glyph* doubleByteLetter = glyphCreator.Create((char*)koreanLetter);
	//4. IsComposing값이 '참'이면
	if (this->IsComposing == true)
	{
		//4.1 현재 줄의 기존 한글을 지운다.
		this->current->Remove(this->current->GetLength() - 1);
	}
	//5. isComposing값이 '거짓'이면
	else
	{
		//5.1 isComposing값을 '참'으로 바꾼다.
		this->IsComposing = true;
	}
	//6. 새로 만든 DoubleByteLetter를 현재 줄에 추가한다.
	Long letterIndex = this->current->Add(doubleByteLetter);
	//7. CFont를 생성한다.
	CFont font;
	//8. 글씨크기와 글씨체를 정하다.
	font.CreatePointFont(this->font.Getsize(), this->font.GetStyle().c_str());
	//9. 폰트를 dc에 지정한다.
	dc.SelectObject(font);
	//10. 추가한 글자를 더한 전체 텍스트의 size를 구한다.
	CString text = CString(this->current->GetContent().c_str());
	CSize textSize = dc.GetTextExtent(text);
	//11. 추가한 한글의 size를 구한다.
	CString letter = CString(this->current->GetAt(letterIndex)->GetContent().c_str());
	CSize letterSize = dc.GetTextExtent(letter);
	//12. 캐럿을 생성한다.
	TEXTMETRIC koreanCaret;
	dc.GetTextMetrics(&koreanCaret);
	this->caret.Create(letterSize.cx, koreanCaret.tmHeight);
	//CreateSolidCaret(letterSize.cx, koreanCaret.tmHeight);
	//13. 캐럿을 이동시킨다.
	this->caret.Move(textSize.cx - letterSize.cx, (this->note->GetCurrent() - 1) * koreanCaret.tmHeight);
	//14. 캐럿을 보이게 한다.
	this->caret.Show();
	//15. 메모장 제목에 *를 추가한다.
	string name = this->fileName;
	name.insert(0, "*");
	name += " - 메모장";
	SetWindowText(CString(name.c_str()));
	//16. 메모장에 변경사항이 있음을 저장한다.
	this->IsDirty = true;
	//17. 갱신한다.
	Invalidate(TRUE);

	return ::DefWindowProc(this->m_hWnd, WM_IME_COMPOSITION, wParam, lParam);
}

//완성된 한글을 입력받았을 때
LRESULT NotepadForm::OnImeChar(WPARAM wParam, LPARAM lParam)
{
	//1. CClientDC를 생성한다.
	CClientDC dc(this);
	//2. CFont를 생성한다.
	CFont font;
	//3. 글씨크기와 글씨체를 정하다.
	font.CreatePointFont(this->font.Getsize(), this->font.GetStyle().c_str());
	//4. 폰트를 dc에 지정한다.
	dc.SelectObject(font);
	//5. glyphCreator를 생성한다.
	GlyphCreator glyphCreator;
	WORD word = LOWORD(wParam);
	char koreanLetter[3];
	koreanLetter[0] = HIBYTE(word);
	koreanLetter[1] = LOBYTE(word);
	koreanLetter[2] = '\0';
	//6. doubleByteLetter를 생성한다.
	Glyph* doubleByteLetter = glyphCreator.Create((char*)koreanLetter);
	//7. 기존에 조립중이던 한글을 지운다.
	this->current->Remove(this->current->GetLength() - 1);
	//8. 현재줄에 완성된 한글을 추가한다.
	this->current->Add(doubleByteLetter);
	//9. IsComposing을 false로 바꾼다.
	this->IsComposing = false;
	//10. 캐럿을 생성한다
	TEXTMETRIC text;
	dc.GetTextMetrics(&text);
	this->caret.Create(0, text.tmHeight);
	//11. 캐럿이 이동할 위치를 정한다.
	CString letter = CString(this->current->GetContent().c_str());
	CSize letterSize = dc.GetTextExtent(letter);
	//12. 캐럿을 이동시킨다.
	this->caret.Move(letterSize.cx, (this->note->GetCurrent() - 1) * text.tmHeight);
	//13. 캐럿을 보여준다.
	this->caret.Show();
	//14 메모장 제목에 *를 추가한다.
	string name = this->fileName;
	name.insert(0, "*");
	name += " - 메모장";
	SetWindowText(CString(name.c_str()));
	//15. 메모장에 변경사항이 있음을 저장한다.
	this->IsDirty = true;
	//16. 갱신한다.
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
	//1. CClientDC를 생성한다.
	CClientDC dc(this);
	//2. CFont를 생성한다.
	CFont font;
	//3. 글씨크기와 글씨체를 정하다.
	font.CreatePointFont(this->font.Getsize(), this->font.GetStyle().c_str());
	//4. 폰트를 dc에 지정한다.
	dc.SelectObject(font);
	//5. TEXTMETRIC을 생성한다.
	TEXTMETRIC text;
	//6. 글꼴의 정보를 얻는다.
	dc.GetTextMetrics(&text);
	//7. 캐럿을 생성한다.
	this->caret.Create(0, text.tmHeight);
	//8. 현재줄의 텍스트들을 저장한다.
	CString letter = CString(this->current->GetContent().c_str());
	//9. 현재줄의 텍스트들의 size를 구한다.
	CSize letterSize = dc.GetTextExtent(letter);
	//10. 캐럿을 이동시킨다.
	this->caret.Move(letterSize.cx, (this->note->GetCurrent() - 1) * text.tmHeight);
	//11. 캐럿을 보이게 한다.
	this->caret.Show();
}

//메모장이 Focus를 잃을 때
void NotepadForm::OnKillFocus(CWnd* pNewWnd)
{
	//1. 캐럿을 숨긴다.
	HideCaret();
	//2. 캐럿을 파괴한다.
	::DestroyCaret();
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

#if 0
//메모장 메뉴에서 불러오기를 클릭했을 떄
void NotepadForm::OnFileOpenClicked()
{
	//1. 메모장 제목을 알아낸다.
	CString noteTitle;
	GetWindowText(noteTitle);
	string title = string(noteTitle);
	//2. 메모장 제목에 '*'가 있으면(변경사항이 있으면)
	if (title[0] == '*')
	{
		//메세지박스 문구를 정한다.
		string message;
		//2.1 파일경로가 정해져 있으면
		if (this->file.GetPath() != "")
		{
			message = this->file.GetPath();
		}
		//2.2 파일 경로가 안정해져 있으면
		else
		{
			message = this->file.GetName();
		}
		message.insert(0, "변경 내용을 ");
		message += "에 저장하시겠습니까?";
		//2.1 Save메세지박스를 출력한다.
		int messageBoxButton = SaveMessageBox(this->GetSafeHwnd(), message.c_str(), "메모장", MB_YESNOCANCEL);

		//3. Save메세지박스에서 Yes를 선택했으면
		if (messageBoxButton == IDYES)
		{
			//1. 파일경로가 ""이 아니면(노트가 처음 생성된 경우가 아니면)
			if (this->file.GetPath() != "")
			{
				//1.1 선택한 메모장을 저장한다.
				this->file.Save(this->file.GetName(), this->file.GetPath());
				//1.2 '*'를 없애준다.
				title.erase(0, 1);
				SetWindowText(CString(title.c_str()));
				
			}
			//2.2 파일경로가 정해져 있지 않으면(노트가 처음 생성된 경우)
			else
			{
				//2.2.1 파일공통 대화상자를 생성한다.
				LPCTSTR  filesFilter = _T("텍스트 문서(*.txt) | *.txt; | 모든 파일 | *.*;  ||");
				CFileDialog fileDialog(FALSE, _T("txt"), "*.txt", 0, filesFilter, this, 0, 1);
				//2.2.2 파일공통 대화상자를 출력한다.
				if (fileDialog.DoModal() == IDOK)
				{
					//2.2.2.1 저장할 파일의 이름을 구한다.
					CString fileName = fileDialog.GetFileName();
					//2.2.2.2 저장할 파일의 경로를 구한다.
					CString filePathName = fileDialog.GetPathName();
					//2.5 파일의 이름에서 .txt를 제거한다.
					string name = string(fileName);
					Long eraseIndex = name.find(".txt");
					name.erase(eraseIndex, 4);
					//2.2.2.3 선택한 메모장을 저장한다.
					this->file.Save(name, string(filePathName));
					//2.13 메모장 제목을 바꾼다.
					name += " - 메모장";
					SetWindowText(CString(name.c_str()));
				}
			}
		}
		//4. 메세지박스에서 yes나 no를 선택한 경우
		if (messageBoxButton == IDYES || messageBoxButton == IDNO)
		{
			//3.1 파일공통 대화상자를 생성한다.
			LPCTSTR  filesFilter = _T("텍스트 문서(*.txt) | *.txt; | 모든 파일 | *.*;  ||");
			CFileDialog fileDialog(TRUE, _T("txt"), NULL, 0, filesFilter, this, 0, 1);
			//3.2 파일공통 대화상자를 출력한다.
			if (fileDialog.DoModal() == IDOK)
			{
				//3.2.1 불러올 파일의 이름을 구한다.
				CString fileName = fileDialog.GetFileName();
				//3.2.2 불러올 파일의 경로를 구한다.
				CString filePathName = fileDialog.GetPathName();
				//3.2.3 파일의 이름에서 .txt를 제거한다.
				string name = string(fileName);
				Long eraseIndex = name.find(".txt");
				name.erase(eraseIndex, 4);
				//3.2.4 기존 메모장의 note를 지운다.
				//메모장에 note는 한개밖에 없다 따라서 다른 메모장을 불러오는 순간 기존note는 없애고
				//새로 불러오는 메모장의 note를 Load를 통해 불러오는 새로운 메모장에 출력해줘야함!!
				if (this->note != NULL)
				{
					delete this->note;
					//this->note를 소멸시키면 note에 있는 Row와 letter들도 다 소멸된다.
					//this->current는 Row인데 이미 this->note를 소멸시키면서 Row들이 다 소멸되었는데
					//또 Row를 소멸하라고 하면 소멸할게 없는데 소멸하라고 했기때문에 뻑이난다.!!!!!
					//delete this->current;
				}
				//불러오는 메모장을 위해 새로운 note를 만듬.
				//3.2.5 glyphCreator를 만든다.
				GlyphCreator glyphCreator;
				//3.2.6 노트를 만든다.
				this->note = glyphCreator.Create((char*)"\0");
				//3.2.7 줄을 만든다.
				Glyph* row = glyphCreator.Create((char*)"\n");
				//3.2.8 줄을 메모장에 추가한다.
				Long rowIndex;
				rowIndex = this->note->Add(row);
				//3.2.9 현재 줄의 위치를 저장한다.
				this->current = this->note->GetAt(rowIndex);
				//3.2.10 선택한 메모장의 노트(내용)를 불러온다.
				this->file.Load(name, string(filePathName));
				//3.2.11 메모장 제목을 바꾼다.
				name += " - 메모장";
				SetWindowText(CString(name.c_str()));
				//3.2.12 갱신한다.
				Invalidate(TRUE);
				//캐럿을 불러온 note(내용)에 맞게 다시 생성해준다.
				//3.2.13 CClientDC를 생성한다.
				CClientDC dc(this);
				//3.2.14 CFont를 생성한다.
				CFont font;
				//3.2.15 글씨크기와 글씨체를 정하다.
				font.CreatePointFont(this->font.Getsize(), this->font.GetStyle().c_str());
				//3.2.16 폰트를 dc에 지정한다.
				dc.SelectObject(font);
				//3.2.17 TEXTMETRIC을 생성한다.
				TEXTMETRIC text;
				//3.2.18 글꼴의 정보를 얻는다.
				dc.GetTextMetrics(&text);
				//3.2.19 캐럿을 생성한다.
				this->caret.Create(0, text.tmHeight);
				//3.2.20 현재줄의 텍스트들을 저장한다.
				CString letter = CString(this->current->GetContent().c_str());
				//3.2.21 현재줄의 텍스트들의 size를 구한다.
				CSize letterSize = dc.GetTextExtent(letter);
				//3.2.22 캐럿을 이동시킨다.
				this->caret.Move(letterSize.cx, (this->note->GetCurrent() - 1) * text.tmHeight);
				//3.2.23 캐럿을 보이게 한다.
				this->caret.Show();
			}
		}
	}
	//변경사항이 없으면(제목 앞에 '*'이 없으면)
	else
	{
		//3.1 파일공통 대화상자를 생성한다.
		LPCTSTR  filesFilter = _T("텍스트 문서(*.txt) | *.txt; | 모든 파일 | *.*;  ||");
		CFileDialog fileDialog(TRUE, _T("txt"), NULL, 0, filesFilter, this, 0, 1);
		//3.2 파일공통 대화상자를 출력한다.
		if (fileDialog.DoModal() == IDOK)
		{
			//3.2.1 불러올 파일의 이름을 구한다.
			CString fileName = fileDialog.GetFileName();
			//3.2.2 불러올 파일의 경로를 구한다.
			CString filePathName = fileDialog.GetPathName();
			//3.2.3 파일의 이름에서 .txt를 제거한다.
			string name = string(fileName);
			Long eraseIndex = name.find(".txt");
			name.erase(eraseIndex, 4);
			//3.2.4 기존 메모장의 note를 지운다.
			//메모장에 note는 한개밖에 없다 따라서 다른 메모장을 불러오는 순간 기존note는 없애고
			//새로 불러오는 메모장의 note를 Load를 통해 불러오는 새로운 메모장에 출력해줘야함!!
			if (this->note != NULL)
			{
				delete this->note;
				//this->note를 소멸시키면 note에 있는 Row와 letter들도 다 소멸된다.
				//this->current는 Row인데 이미 this->note를 소멸시키면서 Row들이 다 소멸되었는데
				//또 Row를 소멸하라고 하면 소멸할게 없는데 소멸하라고 했기때문에 뻑이난다.!!!!!
				//delete this->current;
			}
			//불러오는 메모장을 위해 새로운 note를 만듬.
			//3.2.5 glyphCreator를 만든다.
			GlyphCreator glyphCreator;
			//3.2.6 노트를 만든다.
			this->note = glyphCreator.Create((char*)"\0");
			//3.2.7 줄을 만든다.
			Glyph* row = glyphCreator.Create((char*)"\n");
			//3.2.8 줄을 메모장에 추가한다.
			Long rowIndex;
			rowIndex = this->note->Add(row);
			//3.2.9 현재 줄의 위치를 저장한다.
			this->current = this->note->GetAt(rowIndex);
			//3.2.10 선택한 메모장의 노트(내용)를 불러온다.
			this->file.Load(name, string(filePathName));
			//3.2.11 메모장 제목을 바꾼다.
			name += " - 메모장";
			SetWindowText(CString(name.c_str()));
			//3.2.12 갱신한다.
			Invalidate(TRUE);
			//캐럿을 불러온 note(내용)에 맞게 다시 생성해준다.
			//3.2.13 CClientDC를 생성한다.
			CClientDC dc(this);
			//3.2.14 CFont를 생성한다.
			CFont font;
			//3.2.15 글씨크기와 글씨체를 정하다.
			font.CreatePointFont(this->font.Getsize(), this->font.GetStyle().c_str());
			//3.2.16 폰트를 dc에 지정한다.
			dc.SelectObject(font);
			//3.2.17 TEXTMETRIC을 생성한다.
			TEXTMETRIC text;
			//3.2.18 글꼴의 정보를 얻는다.
			dc.GetTextMetrics(&text);
			//3.2.19 캐럿을 생성한다.
			this->caret.Create(0, text.tmHeight);
			//3.2.20 현재줄의 텍스트들을 저장한다.
			CString letter = CString(this->current->GetContent().c_str());
			//3.2.21 현재줄의 텍스트들의 size를 구한다.
			CSize letterSize = dc.GetTextExtent(letter);
			//3.2.22 캐럿을 이동시킨다.
			this->caret.Move(letterSize.cx, (this->note->GetCurrent() - 1) * text.tmHeight);
			//3.2.23 캐럿을 보이게 한다.
			this->caret.Show();
		}
	}
}

//메모장 메뉴에서 저장하기를 클릭했을 때
void NotepadForm::OnFileSaveClicked()
{
	//1. 파일경로가 ""이 아니면(노트가 처음 생성된 경우가 아니면)
	if (this->file.GetPath() != "")
	{
		//1.1 선택한 메모장을 저장한다.
		this->file.Save(this->file.GetName(), this->file.GetPath());
		//1.2 메모장 제목을 알아낸다.
		CString noteTitle;
		GetWindowText(noteTitle);
		string title = string(noteTitle);
		//1.3 메모장 제목에 '*'가 있으면(변경사항이 있으면)
		if (title[0] == '*')
		{
			//1.3.4 '*'를 없애준다.
			title.erase(0, 1);
			SetWindowText(CString(title.c_str()));
		}
	}
	//2.2 파일경로가 정해져 있지 않으면(노트가 처음 생성된 경우)
	else
	{
		//2.2.1 파일공통 대화상자를 생성한다.
		LPCTSTR  filesFilter = _T("텍스트 문서(*.txt) | *.txt; | 모든 파일 | *.*;  ||");
		CFileDialog fileDialog(FALSE, _T("txt"), "*.txt", 0, filesFilter, this, 0, 1);
		//2.2.2 파일공통 대화상자를 출력한다.
		if (fileDialog.DoModal() == IDOK)
		{
			//2.2.2.1 저장할 파일의 이름을 구한다.
			CString fileName = fileDialog.GetFileName();
			//2.2.2.2 저장할 파일의 경로를 구한다.
			CString filePathName = fileDialog.GetPathName();
			//2.5 파일의 이름에서 .txt를 제거한다.
			string name = string(fileName);
			Long eraseIndex = name.find(".txt");
			name.erase(eraseIndex, 4);
			//2.2.2.3 선택한 메모장을 저장한다.
			this->file.Save(name, string(filePathName));
			//2.13 메모장 제목을 바꾼다.
			name += " - 메모장";
			SetWindowText(CString(name.c_str()));
		}
	}	
}

//메모장에서 다른 이름으로 저장하기 버튼을 클릭했을 떄
void NotepadForm::OnFileSaveDifferentNameClicked()
{
	//2.2.1 파일공통 대화상자를 생성한다.
	LPCTSTR  filesFilter = _T("텍스트 문서(*.txt) | *.txt; | 모든 파일 | *.*;  ||");
	CFileDialog fileDialog(FALSE, _T("txt"), "*.txt", 0, filesFilter, this, 0, 1);
	//2.2.2 파일공통 대화상자를 출력한다.
	if (fileDialog.DoModal() == IDOK)
	{
		//2.2.2.1 저장할 파일의 이름을 구한다.
		CString fileName = fileDialog.GetFileName();
		//2.2.2.2 저장할 파일의 경로를 구한다.
		CString filePathName = fileDialog.GetPathName();
		//2.5 파일의 이름에서 .txt를 제거한다.
		string name = string(fileName);
		Long eraseIndex = name.find(".txt");
		name.erase(eraseIndex, 4);
		//2.2.2.3 선택한 메모장을 저장한다.
		this->file.Save(name, string(filePathName));
		//2.13 메모장 제목을 바꾼다.
		name += " - 메모장";
		SetWindowText(CString(name.c_str()));
	}
}
#endif

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