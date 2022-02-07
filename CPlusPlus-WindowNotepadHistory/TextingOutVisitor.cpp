#include "TextingOutVisitor.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "TextExtent.h"
#include "Font.h"
#include "PrintInformation.h"

//����Ʈ������
TextingOutVisitor::TextingOutVisitor(NotepadForm* notepadForm, CDC* dc,
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
	Glyph* row = 0;
	Long rowIndex = 0;
	//1. dc�� ����Ʈ ���� dc�� �ƴϸ�
	if (this->dc->IsPrinting() == 0)
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
	//2. dc�� ����Ʈ ���� dc�� �ƴϸ�
	else
	{
		Glyph* printNote = this->notepadForm->printInformation->GetPrintNote();
		CDC* printDC = this->notepadForm->printInformation->GetPrintDC();
		Long pageRowCount = this->notepadForm->printInformation->GetPageRowCount();
		Long i = 0;
		while (rowIndex < printNote->GetLength())
		{
			i = 0;
			printDC->StartPage();
			while (i < pageRowCount && rowIndex < printNote->GetLength())
			{
				//2.1.1 ���� ���� ���Ѵ�.
				row = note->GetAt(rowIndex);
				//2.1.2 ��µ� �κ��� ��ġ�� ������Ʈ���ش�.
				this->glyphXPos = 0;
				this->glyphYPos = i;
				//2.1.3 �ٿ��� ������ �ȵ� texts�� ����Ѵ�.
				row->Accept(this);
				//2.1.4 ���� ��ġ�� ������Ų��.
				rowIndex++;
				i++;
			}
			printDC->EndPage();
		}
	}
}

