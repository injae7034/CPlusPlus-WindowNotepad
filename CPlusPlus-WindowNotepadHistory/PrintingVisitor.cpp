#include "PrintingVisitor.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "TextExtent.h"
#include "Font.h"
#include "PrintInformation.h"
#include "PageSetUpInformation.h"

//����Ʈ������
PrintingVisitor::PrintingVisitor(NotepadForm* notepadForm, CDC* dc,
	Long glyphXPos, Long glyphYPos)
	:GlyphVisitor(notepadForm, dc, glyphXPos, glyphYPos)
{

}

//���������
PrintingVisitor::PrintingVisitor(const PrintingVisitor& source)
	:GlyphVisitor(source)
{

}

//ġȯ������
PrintingVisitor& PrintingVisitor::operator=(const PrintingVisitor& source)
{
	GlyphVisitor::operator=(source);

	return *this;
}

//Note
void PrintingVisitor::VisitNote(Glyph* note)
{
	//1. ����Ʈ�� ��Ʈ�� ���Ѵ�.
	Glyph* printNote = this->notepadForm->printInformation->GetPrintNote();
	//2. ����Ʈ ���̾�α��� hdc���� printDC�� ���Ѵ�.
	CDC* printDC = CDC::FromHandle(this->notepadForm->printInformation->GetHDC());
	//3. �� �������� �μ��� ���� ���� ���Ѵ�.
	Long pageRowCount = this->notepadForm->printInformation->GetPageRowCount();
	//4. ����Ʈ ������ ������ ���Ѵ�. 
	CRect rect(0, 0, printDC->GetDeviceCaps(HORZRES), printDC->GetDeviceCaps(VERTRES));
	//5. ������ ���� ������ ������
	Glyph* row = 0;
	Long rowIndex = 0;
	Long i = 0;
	Long glyphXPos = 0;
	if (this->notepadForm->pageSetUpInformation != NULL)
	{
		//10.1 ������ �����ϰ� ����Ʈ ������ ������ ���� ��ġ�� �����Ѵ�.
		glyphXPos = this->notepadForm->pageSetUpInformation->GetPrintableRect().left;
		//10.2 ����Ʈ�� �´� �۲��� ������ ���ؼ� �����Ѵ�.
		CFont font;
		HFONT oldFont;
		font.CreateFontIndirect(&this->notepadForm->printInformation->GetPrintLogFont());
		oldFont = (HFONT)printDC->SelectObject(font);
		//10.3 ����Ʈ�� ������ �۲� ������ ���Ѵ�.
		TEXTMETRIC text;
		printDC->GetTextMetrics(&text);
		Long letterWidth = 0;
		Long rowCountOfPrintNote = printNote->GetLength();
		//4. ����Ʈ�� ��Ʈ�� ���� �������� �������� �ݺ��Ѵ�.
		row = 0;
		rowIndex = 0;
		i = 0;
		while (rowIndex < printNote->GetLength())
		{
			//4.1 i�� �ʱ�ȭ�Ѵ�.
			i = 0;
			//4.2 �μ��� �� �������� ����.
			printDC->StartPage();
			//10.4 �Ӹ����� ������
			if (this->notepadForm->pageSetUpInformation->GetHeader().Compare("") != 0)
			{
				//10.4.1 �Ӹ����� ���̸� ���Ѵ�.
				letterWidth = printDC->GetTextExtent
				(this->notepadForm->pageSetUpInformation->GetHeader()).cx;
				//10.4.2 �Ӹ����� ����Ѵ�.
				//�ڿ��� strechBlt�� ����ϱ� ������ ��ҵ��� ���� ����Ʈ�� ������ ������ ���ݿ���
				//�Ӹ����� ���� ���� �� ������ �����Ѵ�.
				printDC->TextOut(rect.Width() / 2 - letterWidth / 2,
					this->notepadForm->pageSetUpInformation->GetPrintableRect().top,
					this->notepadForm->pageSetUpInformation->GetHeader());
			}
			//10.5 �ٴڱ��� ������
			if (this->notepadForm->pageSetUpInformation->GetFooter().Compare("") != 0)
			{
				//10.5.1 �ٴڱ��� ���̸� ���Ѵ�.
				letterWidth = printDC->GetTextExtent
				(this->notepadForm->pageSetUpInformation->GetFooter()).cx;
				//10.5.2 �ٴڱ��� ����Ѵ�.
				//�ڿ��� strechBlt�� ����ϱ� ������ ��ҵ��� ���� ����Ʈ�� ������ ������ ���ݿ���
				//�Ӹ����� ���� ���� �� ������ �����Ѵ�.
				
				if (rowCountOfPrintNote >
					this->notepadForm->printInformation->GetPageRowCount())
				{
					printDC->TextOut(rect.Width() / 2 - letterWidth / 2,
						(this->notepadForm->printInformation->GetPageRowCount() + 1) * text.tmHeight
						+ this->notepadForm->pageSetUpInformation->GetPrintableRect().top,
						this->notepadForm->pageSetUpInformation->GetFooter());
				}
				else
				{
					printDC->TextOut(rect.Width() / 2 - letterWidth / 2,
						(rowCountOfPrintNote + 1) * text.tmHeight
						+ this->notepadForm->pageSetUpInformation->GetPrintableRect().top,
						this->notepadForm->pageSetUpInformation->GetFooter());
				}
				rowCountOfPrintNote -=
					this->notepadForm->printInformation->GetPageRowCount();
			}
			//4.3 �� �������� �μ��� ���� �������� �������� �׸��� ����Ʈ�� ��Ʈ�� ���� �������� �������� �ݺ��Ѵ�.
			while (i < pageRowCount && rowIndex < printNote->GetLength())
			{
				//4.3.1 ���� ���� ���Ѵ�.
				row = note->GetAt(rowIndex);
				//4.3.2 ��µ� �κ��� ��ġ�� ������Ʈ���ش�.
				this->glyphXPos = glyphXPos;
				this->glyphYPos = i;
				//4.3.3 ���������� ���� texts�� ����Ѵ�.
				row->Accept(this);
				//4.3.4 ���� ��ġ�� ������Ų��.
				rowIndex++;
				i++;
			}
			//4.4 �� �������� ���� ��á�� ������ ���� �������� �ݴ´�.
			printDC->EndPage();
		}
	}
	//6. ������ ���� ������ ������
	else
	{
		//4. ����Ʈ�� ��Ʈ�� ���� �������� �������� �ݺ��Ѵ�.
		row = 0;
		rowIndex = 0;
		i = 0;
		while (rowIndex < printNote->GetLength())
		{
			//4.1 i�� �ʱ�ȭ�Ѵ�.
			i = 0;
			//4.2 �μ��� �� �������� ����.
			printDC->StartPage();
			//4.3 �� �������� �μ��� ���� �������� �������� �׸��� ����Ʈ�� ��Ʈ�� ���� �������� �������� �ݺ��Ѵ�.
			while (i < pageRowCount && rowIndex < printNote->GetLength())
			{
				//4.3.1 ���� ���� ���Ѵ�.
				row = note->GetAt(rowIndex);
				//4.3.2 ��µ� �κ��� ��ġ�� ������Ʈ���ش�.
				this->glyphXPos = glyphXPos;
				this->glyphYPos = i;
				//4.3.3 ���������� ���� texts�� ����Ѵ�.
				row->Accept(this);
				//4.3.4 ���� ��ġ�� ������Ų��.
				rowIndex++;
				i++;
			}
			//4.4 �� �������� ���� ��á�� ������ ���� �������� �ݴ´�.
			printDC->EndPage();
		}
	}
}

