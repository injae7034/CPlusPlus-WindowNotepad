#include "SelectingVisitor.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "TextExtent.h"
#include "Font.h"

//����Ʈ������
SelectingVisitor::SelectingVisitor(NotepadForm* notepadForm, CDC* dc,
	Long glyphXPos, Long glyphYPos)
	:GlyphVisitor(notepadForm, dc, glyphXPos, glyphYPos)
{
	
}

//���������
SelectingVisitor::SelectingVisitor(const SelectingVisitor& source)
	:GlyphVisitor(source)
{

}

//ġȯ������
SelectingVisitor& SelectingVisitor::operator=(const SelectingVisitor& source)
{
	GlyphVisitor::operator=(source);

	return *this;
}

//Note
void SelectingVisitor::VisitNote(Glyph* note)
{
	//1. ���õ� ������ ���Ѵ�.
	Long startingRowPos = 0;
	Long startingLetterPos = 0;
	Long endingRowPos = 0;
	Long endingLetterPos = 0;
	note->CalculateSelectedRange(&startingRowPos,
		&startingLetterPos, &endingRowPos, &endingLetterPos);
	//2. �޸��忡�� ���õ� texts�� ����Ѵ�.
	//2.1 ������ ���۵Ǵ� �ٺ��� �����ؼ� ������ ������ �ٱ��� �ݺ��Ѵ�.
	Glyph* row = 0;
	Long rowIndex = startingRowPos;
	while (rowIndex <= endingRowPos)
	{
		//2.1.1 ���� ���� ���Ѵ�.
		row = note->GetAt(rowIndex);
		//2.1.2 ��µ� �κ��� ��ġ�� ������Ʈ���ش�.
		this->glyphXPos = 0;
		this->glyphYPos = rowIndex;
		//2.1.3 �ٿ��� ������ �� texts�� ����Ѵ�.
		row->Accept(this);
		//2.1.4 ���� ��ġ�� ������Ų��.
		rowIndex++;
	}
}

//Row
void SelectingVisitor::VisitRow(Glyph* row)
{
	//1. �ٿ��� ���õ� �κ��� ����Ѵ�.
	//1.1 ���� ù���ں��� �����ؼ� ���� ���ڰ������� �������� �ݺ��Ѵ�.
	Glyph* letter = 0;//���� �ּ�
	string content;//���� ������ ���� ����
	Long letterWidth = 0;//���ڳʺ�
	Long letterIndex = 0;//�ٿ��� ���� ��ġ
	while (letterIndex < row->GetLength())
	{
		//1.1.1 ���� ���ڸ� ���Ѵ�.
		letter= row->GetAt(letterIndex);
		//1.1.2 ���� ������ ������ ���Ѵ�.
		content = CString(letter->GetContent().c_str());
		//1.1.3 ���� ������ �ʺ� ������� �ؼ� �޸��忡�� ������ X��ǥ ��ġ�� ������Ʈ���ش�.
		this->glyphXPos += letterWidth;
		letterWidth = this->notepadForm->textExtent->GetTextWidth((string)content);
		//1.1.4 ���� ���ڸ� ����Ѵ�.
		letter->Accept(this);
		//1.1.5 ������ ��ġ�� ������Ų��.
		letterIndex++;
	}
}

//DummyRow
void SelectingVisitor::VisitDummyRow(Glyph* dummyRow)
{
	//1. �ٿ��� ���õ� �κ��� ����Ѵ�.
	//1.1 ���� ù���ں��� �����ؼ� ���� ���ڰ������� �������� �ݺ��Ѵ�.
	Glyph* letter = 0;//���� �ּ�
	string content;//���� ������ ���� ����
	Long letterWidth = 0;//���ڳʺ�
	Long letterIndex = 0;//�ٿ��� ���� ��ġ
	while (letterIndex < dummyRow->GetLength())
	{
		//1.1.1 ���� ���ڸ� ���Ѵ�.
		letter = dummyRow->GetAt(letterIndex);
		//1.1.2 ���� ������ ������ ���Ѵ�.
		content = CString(letter->GetContent().c_str());
		//1.1.3 ���� ������ �ʺ� ������� �ؼ� �޸��忡�� ������ X��ǥ ��ġ�� ������Ʈ���ش�.
		this->glyphXPos += letterWidth;
		letterWidth = this->notepadForm->textExtent->GetTextWidth((string)content);
		//1.1.4 ���� ���ڸ� ����Ѵ�.
		letter->Accept(this);
		//1.1.5 ������ ��ġ�� ������Ų��.
		letterIndex++;
	}
}

