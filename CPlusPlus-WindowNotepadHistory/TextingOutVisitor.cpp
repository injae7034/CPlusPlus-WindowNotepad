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
	//1. ������ �ȵ� ������ ���Ѵ�.
	Long startingRowPos = 0;
	Long startingLetterPos = 0;
	Long endingRowPos = 0;
	Long endingLetterPos = 0;
	note->CalculateSelectedRange(&startingRowPos,
		&startingLetterPos, &endingRowPos, &endingLetterPos);
	//2. �޸��忡�� ������ �ȵ� �κ��� ����Ѵ�.
	//2.1 ���õ� �κ������� �ٴ����� ����Ѵ�.
	Glyph* row = 0;
	Long rowIndex = 0;
	while (rowIndex < startingRowPos)
	{
		//2.1 ���� ���� ���Ѵ�.
		row = note->GetAt(rowIndex);
		//2.2 ��µ� �κ��� ��ġ�� ������Ʈ���ش�.
		this->glyphXPos = 0;
		this->glyphYPos = rowIndex;
		//2.3 �ٴ����� ���þ��� ����Ѵ�.
		row->Accept(this);
		//2.4 ���� ��ġ�� ������Ų��.
		rowIndex++;
	}
	//2.2 ������ ���۵Ǵ� �ٿ��� ������ �ȵ� ���ڵ��� ���ڴ����� ����Ѵ�.
	rowIndex = startingRowPos;
	Glyph* letter = 0;//�����ּҸ� ���� ����
	string content;//������ ������ ���� ����
	Long letterWidth = 0;//���ڳʺ�
	//2.2.1 ���� ���� ���Ѵ�.
	row = note->GetAt(rowIndex);
	Long letterIndex = 0;
	//2.2.2 ������ ���۵Ǵ� ���� ù���ڰ� ������ ���۵Ǵ� ������ ��ġ���� �������� �ݺ��Ѵ�.
	while (letterIndex < startingLetterPos)
	{
		//2.2.2.1 ���� ���ڸ� ���Ѵ�.
		letter = row->GetAt(letterIndex);
		//2.2.2.2 ���� ������ ������ ���Ѵ�.
		content = CString(letter->GetContent().c_str());
		//2.2.2.3 ���� ������ �ʺ� ������� �ؼ� �޸��忡�� ������ X��ǥ�� ������Ʈ���ش�.
		this->glyphXPos += letterWidth;
		letterWidth = this->notepadForm->textExtent->GetTextWidth((string)content);
		//2.2.2.4 ���� ���� ��ġ�� ������� �ؼ� �޸��忡�� ������ Y��ǥ�� ������Ʈ���ش�.
		this->glyphYPos = rowIndex;
		//2.2.2.5 ���ڴ����� ���þ��� ����Ѵ�.
		letter->Accept(this);
		//2.2.2.6 ������ ��ġ�� ������Ų��.
		letterIndex++;
		
	}
	//2.3 ������ ������ ���� ó�� ���ں��� ������ ������ ���� ������ ���� ������ġ���� �ݺ��Ѵ�.
	rowIndex = endingRowPos;
	letterIndex = 0;//������ġ ����ġ
	letterWidth = 0;//���ڳʺ� ����ġ
	//2.3.1 ���� ���� ���Ѵ�.
	row = note->GetAt(rowIndex);
	//2.3.2 ������ ������ ���� ó��������ġ���� �����ؼ� ������ ������ ���� ������ ������ġ����
	//�۰ų� ���� ���� �ݺ��Ѵ�.
	while (letterIndex <= endingLetterPos)
	{
		//2.3.2.1 ���� ���ڸ� ���Ѵ�.
		letter = row->GetAt(letterIndex);
		//2.3.2.2 ���� ������ ������ ���Ѵ�.
		content = CString(letter->GetContent().c_str());
		//2.3.2.3 ���� ������ �ʺ� ������� �ؼ� �޸��忡�� ������ X��ǥ�� ������Ʈ���ش�.
		this->glyphXPos += letterWidth;
		letterWidth = this->notepadForm->textExtent->GetTextWidth((string)content);
		//2.3.2.4 ���� ���� ��ġ�� ������� �ؼ� �޸��忡�� ������ Y��ǥ�� ������Ʈ���ش�.
		this->glyphYPos = rowIndex;
		//2.3.2.5 ������ ��ġ�� ������Ų��.
		letterIndex++;
	}
	//2.4 ������ ������ ���� ������ ������ ���ڴ������� 
	//���� ���������ڱ��� ������ �ȵ� ���ڵ��� ���ڴ����� ����Ѵ�.
	letterIndex = endingLetterPos + 1;
	//2.4.1 ������ ������ ���� ������ ���� ���ڰ� ������ ������ ���� �������� �������� �ݺ��Ѵ�.
	while (letterIndex < row->GetLength())
	{
		//2.4.1.1 ���� ���ڸ� ���Ѵ�.
		letter = row->GetAt(letterIndex);
		//2.4.1.2 ���� ������ ������ ���Ѵ�.
		content = CString(letter->GetContent().c_str());
		//2.4.1.3 ���� ������ �ʺ� ������� �ؼ� �޸��忡�� ������ X��ǥ�� ������Ʈ���ش�.
		this->glyphXPos += letterWidth;
		letterWidth = this->notepadForm->textExtent->GetTextWidth((string)content);
		//2.4.1.4 ���� ���� ��ġ�� ������� �ؼ� �޸��忡�� ������ Y��ǥ�� ������Ʈ���ش�.
		this->glyphYPos = rowIndex;
		//2.4.1.5 ���ڴ����� ���þ��� ����Ѵ�.
		letter->Accept(this);
		//2.4.1.6 ������ ��ġ�� ������Ų��.
		letterIndex++;
	}
	//2.5 �������ٱ��� �ٴ����� ����Ѵ�.
	while (rowIndex < note->GetLength())
	{
		//2.2.1 ���� ���� ���Ѵ�.
		row = note->GetAt(rowIndex);
		//2.2.2 ��µ� �κ��� ��ġ�� ������Ʈ���ش�.
		this->glyphXPos = 0;
		this->glyphYPos = rowIndex;
		//2.2.3 �ٴ����� ���þ��� ����Ѵ�.
		row->Accept(this);
		//2.2.4 ���� ��ġ�� ������Ų��.
		rowIndex++;
	}
}