//Row
void PrintingVisitor::VisitRow(Glyph* row)
{
	Glyph* letter = 0;//���� �ּ�
	string content;//���� ������ ���� ����
	Long letterWidth = 0;//���ڳʺ�
	Long letterIndex = 0;//�ٿ��� ���� ��ġ
	//1. ����Ʈ�� �´� �۲��� ������ ���ؼ� �����Ѵ�.
	CFont font;
	HFONT oldFont;
	font.CreateFontIndirect(&this->notepadForm->printInformation->GetPrintLogFont());
	oldFont = (HFONT)this->dc->SelectObject(font);
	//2. ����Ʈ�� ������ �۲��� ������ ��´�.
	TEXTMETRIC text;
	this->dc->GetTextMetrics(&text);
	//3. ���� ���ڰ������� �������� �ݺ��Ѵ�.
	while (letterIndex < row->GetLength())
	{
		//3.1 ���� ���ڸ� ���Ѵ�.
		letter = row->GetAt(letterIndex);
		//3.2 ���� ������ ������ ���Ѵ�.
		content = CString(letter->GetContent().c_str());
		//3.3 ���� ������ �ʺ� ������� �ؼ� �޸��忡�� ������ X��ǥ ��ġ�� ������Ʈ���ش�.
		this->glyphXPos += letterWidth;
		letterWidth = this->dc->GetTextExtent(content.c_str()).cx;
		//3.4 ���� ���ڸ� ����Ѵ�.
		letter->Accept(this);
		//3.5 ������ ��ġ�� ������Ų��.
		letterIndex++;
	}
}