//SingleByteLetter
void SelectingVisitor::VisitSingleByteLetter(Glyph* singleByteLetter)
{
	//1. �۾�ũ��� �۾�ü�� ���ϴ�.
	CFont font;
	font.CreateFontIndirect(&this->notepadForm->font.GetLogFont());
	//2. ��Ʈ�� dc�� �����Ѵ�.
	HFONT oldFont;
	oldFont = (HFONT)this->dc->SelectObject(font);
	//3. dc�� ������ �۲��� ������ ��´�.
	TEXTMETRIC text;
	this->dc->GetTextMetrics(&text);
	//4. ��ũ�� ������ ���Ѵ�.
	Long currentXPos = this->notepadForm->GetScrollPos(SB_HORZ);
	Long currentYPos = this->notepadForm->GetScrollPos(SB_VERT);
	//5. ������ ������ ���Ѵ�.
	CString content = CString(singleByteLetter->GetContent().c_str());
	//6. ���࿡ ���ڰ� �ǹ����̸� ������ ���� 8���� �ٲ��ش�.
	if (content == "\t")
	{
		content = "        ";
	}
	//7. ���� ���ڰ� ������ �Ǿ�������
	if (singleByteLetter->IsSelected() == true)
	{
		//4. ������ û������ �����Ѵ�.
		this->dc->SetBkColor(GetSysColor(COLOR_HIGHLIGHT));//red, green, blue ���� ���� 
		//5. ���ڻ��� ������� �����Ѵ�.
		this->dc->SetTextColor(RGB(255, 255, 255));
		//���ڸ� ����Ѵ�.
		this->dc->TextOut(this->glyphXPos - currentXPos, this->glyphYPos * text.tmHeight
			- currentYPos, content);
		
	}
	
	//font�� ��Ʈ�����ȭ���ڿ��� ����Ǿ����� ���� font�� ������ ���� ����� font�� ������ �� ����.
	this->dc->SelectObject(oldFont);
	font.DeleteObject();
}

//DobuleByteLetter
void SelectingVisitor::VisitDoubleByteLetter(Glyph* doubleByteLetter)
{
	//1. �۾�ũ��� �۾�ü�� ���ϴ�.
	CFont font;
	font.CreateFontIndirect(&this->notepadForm->font.GetLogFont());
	//2. ��Ʈ�� dc�� �����Ѵ�.
	HFONT oldFont;
	oldFont = (HFONT)this->dc->SelectObject(font);
	//3. dc�� ������ �۲��� ������ ��´�.
	TEXTMETRIC text;
	this->dc->GetTextMetrics(&text);
	//4. ��ũ�� ������ ���Ѵ�.
	Long currentXPos = this->notepadForm->GetScrollPos(SB_HORZ);
	Long currentYPos = this->notepadForm->GetScrollPos(SB_VERT);
	//5. ������ ������ ���Ѵ�.
	CString content = CString(doubleByteLetter->GetContent().c_str());
	//6. ���࿡ ���ڰ� �ǹ����̸� ������ ���� 8���� �ٲ��ش�.
	if (content == "\t")
	{
		content = "        ";
	}
	//7. ���� ���ڰ� ������ �Ǿ�������
	if (doubleByteLetter->IsSelected() == true)
	{
		//4. ������ û������ �����Ѵ�.
		this->dc->SetBkColor(GetSysColor(COLOR_HIGHLIGHT));//red, green, blue ���� ���� 
		//5. ���ڻ��� ������� �����Ѵ�.
		this->dc->SetTextColor(RGB(255, 255, 255));
		//���ڸ� ����Ѵ�.
		this->dc->TextOut(this->glyphXPos - currentXPos, this->glyphYPos * text.tmHeight
			- currentYPos, content);

	}

	//font�� ��Ʈ�����ȭ���ڿ��� ����Ǿ����� ���� font�� ������ ���� ����� font�� ������ �� ����.
	this->dc->SelectObject(oldFont);
	font.DeleteObject();
}


//�Ҹ���
SelectingVisitor::~SelectingVisitor()
{

}