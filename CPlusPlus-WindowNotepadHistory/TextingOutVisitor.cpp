#include "TextingOutVisitor.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "TextExtent.h"
#include "Font.h"

//����Ʈ������
TextingOutVisitor::TextingOutVisitor(NotepadForm* notepadForm, CPaintDC* dc,
	Long glyphXPos, Long glyphYPos)
	:GlyphVisitor(notepadForm, dc, glyphXPos, glyphYPos)
{

}

//���������
TextingOutVisitor::TextingOutVisitor(const TextingOutVisitor& source)
	:GlyphVisitor(source)
{

}

//ġȯ������
TextingOutVisitor& TextingOutVisitor::operator=(const TextingOutVisitor& source)
{
	GlyphVisitor::operator=(source);

	return *this;
}

//Note
void TextingOutVisitor::VisitNote(Glyph* note)
{
	//1. ���õ� ������ ���Ѵ�.
	Long startingRowPos = 0;
	Long startingLetterPos = 0;
	Long endingRowPos = 0;
	Long endingLetterPos = 0;
	note->CalculateSelectedRange(&startingRowPos,
		&startingLetterPos, &endingRowPos, &endingLetterPos);
	//2. �޸��忡�� ������ �ȵ� texts�� ����Ѵ�.
	//2.1 �޸����� ù �ٺ��� ���õ� ���۵Ǵ� �ٱ��� �ݺ��Ѵ�.
	Glyph* row = 0;
	Long rowIndex = 0;
	while (rowIndex <= startingRowPos)
	{
		//2.1.1 ���� ���� ���Ѵ�.
		row = note->GetAt(rowIndex);
		//2.1.2 ��µ� �κ��� ��ġ�� ������Ʈ���ش�.
		this->glyphXPos = 0;
		this->glyphYPos = rowIndex;
		//2.1.3 �ٿ��� ������ �ȵ� texts�� ����Ѵ�.
		row->Accept(this);
		//2.1.4 ���� ��ġ�� ������Ų��.
		rowIndex++;
	}
	//2.2 ���� ��ġ�� ������ ������ �ٷ� �̵���Ų��.
	rowIndex = endingRowPos;
	//2.3 ������ ������ �ٺ��� �޸����� �������ٱ��� �ݺ��Ѵ�.
	while (rowIndex < note->GetLength())
	{
		//2.3.1 ���� ���� ���Ѵ�.
		row = note->GetAt(rowIndex);
		//2.3.2 ��µ� �κ��� ��ġ�� ������Ʈ���ش�.
		this->glyphXPos = 0;
		this->glyphYPos = rowIndex;
		//2.3.3  �ٿ��� ������ �ȵ� texts�� ����Ѵ�.
		row->Accept(this);
		//2.3.4 ���� ��ġ�� ������Ų��.
		rowIndex++;
	}
}

//Row
void TextingOutVisitor::VisitRow(Glyph* row)
{
	//1. �ٿ��� ������ �ȵ� texts�� ����Ѵ�.
	//1.1 ���� ù���ں��� �����ؼ� ���� ���ڰ������� �������� �ݺ��Ѵ�.
	Glyph* letter = 0;//���� �ּ�
	string content;//���� ������ ���� ����
	Long letterWidth = 0;//���ڳʺ�
	Long letterIndex = 0;//�ٿ��� ���� ��ġ
	while (letterIndex < row->GetLength())
	{
		//1.1.1 ���� ���ڸ� ���Ѵ�.
		letter = row->GetAt(letterIndex);
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
void TextingOutVisitor::VisitDummyRow(Glyph* dummyRow)
{
	//1. �ٿ��� ������ �ȵ� texts�� ����Ѵ�.
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
void TextingOutVisitor::VisitSingleByteLetter(Glyph* singleByteLetter)
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
	//7. ���� ���ڰ� ������ �ȵǾ�������
	if (singleByteLetter->IsSelected() == false)
	{
		//7.1 ������ ������� �����Ѵ�.
		this->dc->SetBkColor(RGB(255, 255, 255));
		//7.2 �۲������� �޾� ���ڻ��� ���Ѵ�.
		this->dc->SetTextColor(this->notepadForm->font.GetColor());
		//7.3 ���ڸ� ����Ѵ�.
		this->dc->TextOut(this->glyphXPos - currentXPos, this->glyphYPos * text.tmHeight
			- currentYPos, content);

	}

	//font�� ��Ʈ�����ȭ���ڿ��� ����Ǿ����� ���� font�� ������ ���� ����� font�� ������ �� ����.
	this->dc->SelectObject(oldFont);
	font.DeleteObject();
}

//DobuleByteLetter
void TextingOutVisitor::VisitDoubleByteLetter(Glyph* doubleByteLetter)
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
	//7. ���� ���ڰ� ������ �ȵǾ�������
	if (doubleByteLetter->IsSelected() == false)
	{
		//7.1 ������ ������� �����Ѵ�.
		this->dc->SetBkColor(RGB(255, 255, 255));
		//7.2 �۲������� �޾� ���ڻ��� ���Ѵ�.
		this->dc->SetTextColor(this->notepadForm->font.GetColor());
		//7.3 ���ڸ� ����Ѵ�.
		this->dc->TextOut(this->glyphXPos - currentXPos, this->glyphYPos * text.tmHeight
			- currentYPos, content);

	}

	//font�� ��Ʈ�����ȭ���ڿ��� ����Ǿ����� ���� font�� ������ ���� ����� font�� ������ �� ����.
	this->dc->SelectObject(oldFont);
	font.DeleteObject();
}

//�Ҹ���
TextingOutVisitor::~TextingOutVisitor()
{

}