//Row
void TextingOutVisitor::VisitRow(Glyph* row)
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
	//4. ������ ������� �����Ѵ�.
	this->dc->SetBkColor(RGB(255, 255, 255));
	//5. �۲��� ������ ��� ���ڻ��� �����Ѵ�.
	this->dc->SetTextColor(this->notepadForm->font.GetColor());
	//6. ��ũ�� ������ ���Ѵ�.
	Long currentXPos = this->notepadForm->GetScrollPos(SB_HORZ);
	Long currentYPos = this->notepadForm->GetScrollPos(SB_VERT);
	//7. ���� ������ ���Ѵ�.
	CString content = CString(row->GetContent().c_str());
	//8. ���� �޸��忡 ����Ѵ�.
	this->dc->TextOut(this->glyphXPos - currentXPos, 
		this->glyphYPos * text.tmHeight - currentYPos, content);
	
	//font�� ��Ʈ�����ȭ���ڿ��� ����Ǿ����� ���� font�� ������ ���� ����� font�� ������ �� ����.
	this->dc->SelectObject(oldFont);
	font.DeleteObject();
}

//DummyRow
void TextingOutVisitor::VisitDummyRow(Glyph* dummyRow)
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
	//4. ������ ������� �����Ѵ�.
	this->dc->SetBkColor(RGB(255, 255, 255));
	//5. �۲��� ������ ��� ���ڻ��� �����Ѵ�.
	this->dc->SetTextColor(this->notepadForm->font.GetColor());
	//6. ��ũ�� ������ ���Ѵ�.
	Long currentXPos = this->notepadForm->GetScrollPos(SB_HORZ);
	Long currentYPos = this->notepadForm->GetScrollPos(SB_VERT);
	//7. ���� ������ ���Ѵ�.
	CString content = CString(dummyRow->GetContent().c_str());
	//8. ���� �޸��忡 ����Ѵ�.
	this->dc->TextOut(this->glyphXPos - currentXPos,
		this->glyphYPos * text.tmHeight - currentYPos, content);

	//font�� ��Ʈ�����ȭ���ڿ��� ����Ǿ����� ���� font�� ������ ���� ����� font�� ������ �� ����.
	this->dc->SelectObject(oldFont);
	font.DeleteObject();
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
	//7. ������ ������� �����Ѵ�.
	this->dc->SetBkColor(RGB(255, 255, 255));
	//8. �۲������� �޾� ���ڻ��� ���Ѵ�.
	this->dc->SetTextColor(this->notepadForm->font.GetColor());
	//9. ���ڴ����� ����Ѵ�.
	this->dc->TextOut(this->glyphXPos - currentXPos, this->glyphYPos * text.tmHeight
		- currentYPos, content);

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
	//7. ������ ������� �����Ѵ�.
	this->dc->SetBkColor(RGB(255, 255, 255));
	//8. �۲������� �޾� ���ڻ��� ���Ѵ�.
	this->dc->SetTextColor(this->notepadForm->font.GetColor());
	//9. ���ڴ����� ����Ѵ�.
	this->dc->TextOut(this->glyphXPos - currentXPos, this->glyphYPos * text.tmHeight
		- currentYPos, content);

	//font�� ��Ʈ�����ȭ���ڿ��� ����Ǿ����� ���� font�� ������ ���� ����� font�� ������ �� ����.
	this->dc->SelectObject(oldFont);
	font.DeleteObject();
}

//�Ҹ���
TextingOutVisitor::~TextingOutVisitor()
{

}