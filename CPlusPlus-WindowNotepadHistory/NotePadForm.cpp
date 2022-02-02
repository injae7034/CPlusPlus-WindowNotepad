#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "Glyph.h"

BEGIN_MESSAGE_MAP(NotepadForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

//NotepadForm������
NotepadForm::NotepadForm()
	:CFrameWnd()
{
	this->note = NULL;//NULL�� �ʱ�ȭ��Ŵ.
}

//NotepadForm�Ҹ���
NotepadForm::~NotepadForm()
{

}

//1. �����찡 ������ ��
int NotepadForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//1.1 glyphCreator�� �����.
	GlyphCreator glyphCreator;
	//1.2 �޸����� �����.
	this->note = glyphCreator.Create((char*)"\0");
	//1.3 ù��° ���� �����.
	Glyph* row = glyphCreator.Create((char*)"\n");
	Long rowIndex;
	Long letterIndex;
	rowIndex = this->note->Add(row);
	//1.4 ù��° �ٿ� 'A'�� �Է��Ѵ�.
	Glyph* letter = glyphCreator.Create((char*)"A");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.5 ù��° �ٿ� 'p'�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)"p");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.6 ù��° �ٿ� 'p'�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)"p");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.7 ù��° �ٿ� 'l'�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)"l");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.8 ù��° �ٿ� 'e�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)"e");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.9 ù��° �ٿ� "��"�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)"��");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.10 �ι�° ���� �����.
	row = glyphCreator.Create((char*)"\n");
	rowIndex = this->note->Add(row);
	//1.11 �ι�° �ٿ� "��"�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)"��");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.12 �ι�° �ٿ� "��"�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)"��");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.13 �ι�° �ٿ� "��"�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)"��");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.14 �ι�° �ٿ� "."�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)".");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.15 ����° ���� �����.
	row = glyphCreator.Create((char*)"\r");
	rowIndex = note->Add(row);
	//1.16 ����° �ٿ� "��"�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)"��");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.17 ����° �ٿ� "��"�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)"��");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.18 ����° �ٿ� "��"�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)"��");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.19 �׹�° ���� �����.
	row = glyphCreator.Create((char*)"\r");
	rowIndex = note->Add(row);
	//1.20 �׹�° �ٿ� "��"�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)"��");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.21 �׹�° �ٿ� "��"�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)"��");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.22 �׹�° �ٿ� "��"�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)"��");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.23 �׹�° �ٿ� "."�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)".");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);

	return 0;
}

//2. �޸��忡 �ؽ�Ʈ�� ����� ��
void NotepadForm::OnPaint()
{
	//2.1 CPaintDC�� �����Ѵ�.
	CPaintDC dc(this);
	//2.2 �ؽ�Ʈ�� ����� �����ϰ���.
	dc.SetBkMode(TRANSPARENT);
	//2.3 �ؽ�Ʈ�� ������ ����.
	dc.SetTextColor(RGB(0, 0, 0));
	//2.4 ������ ���ؼ����� ����.
	dc.SetTextAlign(TA_LEFT);
	//2.5 CFont�� �����Ѵ�.
	CFont font;
	//2.6 �۾�ũ��� �۾�ü�� ���ϴ�.
	font.CreatePointFont(500, _T("�����ٸ���"));
	//2.7 ��Ʈ�� dc�� �����Ѵ�.
	dc.SelectObject(font);
	//2.8 note�� ����� ���ڵ��� ����Ѵ�.
	Long i = 0;
	CString content;
	//�ٴ����� �ݺ������� ���ؼ� ���� ������ �ٰ�����ŭ ����ϵ��� ��.
	while (i < this->note->GetLength())
	{
		content = CString(this->note->GetAt(i)->GetContent().c_str());
		dc.TextOut(0, i * 150, content);
		i++;
		
	}
	//��Ʈ�� �״�� ����ϸ� ���� ������ �ȵǰ� ���ٿ� ������ �ٳ���.
	content = CString(this->note->GetContent().c_str());
	dc.TextOut(0, i * 150, content);

#if 0
	//2.8 Note�� ����� Content�� �ҷ��´�.
	CString content = CString(this->note->GetContent().c_str());
	//2.9 Content�� ����Ѵ�.
	dc.TextOut(0, 0, _T(this->note->GetContent().c_str()));
#endif
}