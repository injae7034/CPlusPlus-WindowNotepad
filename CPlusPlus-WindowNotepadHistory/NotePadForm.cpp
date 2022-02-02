#include "NotepadForm.h"
#include "GlyphCreator.h"

BEGIN_MESSAGE_MAP(NotepadForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_MESSAGE(WM_IME_COMPOSITION, OnComposition)
	ON_MESSAGE(WM_IME_CHAR, OnImeChar)
	ON_MESSAGE(WM_IME_STARTCOMPOSITION, OnStartCompostion)
	//ON_MESSAGE(WM_IME_SETCONTEXT, OnSetContext)
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
	//CFont에서 사용하고자 하는 글자크기와 글자체로 초기화시킴.
	//기본생성자로 생성된 this->font에 매개변수 2개생성자로 치환(=)시킴
	this->font = Font(300, "궁서체");
	//기본생성자로 생성된 this->caret에 매개변수 1개생성자로 치환(=)시킴.
	this->caret = Caret(this);
}

//NotepadForm소멸자
NotepadForm::~NotepadForm()
{

}

//1. 메모장 윈도우가 생성될 때
int NotepadForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//1.1 glyphCreator를 만든다.
	GlyphCreator glyphCreator;
	//1.2 메모장을 만든다.
	this->note = glyphCreator.Create((char*)"\0");
	//1.3 줄을 만든다.
	Glyph* row = glyphCreator.Create((char*)"\n");
	//1.4 줄을 메모장에 추가한다.
	Long rowIndex;
	rowIndex = this->note->Add(row);
	//1.5 현재 줄의 위치를 저장한다.
	this->current = this->note->GetAt(rowIndex);
	
	return 0;
}

//2. 키보드에 글자를 입력할 때
void NotepadForm::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//2.1 CClientDC를 생성한다.
	CClientDC dc(this);
	//2.2 glyphCreator를 생성한다.
	GlyphCreator glyphCreator;
	//2.3 glyph를 생성한다.
	Glyph* glyph = glyphCreator.Create((char*)&nChar);
	Long index;
	CPoint caretPosition;
	//2.4 CFont를 생성한다.
	CFont font;
	//2.5 글씨크기와 글씨체를 정하다.
	font.CreatePointFont(this->font.Getsize(), this->font.GetStyle().c_str());
	//2.6 폰트를 dc에 지정한다.
	dc.SelectObject(font);
	//2.7 TEXTMETRIC을 생성한다.
	TEXTMETRIC text;
	//2.8 글꼴의 정보를 얻는다.
	dc.GetTextMetrics(&text);
	//2.9 캐럿을 생성한다.
	//CreateSolidCaret(0, text.tmHeight);
	this->caret.Create(0, text.tmHeight);
	//2.10 입력받은 문자가 개행문자가 아니면
	if (nChar != '\n' && nChar != '\r')
	{
		//2.10.1 현재 줄에 글자를 추가한다.
		index = this->current->Add(glyph);
		//2.10.2 추가한 글자의 너비를 구한다.
		CString letter = CString(this->current->GetContent().c_str());
		CSize letterSize = dc.GetTextExtent(letter);
		//2.10.3 캐럿이 이동할 위치를 정한다.
		//caretPosition = CPoint(letterSize.cx, (this->note->GetCurrent() - 1) * text.tmHeight);
		//2.10.3 캐럿을 이동시킨다.
		this->caret.Move(letterSize.cx, (this->note->GetCurrent() - 1) * text.tmHeight);
	}
	//2.11 입력받은 문자가 개행문자이면
	else if (nChar == '\n' || nChar == '\r')
	{
		//2.11.1 새로운 줄을 추가한다.
		index = this->note->Add(glyph);
		//2.11.2 현재 줄의 위치를 새로 저장한다.
		this->current = this->note->GetAt(index);
		//2.11.3 캐럿이 이동할 위치를 정한다.
		//caretPosition = CPoint(0, (this->note->GetCurrent() - 1) * text.tmHeight);
		//2.11.3 캐럿을 이동시킨다.
		this->caret.Move(0, (this->note->GetCurrent() - 1) * text.tmHeight);
	}
	//2.12 캐럿의 위치를 이동한다.
	//this->SetCaretPos(caretPosition);
	//2.13 캐럿을 보이게 한다.
	this->caret.Show();
	//ShowCaret();
	//2.14 isComposing을 false로 바꾼다.
	this->IsComposing = false;
	//2.15 갱신한다.
	Invalidate(TRUE);
}

