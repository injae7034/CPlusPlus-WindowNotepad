#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "Glyph.h"

BEGIN_MESSAGE_MAP(NotepadForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_WM_PAINT()
END_MESSAGE_MAP()

//NotepadForm������
NotepadForm::NotepadForm()
	:CFrameWnd()
{
	this->note = NULL;//NULL�� �ʱ�ȭ��Ŵ.
	this->current = NULL;//NULL�� �ʱ�ȭ��Ŵ.
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