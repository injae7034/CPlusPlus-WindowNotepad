#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "Glyph.h"

BEGIN_MESSAGE_MAP(NotepadForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_WM_PAINT()
	ON_MESSAGE(WM_IME_COMPOSITION, OnComposition)
	ON_MESSAGE(WM_IME_CHAR, OnImeChar)
	ON_MESSAGE(WM_IME_STARTCOMPOSITION, OnStartCompostion)
	//ON_MESSAGE(WM_IME_SETCONTEXT, OnSetContext)
END_MESSAGE_MAP()

//NotepadForm생성자
NotepadForm::NotepadForm()
	:CFrameWnd()
{
	this->note = NULL;//NULL로 초기화시킴.
	this->current = NULL;//NULL로 초기화시킴.
	this->IsComposing = false;//false로 초기화시킴
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
	//2.1 glyphCreator를 생성한다.
	GlyphCreator glyphCreator;
	//2.2 glyph를 생성한다.
	Glyph* glyph = glyphCreator.Create((char*)&nChar);
	Long index;
	//2.3 입력받은 문자가 개행문자가 아니면
	if (nChar != '\n' && nChar != '\r')
	{
		//2.3.1 현재 줄에 글자를 추가한다.
		index = this->current->Add(glyph);
	}
	//2.4 입력받은 문자가 개행문자이면
	else if (nChar == '\n' || nChar == '\r')
	{
		//2.4.1 새로운 줄을 추가한다.
		index = this->note->Add(glyph);
		//2.4.2 현재 줄의 위치를 새로 저장한다.
		this->current = this->note->GetAt(index);
	}
	//2.5 isComposing을 false로 바꾼다.
	this->IsComposing = false;
	//2.6 갱신한다.
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
	font.CreatePointFont(500, _T("나눔바른펜"));
	//3.7 폰트를 dc에 지정한다.
	dc.SelectObject(font);
	//3.8 note에 저장된 글자들을 출력한다.
	Long i = 0;
	CString content;
	//줄단위의 반복구조를 통해서 줄을 나눠서 줄개수만큼 출력하도록 함.
	while (i < this->note->GetLength())
	{
		content = CString(this->note->GetAt(i)->GetContent().c_str());
		dc.TextOut(0, i * 150, content);
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
	//4.1 glyphCreator를 생성한다.
	GlyphCreator glyphCreator;
	WORD word = LOWORD(wParam);
	char koreanLetter[3];
	koreanLetter[0] = HIBYTE(word);
	koreanLetter[1] = LOBYTE(word);
	koreanLetter[2] = '\0';
	//4.2 doubleByteLetter를 생성한다.
	Glyph* doubleByteLetter = glyphCreator.Create((char*)koreanLetter);
	//4.3 IsComposing값이 '참'이면
	if (this->IsComposing == true)
	{
		//4.3.1 현재 줄의 기존 한글을 지운다.
		this->current->Remove(this->current->GetLength() - 1);
	}
	//4.4 isComposing값이 '거짓'이면
	else
	{
		//4.4.1 isComposing값을 '참'으로 바꾼다.
		this->IsComposing = true;
	}
	//4.5 새로 만든 DoubleByteLetter를 현재 줄에 추가한다.
	this->current->Add(doubleByteLetter);
	//4.6 갱신한다.
	Invalidate(TRUE);

	return ::DefWindowProc(this->m_hWnd, WM_IME_COMPOSITION, wParam, lParam);
}

//5. 완성된 한글을 입력받았을 때
LRESULT NotepadForm::OnImeChar(WPARAM wParam, LPARAM lParam)
{
	//5.1 glyphCreator를 생성한다.
	GlyphCreator glyphCreator;
	WORD word = LOWORD(wParam);
	char koreanLetter[3];
	koreanLetter[0] = HIBYTE(word);
	koreanLetter[1] = LOBYTE(word);
	koreanLetter[2] = '\0';
	//5.2 doubleByteLetter를 생성한다.
	Glyph* doubleByteLetter = glyphCreator.Create((char*)koreanLetter);
	//5.3 기존에 조립중이던 한글을 지운다.
	this->current->Remove(this->current->GetLength() - 1);
	//5.4 현재줄에 완성된 한글을 추가한다.
	this->current->Add(doubleByteLetter);
	//5.5 IsComposing을 false로 바꾼다.
	this->IsComposing = false;

	//5.6 갱신한다.
	Invalidate(TRUE);

	return 0;
}

//한글 조립 시작을 알림
LRESULT NotepadForm::OnStartCompostion(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

#if 0
//6. 한글 조립과정을 다른 윈도우에서 안보이게 하기 위한 조치
LRESULT NotepadForm::OnSetContext(WPARAM wParam, LPARAM lParam)
{
	lParam &= ~(ISC_SHOWUICOMPOSITIONWINDOW | ISC_SHOWUIALLCANDIDATEWINDOW);

	return 0;
}
#endif