//3. 메모장에 텍스트를 출력할 떄
void NotepadForm::OnPaint()
{
	//3.1 CPaintDC를 생성한다.
	CPaintDC dc(this);
	//3.2 텍스트의 배경을 투명하게함.
	dc.SetBkMode(TRANSPARENT);
	//3.3 텍스트의 색깔을 정함.
	dc.SetTextColor(RGB(0, 0, 0));
	//3.4 왼쪽을 기준선으로 정함.
	dc.SetTextAlign(TA_LEFT);
	//3.5 CFont를 생성한다.
	CFont font;
	//3.6 글씨크기와 글씨체를 정하다.
	font.CreatePointFont(this->font.Getsize(), this->font.GetStyle().c_str());
	//3.7 폰트를 dc에 지정한다.
	dc.SelectObject(font);
	//3.8 TEXTMETRIC을 생성한다.
	TEXTMETRIC text;
	//3.9 글꼴의 정보를 얻는다.
	dc.GetTextMetrics(&text);
	//3.10 note에 저장된 글자들을 출력한다.
	Long i = 0;
	CString content;
	//줄단위의 반복구조를 통해서 줄을 나눠서 줄개수만큼 출력하도록 함.
	while (i < this->note->GetLength())
	{
		content = CString(this->note->GetAt(i)->GetContent().c_str());
		//텍스트 시작 위치설정 처음줄은 (0,0)에서 시작하고 두번째줄은 (0, 150)에서 시작함.
		dc.TextOut(0, i * text.tmHeight, content);
		i++;
	}
#if 0
	//노트를 그대로 출력하면 줄이 구분이 안되고 한줄에 내용이 다나옴.
	content = CString(this->note->GetContent().c_str());
	dc.TextOut(0, i * 150, content);
#endif

#if 0
	//2.8 Note에 저장된 Content를 불러온다.
	CString content = CString(this->note->GetContent().c_str());
	//2.9 Content를 출력한다.
	dc.TextOut(0, 0, _T(this->note->GetContent().c_str()));
#endif
}

//4. 한글을 입력받을 때
LRESULT NotepadForm::OnComposition(WPARAM wParam, LPARAM lParam)
{
	//4.1 CClientDC를 생성한다.
	CClientDC dc(this);
	//4.2 glyphCreator를 생성한다.
	GlyphCreator glyphCreator;
	WORD word = LOWORD(wParam);
	char koreanLetter[3];
	koreanLetter[0] = HIBYTE(word);
	koreanLetter[1] = LOBYTE(word);
	koreanLetter[2] = '\0';
	//4.3 doubleByteLetter를 생성한다.
	Glyph* doubleByteLetter = glyphCreator.Create((char*)koreanLetter);
	//4.4 IsComposing값이 '참'이면
	if (this->IsComposing == true)
	{
		//4.4.1 현재 줄의 기존 한글을 지운다.
		this->current->Remove(this->current->GetLength() - 1);
	}
	//4.5 isComposing값이 '거짓'이면
	else
	{
		//4.5.1 isComposing값을 '참'으로 바꾼다.
		this->IsComposing = true;
	}
	//4.6 새로 만든 DoubleByteLetter를 현재 줄에 추가한다.
	Long letterIndex = this->current->Add(doubleByteLetter);
	//4.7 CFont를 생성한다.
	CFont font;
	//4.8 글씨크기와 글씨체를 정하다.
	font.CreatePointFont(this->font.Getsize(), this->font.GetStyle().c_str());
	//4.9 폰트를 dc에 지정한다.
	dc.SelectObject(font);
	//4.10 추가한 글자를 더한 전체 텍스트의 size를 구한다.
	CString text = CString(this->current->GetContent().c_str());
	CSize textSize = dc.GetTextExtent(text);
	//4.11 추가한 한글의 size를 구한다.
	CString letter = CString(this->current->GetAt(letterIndex)->GetContent().c_str());
	CSize letterSize = dc.GetTextExtent(letter);
	//4.12 캐럿을 생성한다.
	TEXTMETRIC koreanCaret;
	dc.GetTextMetrics(&koreanCaret);
	this->caret.Create(letterSize.cx, koreanCaret.tmHeight);
	//CreateSolidCaret(letterSize.cx, koreanCaret.tmHeight);
	//4.13 캐럿이 이동할 위치를 정한다.
	//CPoint caretPosition = CPoint(textSize.cx - letterSize.cx, (this->note->GetCurrent() - 1) * koreanCaret.tmHeight);
	//4.14 캐럿을 이동시킨다.
	this->caret.Move(textSize.cx - letterSize.cx, (this->note->GetCurrent() - 1) * koreanCaret.tmHeight);
	//this->SetCaretPos(caretPosition);
	//4.15 캐럿을 보이게 한다.
	this->caret.Show();
	//ShowCaret();
	//4.16 갱신한다.
	Invalidate(TRUE);

	return ::DefWindowProc(this->m_hWnd, WM_IME_COMPOSITION, wParam, lParam);
}