//Row
void TextingOutVisitor::VisitRow(Glyph* row)
{	
	Glyph* letter = 0;//���� �ּ�
	string content;//���� ������ ���� ����
	Long letterWidth = 0;//���ڳʺ�
	Long letterIndex = 0;//�ٿ��� ���� ��ġ
	//1. dc�� ����Ʈ ���� dc�� �ƴϸ�
	if (this->dc->IsPrinting() == 0)
	{
		//1.1 ���� ù���ں��� �����ؼ� ���� ���ڰ������� �������� �ݺ��Ѵ�.
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
	//2. dc�� ����Ʈ ���� dc�̸�
	else
	{
		//1. �۾�ũ��� �۾�ü�� ���ϴ�.
		CFont font;
		//2. ��Ʈ�� dc�� �����Ѵ�.
		HFONT oldFont;
		font.CreateFontIndirect(&this->notepadForm->printInformation->GetPrintLogFont());
		oldFont = (HFONT)this->dc->SelectObject(font);
		//3. dc�� ������ �۲��� ������ ��´�.
		TEXTMETRIC text;
		this->dc->GetTextMetrics(&text);
		while (letterIndex < row->GetLength())
		{
			//1.1.1 ���� ���ڸ� ���Ѵ�.
			letter = row->GetAt(letterIndex);
			//1.1.2 ���� ������ ������ ���Ѵ�.
			content = CString(letter->GetContent().c_str());
			//1.1.3 ���� ������ �ʺ� ������� �ؼ� �޸��忡�� ������ X��ǥ ��ġ�� ������Ʈ���ش�.
			this->glyphXPos += letterWidth;
			letterWidth = this->dc->GetTextExtent(content.c_str()).cx;
			//1.1.4 ���� ���ڸ� ����Ѵ�.
			letter->Accept(this);
			//1.1.5 ������ ��ġ�� ������Ų��.
			letterIndex++;
		}
	}
}

//DummyRow
void TextingOutVisitor::VisitDummyRow(Glyph* dummyRow)
{
	Glyph* letter = 0;//���� �ּ�
	string content;//���� ������ ���� ����
	Long letterWidth = 0;//���ڳʺ�
	Long letterIndex = 0;//�ٿ��� ���� ��ġ
	//1. dc�� ����Ʈ ���� dc�� �ƴϸ�
	if (this->dc->IsPrinting() == 0)
	{
		//1.1 ���� ù���ں��� �����ؼ� ���� ���ڰ������� �������� �ݺ��Ѵ�.
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
	//2. dc�� ����Ʈ ���� dc�̸�
	else
	{
		//1. �۾�ũ��� �۾�ü�� ���ϴ�.
		CFont font;
		//2. ��Ʈ�� dc�� �����Ѵ�.
		HFONT oldFont;
		font.CreateFontIndirect(&this->notepadForm->printInformation->GetPrintLogFont());
		oldFont = (HFONT)this->dc->SelectObject(font);
		//3. dc�� ������ �۲��� ������ ��´�.
		TEXTMETRIC text;
		this->dc->GetTextMetrics(&text);
		while (letterIndex < dummyRow->GetLength())
		{
			//1.1.1 ���� ���ڸ� ���Ѵ�.
			letter = dummyRow->GetAt(letterIndex);
			//1.1.2 ���� ������ ������ ���Ѵ�.
			content = CString(letter->GetContent().c_str());
			//1.1.3 ���� ������ �ʺ� ������� �ؼ� �޸��忡�� ������ X��ǥ ��ġ�� ������Ʈ���ش�.
			this->glyphXPos += letterWidth;
			letterWidth = this->dc->GetTextExtent(content.c_str()).cx;
			//1.1.4 ���� ���ڸ� ����Ѵ�.
			letter->Accept(this);
			//1.1.5 ������ ��ġ�� ������Ų��.
			letterIndex++;
		}
	}
}

//SingleByteLetter
void TextingOutVisitor::VisitSingleByteLetter(Glyph* singleByteLetter)
{
	CFont font;
	HFONT oldFont;
	Long currentXPos = 0;
	Long currentYPos = 0;
	//1. dc�� ����Ʈ���� dc�� �ƴϸ�
	if (this->dc->IsPrinting() == 0)
	{
		//1.1 �޸����� font������ ���� font�� �����Ѵ�.
		font.CreateFontIndirect(&this->notepadForm->font.GetLogFont());
		oldFont = (HFONT)this->dc->SelectObject(font);
		//1.2 ��ũ���� ������ ���Ѵ�.
		currentXPos = this->notepadForm->GetScrollPos(SB_HORZ);
		currentYPos = this->notepadForm->GetScrollPos(SB_VERT);
	}
	//2. dc�� ����Ʈ���� dc�̸�
	else
	{
		//2.1 ����Ʈ ������ ������ �����ش�.
		font.CreateFontIndirect(&this->notepadForm->printInformation->GetPrintLogFont());
		oldFont = (HFONT)this->dc->SelectObject(font);
	}
	//3. �۲� ������ ���Ѵ�.
	TEXTMETRIC text;
	this->dc->GetTextMetrics(&text);
	//4. ������ ������ ���Ѵ�.
	CString content = CString(singleByteLetter->GetContent().c_str());
	//5. ���࿡ ���ڰ� �ǹ����̸� ������ ���� 8���� �ٲ��ش�.
	if (content == "\t")
	{
		content = "        ";
	}
	//6. ���� ���ڰ� ������ �ȵǾ�������
	if (singleByteLetter->IsSelected() == false)
	{
		//6.1 ������ ������� �����Ѵ�.
		this->dc->SetBkColor(RGB(255, 255, 255));
		//6.2 �۲������� �޾� ���ڻ��� ���Ѵ�.
		this->dc->SetTextColor(this->notepadForm->font.GetColor());
		//6.3 ���ڸ� ����Ѵ�.
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
	CFont font;
	HFONT oldFont;
	Long currentXPos = 0;
	Long currentYPos = 0;
	//1. dc�� ����Ʈ���� dc�� �ƴϸ�
	if (this->dc->IsPrinting() == 0)
	{
		//1.1 �޸����� font������ ���� font�� �����Ѵ�.
		font.CreateFontIndirect(&this->notepadForm->font.GetLogFont());
		oldFont = (HFONT)this->dc->SelectObject(font);
		//1.2 ��ũ���� ������ ���Ѵ�.
		currentXPos = this->notepadForm->GetScrollPos(SB_HORZ);
		currentYPos = this->notepadForm->GetScrollPos(SB_VERT);
	}
	//2. dc�� ����Ʈ���� dc�̸�
	else
	{
		//2.1 ����Ʈ ������ ������ �����ش�.
		font.CreateFontIndirect(&this->notepadForm->printInformation->GetPrintLogFont());
		oldFont = (HFONT)this->dc->SelectObject(font);
	}
	//3. �۲� ������ ���Ѵ�.
	TEXTMETRIC text;
	this->dc->GetTextMetrics(&text);
	//4. ������ ������ ���Ѵ�.
	CString content = CString(doubleByteLetter->GetContent().c_str());
	//5. ���࿡ ���ڰ� �ǹ����̸� ������ ���� 8���� �ٲ��ش�.
	if (content == "\t")
	{
		content = "        ";
	}
	//6. ���� ���ڰ� ������ �ȵǾ�������
	if (doubleByteLetter->IsSelected() == false)
	{
		//6.1 ������ ������� �����Ѵ�.
		this->dc->SetBkColor(RGB(255, 255, 255));
		//6.2 �۲������� �޾� ���ڻ��� ���Ѵ�.
		this->dc->SetTextColor(this->notepadForm->font.GetColor());
		//6.3 ���ڸ� ����Ѵ�.
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