//DummyRow
void PrintingVisitor::VisitDummyRow(Glyph* dummyRow)
{
	Glyph* letter = 0;//���� �ּ�
	string content;//���� ������ ���� ����
	Long letterWidth = 0;//���ڳʺ�
	Long letterIndex = 0;//�ٿ��� ���� ��ġ
	//1. ����Ʈ�� �´� �۲��� ������ ���ؼ� �����Ѵ�.
	CFont font;
	HFONT oldFont;
	font.CreateFontIndirect(&this->notepadForm->printInformation->GetPrintLogFont());
	oldFont = (HFONT)this->dc->SelectObject(font);
	//2. ����Ʈ�� ������ �۲��� ������ ��´�.
	TEXTMETRIC text;
	this->dc->GetTextMetrics(&text);
	//3. ���� ���ڰ������� �������� �ݺ��Ѵ�.
	while (letterIndex < dummyRow->GetLength())
	{
		//3.1 ���� ���ڸ� ���Ѵ�.
		letter = dummyRow->GetAt(letterIndex);
		//3.2 ���� ������ ������ ���Ѵ�.
		content = CString(letter->GetContent().c_str());
		//3.3 ���� ������ �ʺ� ������� �ؼ� �޸��忡�� ������ X��ǥ ��ġ�� ������Ʈ���ش�.
		this->glyphXPos += letterWidth;
		letterWidth = this->dc->GetTextExtent(content.c_str()).cx;
		//3.4 ���� ���ڸ� ����Ѵ�.
		letter->Accept(this);
		//3.5 ������ ��ġ�� ������Ų��.
		letterIndex++;
	}
}

//SingleByteLetter
void PrintingVisitor::VisitSingleByteLetter(Glyph* singleByteLetter)
{
	//1. ����Ʈ�� �´� �۲��� ������ ���ؼ� �����Ѵ�.
	CFont font;
	HFONT oldFont;
	font.CreateFontIndirect(&this->notepadForm->printInformation->GetPrintLogFont());
	oldFont = (HFONT)this->dc->SelectObject(font);
	//2. ����Ʈ�� ������ �۲� ������ ���Ѵ�.
	TEXTMETRIC text;
	this->dc->GetTextMetrics(&text);
	//3. ������ ������ ���Ѵ�.
	CString content = CString(singleByteLetter->GetContent().c_str());
	//4. ���࿡ ���ڰ� �ǹ����̸� ������ ���� 8���� �ٲ��ش�.
	if (content == "\t")
	{
		content = "        ";
	}
	//5. ������ ������� �����Ѵ�.
	this->dc->SetBkColor(RGB(255, 255, 255));
	//6. �۲������� �޾� ���ڻ��� ���Ѵ�.
	this->dc->SetTextColor(this->notepadForm->font.GetColor());

	Long glyphYPos = 0;
	if (this->notepadForm->pageSetUpInformation != NULL)
	{
		glyphYPos = this->notepadForm->pageSetUpInformation->GetPrintableRect().top;
		if (this->notepadForm->pageSetUpInformation->GetHeader().Compare("") != 0)
		{
			glyphYPos += text.tmHeight;
		}
	}

	//7. ���ڸ� ����Ѵ�.
	this->dc->TextOut(this->glyphXPos, this->glyphYPos * text.tmHeight + glyphYPos, content);
	//8. font�� ��Ʈ�����ȭ���ڿ��� ����Ǿ����� ���� font�� ������ ���� ����� font�� ������ �� ����.
	this->dc->SelectObject(oldFont);
	font.DeleteObject();
}

//DobuleByteLetter
void PrintingVisitor::VisitDoubleByteLetter(Glyph* doubleByteLetter)
{
	//1. ����Ʈ�� �´� �۲��� ������ ���ؼ� �����Ѵ�.
	CFont font;
	HFONT oldFont;
	font.CreateFontIndirect(&this->notepadForm->printInformation->GetPrintLogFont());
	oldFont = (HFONT)this->dc->SelectObject(font);
	//2. ����Ʈ�� ������ �۲� ������ ���Ѵ�.
	TEXTMETRIC text;
	this->dc->GetTextMetrics(&text);
	//3. ������ ������ ���Ѵ�.
	CString content = CString(doubleByteLetter->GetContent().c_str());
	//4. ���࿡ ���ڰ� �ǹ����̸� ������ ���� 8���� �ٲ��ش�.
	if (content == "\t")
	{
		content = "        ";
	}
	//5. ������ ������� �����Ѵ�.
	this->dc->SetBkColor(RGB(255, 255, 255));
	//6. �۲������� �޾� ���ڻ��� ���Ѵ�.
	this->dc->SetTextColor(this->notepadForm->font.GetColor());

	Long glyphYPos = 0;
	if (this->notepadForm->pageSetUpInformation != NULL)
	{
		glyphYPos = this->notepadForm->pageSetUpInformation->GetPrintableRect().top;
		if (this->notepadForm->pageSetUpInformation->GetHeader().Compare("") != 0)
		{
			glyphYPos += text.tmHeight;
		}
	}

	//7. ���ڸ� ����Ѵ�.
	this->dc->TextOut(this->glyphXPos, this->glyphYPos * text.tmHeight + glyphYPos, content);
	//8. font�� ��Ʈ�����ȭ���ڿ��� ����Ǿ����� ���� font�� ������ ���� ����� font�� ������ �� ����.
	this->dc->SelectObject(oldFont);
	font.DeleteObject();
}

//�Ҹ���
PrintingVisitor::~PrintingVisitor()
{

}