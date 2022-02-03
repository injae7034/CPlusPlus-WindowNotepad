#include "LetterTextOutVisitor.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "TextExtent.h"

//����Ʈ������
LetterTextOutVisitor::LetterTextOutVisitor(NotepadForm* notepadForm)
	:TextOutVisitor(notepadForm)
{

}

void LetterTextOutVisitor::VisitNoteTextOut(Glyph* note)
{
	//1. CPaintDC�� �����Ѵ�.
	CPaintDC dc(notepadForm);
	//2. ������ ���ؼ����� ����.
	dc.SetTextAlign(TA_LEFT);
	//3. CFont�� �����Ѵ�.
	CFont font;
	//4. �۾�ũ��� �۾�ü�� ���ϴ�.
	font.CreateFontIndirect(&notepadForm->font.GetLogFont());
	//5. ��Ʈ�� dc�� �����Ѵ�.
	HFONT oldFont;
	oldFont = (HFONT)dc.SelectObject(font);
	//6. TEXTMETRIC�� �����Ѵ�.
	TEXTMETRIC text;
	//7. �۲��� ������ ��´�.
	dc.GetTextMetrics(&text);
	//8. note�� ����� ���ڵ��� ����Ѵ�.
	Long rowIndex = 0;
	Long currentXPos;
	Long currentYPos;
	CString content;

	Long currentWidth = 0;
	Long letterWidth = 0;
	Long letterCount = 0;
	Long letterIndex = 0;
	Glyph* row = 0;
	Glyph* letter = 0;

	//9. �ٴ����� �ݺ������� ���ؼ� ���� ������ �ٰ�����ŭ ����ϵ��� ��.
	while (rowIndex < notepadForm->note->GetLength())
	{
		//9.1 ���� ���� ���Ѵ�.
		row = notepadForm->note->GetAt(rowIndex);
		//9.2 ��ũ���� ��ġ�� ���Ѵ�.
		currentXPos = notepadForm->GetScrollPos(SB_HORZ);
		currentYPos = notepadForm->GetScrollPos(SB_VERT);
		//9.3 �������� ù ���ڸ� ���Ѵ�.
		letterIndex = 0;
		currentWidth = 0;
		letterWidth = 0;
		//9.4 �������� ������ ���Ѵ�.
		letterCount = row->GetLength();
		//9.5 ������� ��ġ�� ���� ���ڰ������� �������� �ݺ��Ѵ�.
		while (letterIndex < letterCount)
		{
			//9.5.1 ���� ���ڸ� ���Ѵ�.
			letter = row->GetAt(letterIndex);
			//9.5.2 ���� ������ ������ ���Ѵ�.
			content = CString(letter->GetContent().c_str());
			//9.5.3 ������ �ʺ� ���Ѵ�.
			//ó���� 0���� �����ؾ��ϱ� ������ currentWidth += letterWidth�� 
			//letterWidth = this->textExtent->GetTextWidth((string)content) ���� �տ� �־����.
			//�׷��� ���� ���� �ʺ� ���ؼ� ���� ������ ���������� ����ָ� ��
			//ù������ �������� 0�̰� ���� ������ �������� ù������ �ʺ��̴�!!!
			currentWidth += letterWidth;
			letterWidth = notepadForm->textExtent->GetTextWidth((string)content);
			//���࿡ ���ڰ� �ǹ����̸� ������ ���� 8���� �ٲ��ش�.
			if (content == "\t")
			{
				content = "        ";
			}
			//9.5.3. ���� ���ڰ� ������ �ȵǾ�������
			if (letter->IsSelected() == false)
			{
				//9.5.3.1 ���ڸ� ȭ�鿡 ����Ѵ�.
				dc.SetBkColor(RGB(255, 255, 255));
				//�ؽ�Ʈ�� ������ ����. �̷����ؾ� ���߿� �۲û��ڿ��� ������ �ٲٸ� �ݿ��� �� ����.
				dc.SetTextColor(notepadForm->font.GetColor());
				//dc.SetTextColor(RGB(0, 0, 0));//�̷����ϸ� ���߿� �۲ÿ��� ���ڻ��� �ٲܼ�����
				dc.TextOut(currentWidth - currentXPos, rowIndex * text.tmHeight
					- currentYPos, content);
			}
			//9.5.4 ���� ���ڰ� ������ �Ǿ�������
			else
			{
				//9.5.4.1
				dc.SetBkColor(GetSysColor(COLOR_HIGHLIGHT));//red, green, blue ���� ���� 
				dc.SetTextColor(RGB(255, 255, 255));//�̷����ϸ� ���߿� �۲ÿ��� ���ڻ��� �ٲܼ� ����
				dc.TextOut(currentWidth - currentXPos, rowIndex * text.tmHeight
					- currentYPos, content);
			}
			letterIndex++;
		}
		//9.6 �ؽ�Ʈ ������ġ�� �����Ǿ�� ȭ�鸸 �̵��ϹǷ� �̵��Ѹ�ŭ �������!
		//�׷� ���� ȭ���� ó�� �������� �����Ǿ� �ִµ� -�������ν� ȭ���� �����̴� ��ó�� ����.
		//dc.TextOut(0 - currentXPos, rowIndex * text.tmHeight - currentYPos, content);
		rowIndex++;
	}
	dc.SelectObject(oldFont);
	//font�� ��Ʈ�����ȭ���ڿ��� ����Ǿ����� ���� font�� ������ ���� ����� font�� ������ �� ����. 
	font.DeleteObject();
}

void LetterTextOutVisitor::VisitRowTextOut(Glyph* row)
{

}

void LetterTextOutVisitor::VisitLetterTextOut(Glyph* letter)
{

}

//�Ҹ���
LetterTextOutVisitor::~LetterTextOutVisitor()
{

}