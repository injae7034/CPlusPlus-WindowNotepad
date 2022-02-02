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

//NotepadForm������
NotepadForm::NotepadForm()
	:CFrameWnd()
{
	this->note = NULL;//NULL�� �ʱ�ȭ��Ŵ.
	this->current = NULL;//NULL�� �ʱ�ȭ��Ŵ.
	this->IsComposing = false;//false�� �ʱ�ȭ��Ŵ
}

//NotepadForm�Ҹ���
NotepadForm::~NotepadForm()
{

}

//1. �޸��� �����찡 ������ ��
int NotepadForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//1.1 glyphCreator�� �����.
	GlyphCreator glyphCreator;
	//1.2 �޸����� �����.
	this->note = glyphCreator.Create((char*)"\0");
	//1.3 ���� �����.
	Glyph* row = glyphCreator.Create((char*)"\n");
	//1.4 ���� �޸��忡 �߰��Ѵ�.
	Long rowIndex;
	rowIndex = this->note->Add(row);
	//1.5 ���� ���� ��ġ�� �����Ѵ�.
	this->current = this->note->GetAt(rowIndex);
	
	return 0;
}

//2. Ű���忡 ���ڸ� �Է��� ��
void NotepadForm::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//2.1 glyphCreator�� �����Ѵ�.
	GlyphCreator glyphCreator;
	//2.2 glyph�� �����Ѵ�.
	Glyph* glyph = glyphCreator.Create((char*)&nChar);
	Long index;
	//2.3 �Է¹��� ���ڰ� ���๮�ڰ� �ƴϸ�
	if (nChar != '\n' && nChar != '\r')
	{
		//2.3.1 ���� �ٿ� ���ڸ� �߰��Ѵ�.
		index = this->current->Add(glyph);
	}
	//2.4 �Է¹��� ���ڰ� ���๮���̸�
	else if (nChar == '\n' || nChar == '\r')
	{
		//2.4.1 ���ο� ���� �߰��Ѵ�.
		index = this->note->Add(glyph);
		//2.4.2 ���� ���� ��ġ�� ���� �����Ѵ�.
		this->current = this->note->GetAt(index);
	}
	//2.5 isComposing�� false�� �ٲ۴�.
	this->IsComposing = false;
	//2.6 �����Ѵ�.
	Invalidate(TRUE);
}

//3. �޸��忡 �ؽ�Ʈ�� ����� ��
void NotepadForm::OnPaint()
{
	//3.1 CPaintDC�� �����Ѵ�.
	CPaintDC dc(this);
	//3.2 �ؽ�Ʈ�� ����� �����ϰ���.
	dc.SetBkMode(TRANSPARENT);
	//3.3 �ؽ�Ʈ�� ������ ����.
	dc.SetTextColor(RGB(0, 0, 0));
	//3.4 ������ ���ؼ����� ����.
	dc.SetTextAlign(TA_LEFT);
	//3.5 CFont�� �����Ѵ�.
	CFont font;
	//3.6 �۾�ũ��� �۾�ü�� ���ϴ�.
	font.CreatePointFont(500, _T("�����ٸ���"));
	//3.7 ��Ʈ�� dc�� �����Ѵ�.
	dc.SelectObject(font);
	//3.8 note�� ����� ���ڵ��� ����Ѵ�.
	Long i = 0;
	CString content;
	//�ٴ����� �ݺ������� ���ؼ� ���� ������ �ٰ�����ŭ ����ϵ��� ��.
	while (i < this->note->GetLength())
	{
		content = CString(this->note->GetAt(i)->GetContent().c_str());
		dc.TextOut(0, i * 150, content);
		i++;
	}
#if 0
	//��Ʈ�� �״�� ����ϸ� ���� ������ �ȵǰ� ���ٿ� ������ �ٳ���.
	content = CString(this->note->GetContent().c_str());
	dc.TextOut(0, i * 150, content);
#endif

#if 0
	//2.8 Note�� ����� Content�� �ҷ��´�.
	CString content = CString(this->note->GetContent().c_str());
	//2.9 Content�� ����Ѵ�.
	dc.TextOut(0, 0, _T(this->note->GetContent().c_str()));
#endif
}

//4. �ѱ��� �Է¹��� ��
LRESULT NotepadForm::OnComposition(WPARAM wParam, LPARAM lParam)
{
	//4.1 glyphCreator�� �����Ѵ�.
	GlyphCreator glyphCreator;
	WORD word = LOWORD(wParam);
	char koreanLetter[3];
	koreanLetter[0] = HIBYTE(word);
	koreanLetter[1] = LOBYTE(word);
	koreanLetter[2] = '\0';
	//4.2 doubleByteLetter�� �����Ѵ�.
	Glyph* doubleByteLetter = glyphCreator.Create((char*)koreanLetter);
	//4.3 IsComposing���� '��'�̸�
	if (this->IsComposing == true)
	{
		//4.3.1 ���� ���� ���� �ѱ��� �����.
		this->current->Remove(this->current->GetLength() - 1);
	}
	//4.4 isComposing���� '����'�̸�
	else
	{
		//4.4.1 isComposing���� '��'���� �ٲ۴�.
		this->IsComposing = true;
	}
	//4.5 ���� ���� DoubleByteLetter�� ���� �ٿ� �߰��Ѵ�.
	this->current->Add(doubleByteLetter);
	//4.6 �����Ѵ�.
	Invalidate(TRUE);

	return ::DefWindowProc(this->m_hWnd, WM_IME_COMPOSITION, wParam, lParam);
}

//5. �ϼ��� �ѱ��� �Է¹޾��� ��
LRESULT NotepadForm::OnImeChar(WPARAM wParam, LPARAM lParam)
{
	//5.1 glyphCreator�� �����Ѵ�.
	GlyphCreator glyphCreator;
	WORD word = LOWORD(wParam);
	char koreanLetter[3];
	koreanLetter[0] = HIBYTE(word);
	koreanLetter[1] = LOBYTE(word);
	koreanLetter[2] = '\0';
	//5.2 doubleByteLetter�� �����Ѵ�.
	Glyph* doubleByteLetter = glyphCreator.Create((char*)koreanLetter);
	//5.3 ������ �������̴� �ѱ��� �����.
	this->current->Remove(this->current->GetLength() - 1);
	//5.4 �����ٿ� �ϼ��� �ѱ��� �߰��Ѵ�.
	this->current->Add(doubleByteLetter);
	//5.5 IsComposing�� false�� �ٲ۴�.
	this->IsComposing = false;

	//5.6 �����Ѵ�.
	Invalidate(TRUE);

	return 0;
}

//�ѱ� ���� ������ �˸�
LRESULT NotepadForm::OnStartCompostion(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

#if 0
//6. �ѱ� ���������� �ٸ� �����쿡�� �Ⱥ��̰� �ϱ� ���� ��ġ
LRESULT NotepadForm::OnSetContext(WPARAM wParam, LPARAM lParam)
{
	lParam &= ~(ISC_SHOWUICOMPOSITIONWINDOW | ISC_SHOWUIALLCANDIDATEWINDOW);

	return 0;
}
#endif