//5. 완성된 한글을 입력받았을 때
LRESULT NotepadForm::OnImeChar(WPARAM wParam, LPARAM lParam)
{
	//5.1 CClientDC를 생성한다.
	CClientDC dc(this);
	//5.2 CFont를 생성한다.
	CFont font;
	//5.3 글씨크기와 글씨체를 정하다.
	font.CreatePointFont(this->font.Getsize(), this->font.GetStyle().c_str());
	//5.4 폰트를 dc에 지정한다.
	dc.SelectObject(font);
	//5.5 glyphCreator를 생성한다.
	GlyphCreator glyphCreator;
	WORD word = LOWORD(wParam);
	char koreanLetter[3];
	koreanLetter[0] = HIBYTE(word);
	koreanLetter[1] = LOBYTE(word);
	koreanLetter[2] = '\0';
	//5.6 doubleByteLetter를 생성한다.
	Glyph* doubleByteLetter = glyphCreator.Create((char*)koreanLetter);
	//5.7 기존에 조립중이던 한글을 지운다.
	this->current->Remove(this->current->GetLength() - 1);
	//5.8 현재줄에 완성된 한글을 추가한다.
	this->current->Add(doubleByteLetter);
	//5.9 IsComposing을 false로 바꾼다.
	this->IsComposing = false;
	//5.10 캐럿을 생성한다
	TEXTMETRIC text;
	dc.GetTextMetrics(&text);
	this->caret.Create(0, text.tmHeight);
	//CreateSolidCaret(0, text.tmHeight);
	//5.11 캐럿이 이동할 위치를 정한다.
	CString letter = CString(this->current->GetContent().c_str());
	CSize letterSize = dc.GetTextExtent(letter);
	//CPoint caretPosition = CPoint(letterSize.cx, (this->note->GetCurrent() - 1) * text.tmHeight);
	//5.12 캐럿을 이동시킨다.
	this->caret.Move(letterSize.cx, (this->note->GetCurrent() - 1) * text.tmHeight);
	//SetCaretPos(caretPosition);
	//5.13 캐럿을 보여준다.
	this->caret.Show();
	//ShowCaret();
	//5.14 갱신한다.
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
	//CreateSolidCaret(0, text.tmHeight);
	//8. 현재줄의 텍스트들을 저장한다.
	CString letter = CString(this->current->GetContent().c_str());
	//9. 현재줄의 텍스트들의 size를 구한다.
	CSize letterSize = dc.GetTextExtent(letter);
	//10. 캐럿이 이동할 위치를 정한다.
	//CPoint caretPosition = CPoint(letterSize.cx, (this->note->GetCurrent() - 1) * text.tmHeight);
	//11. 캐럿을 이동시킨다.
	this->caret.Move(letterSize.cx, (this->note->GetCurrent() - 1) * text.tmHeight);
	//SetCaretPos(caretPosition);
	//12. 캐럿을 보이게 한다.
	this->caret.Show();
	//ShowCaret();

#if 0
	//Caret의 위치 지정

		CSize strSize;

	strSize = dc.GetTextExtent(str);

	pt.x = strSize.cx;

	pt.y = Line * txtKey.tmHeight;

	SetCaret(pt);

	ShowCaret();
#endif
}

//메모장이 Focus를 잃을 때
void NotepadForm::OnKillFocus(CWnd* pNewWnd)
{
	//1. 캐럿을 숨긴다.
	HideCaret();
	//2. 캐럿을 파괴한다.
	::DestroyCaret();
}



#if 0
//6. 한글 조립과정을 다른 윈도우에서 안보이게 하기 위한 조치
LRESULT NotepadForm::OnSetContext(WPARAM wParam, LPARAM lParam)
{
	lParam &= ~(ISC_SHOWUICOMPOSITIONWINDOW | ISC_SHOWUIALLCANDIDATEWINDOW);

	return 0;
}
#endif