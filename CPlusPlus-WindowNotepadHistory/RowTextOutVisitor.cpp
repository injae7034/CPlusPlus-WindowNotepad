#include "RowTextOutVisitor.h"
#include "NotepadForm.h"
#include "Glyph.h"

//����Ʈ������
RowTextOutVisitor::RowTextOutVisitor(NotepadForm* notepadForm)
	:TextOutVisitor(notepadForm)
{

}

void RowTextOutVisitor::VisitNoteTextOut(Glyph *note)
{
	//1. CPaintDC�� �����Ѵ�.
	CPaintDC dc(notepadForm);
	//2. �ؽ�Ʈ�� ������ ����.
	dc.SetTextColor(notepadForm->font.GetColor());
	//3. ������ ���ؼ����� ����.
	dc.SetTextAlign(TA_LEFT);
	//4. CFont�� �����Ѵ�.
	CFont font;
	//5. �۾�ũ��� �۾�ü�� ���ϴ�.
	font.CreateFontIndirect(&notepadForm->font.GetLogFont());
	//6. ��Ʈ�� dc�� �����Ѵ�.
	HFONT oldFont;
	oldFont = (HFONT)dc.SelectObject(font);
	//7. TEXTMETRIC�� �����Ѵ�.
	TEXTMETRIC text;
	//8. �۲��� ������ ��´�.
	dc.GetTextMetrics(&text);
	//9. note�� ����� ���ڵ��� ����Ѵ�.
	Long i = 0;
	Long currentXPos;
	Long currentYPos;
	CString content;
	//10. �ٴ����� �ݺ������� ���ؼ� ���� ������ �ٰ�����ŭ ����ϵ��� ��.
	while (i < notepadForm->note->GetLength())
	{
		//10.1 ���� ���� ���ڵ��� ���Ѵ�.
		content = CString(notepadForm->note->GetAt(i)->GetContent().c_str());
		//10.2 ��ũ���� ��ġ�� ���Ѵ�.
		currentXPos = notepadForm->GetScrollPos(SB_HORZ);
		currentYPos = notepadForm->GetScrollPos(SB_VERT);
		//10.3 �ؽ�Ʈ ���� ��ġ���� ó������ (0,0)���� �����ϰ� �ι�°���� (0, ������ճ���)���� ������.
		// �ؽ�Ʈ ������ġ�� �����Ǿ�� ȭ�鸸 �̵��ϹǷ� �̵��Ѹ�ŭ �������!
		//�׷� ���� ȭ���� ó�� �������� �����Ǿ� �ִµ� -�������ν� ȭ���� �����̴� ��ó�� ����.
		dc.TextOut(0 - currentXPos, i * text.tmHeight - currentYPos, content);
		//dc.TextOut(0, i * text.tmHeight, content);
		//this->Notify(); ���⼭ Notify ���ָ� �ȵ�! ĳ���� ��� ���Եǰ� �Ȼ����.
		i++;
	}
	dc.SelectObject(oldFont);
	//font�� ��Ʈ�����ȭ���ڿ��� ����Ǿ����� ���� font�� ������ ���� ����� font�� ������ �� ����. 
	font.DeleteObject();
}

void RowTextOutVisitor::VisitRowTextOut(Glyph* row)
{

}

void RowTextOutVisitor::VisitLetterTextOut(Glyph* letter)
{

}

//�Ҹ���
RowTextOutVisitor::~RowTextOutVisitor()
{

}