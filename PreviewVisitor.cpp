#include "PreviewVisitor.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "TextExtent.h"
#include "Font.h"
#include "PrintInformation.h"
#include "PreviewForm.h"
#include "Glyph.h"
#include "PageSetUpInformation.h"

//����Ʈ������
PreviewVisitor::PreviewVisitor(NotepadForm* notepadForm, CDC* dc,
	Long glyphXPos, Long glyphYPos)
	:GlyphVisitor(notepadForm, dc, glyphXPos, glyphYPos)//�������� �κ� ó��
{
	this->tempDC = 0;//previewVisitor���� �ִ� ���� �κ� ó��
}

//���������
PreviewVisitor::PreviewVisitor(const PreviewVisitor& source)
	:GlyphVisitor(source)//�������� �κ� ó��
{
	this->tempDC = source.tempDC;//previewVisitor���� �ִ� ���� �κ� ó��
}

//ġȯ������
PreviewVisitor& PreviewVisitor::operator=(const PreviewVisitor& source)
{
	GlyphVisitor::operator=(source);//�������� �κ� ó��

	this->tempDC = source.tempDC;//previewVisitor���� �ִ� ���� �κ� ó��

	return *this;
}

//Note �̸������ ������������ �����ָ� �ȴ�. ������������ ���������� ��ư�� ������
//���������� ���������� �����ָ� �ȴ�.
void PreviewVisitor::VisitNote(Glyph* note)
{
	//1. ����Ʈ ���̾�α��� hdc���� cdc�� ���Ѵ�.
	CDC* cdc = CDC::FromHandle(this->notepadForm->printInformation->GetHDC());
	//2. ����Ʈ ������ ������ ���Ѵ�. 
	CRect rect(0, 0, cdc->GetDeviceCaps(HORZRES), cdc->GetDeviceCaps(VERTRES));
	//3. ���ڸ� ����� ��Ʈ���� �����Ѵ�.(����Ʈ ȭ���� �޸��� ȭ�麸�� ��ũ�� ������ �޸��� ȭ�鿡��
	//����Ʈ ȭ���� ����� �����ֱ� ���ؼ��� ��Ҹ� ��������ϴµ� ��ҽ�Ű�� ���ؼ��� ��Ʈ���� �����ؾ��Ѵ�.)
	CDC dcText;
	dcText.CreateCompatibleDC(this->dc);
	HBITMAP hbmp = ::CreateCompatibleBitmap(dcText, rect.Width(), rect.Height());
	HBITMAP oldBMP = (HBITMAP)dcText.SelectObject(hbmp);
	//4. �ؽ�Ʈ�� ��µ� �κ��� ����� ������� ĥ�Ѵ�.
	dcText.FillRect(&rect, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	//5. ���ڰ� ��µ� ��Ʈ���� dc�� �����Ѵ�.
	this->tempDC = &dcText;
	//6. �̸����� ���� ���߾��� ���Ѵ�.
	CRect clientRect;
	this->notepadForm->previewForm->GetClientRect(&clientRect);
	CPoint centerPoint = clientRect.CenterPoint();
	//7. Ŭ���̾�Ʈ ������ ũ�Ⱑ �ٲ� ��ŭ�� ������ ���Ѵ�. 
	double changedRectWidth = clientRect.Width();
	double changedRectHeight = clientRect.Height();
	double firstRectWidth = this->notepadForm->previewForm->GetFirstClientRect().Width();
	double firstRectHeight = this->notepadForm->previewForm->GetFirstClientRect().Height();
	double multiplicandWidth = changedRectWidth / firstRectWidth;
	double multiplicandHeight = changedRectHeight / firstRectHeight;
	//8. ��Ʈ�� ��� ũ�⸦ ���Ѵ�.
	double downsizedWidth = rect.Width() / 10;
	double downsizedHeight = rect.Height() / 10;
	//9. ������ ���� ������ ������
	Long glyphXPos = 0;
	if (this->notepadForm->pageSetUpInformation != NULL)
	{
		//9.1 ������ �����ϰ� ����Ʈ ������ ������ ���� ��ġ�� �����Ѵ�.
		glyphXPos = this->notepadForm->pageSetUpInformation->GetPrintableRect().left;
		//9.2 ����Ʈ�� �´� �۲��� ������ ���ؼ� �����Ѵ�.
		CFont font;
		HFONT oldFont;
		TEXTMETRIC text;
		font.CreateFontIndirect(&this->notepadForm->printInformation->GetPrintLogFont());
		oldFont = (HFONT)dcText.SelectObject(font);
		dcText.GetTextMetrics(&text);
		Long letterWidth = 0;
		//9.3 �Ӹ����� ������
		if (this->notepadForm->pageSetUpInformation->GetHeader().Compare("") != 0)
		{
			//9.3.1 �Ӹ����� ���̸� ���Ѵ�.
			letterWidth = dcText.GetTextExtent
			(this->notepadForm->pageSetUpInformation->GetHeader()).cx;
			//9.3.2 �Ӹ����� ����Ѵ�.
			//�ڿ��� strechBlt�� ����ϱ� ������ ��ҵ��� ���� ����Ʈ�� ������ ������ ���ݿ���
			//�Ӹ����� ���� ���� �� ������ �����Ѵ�.
			dcText.TextOut(rect.Width() / 2 - letterWidth / 2,
				this->notepadForm->pageSetUpInformation->GetPrintableRect().top,
				this->notepadForm->pageSetUpInformation->GetHeader());
		}
		//9.4 �ٴڱ��� ������
		if (this->notepadForm->pageSetUpInformation->GetFooter().Compare("") != 0)
		{
			//9.4.1 �ٴڱ��� ���̸� ���Ѵ�.
			letterWidth = dcText.GetTextExtent
			(this->notepadForm->pageSetUpInformation->GetFooter()).cx;
			//9.4.2 �ٴڱ��� ����Ѵ�.
			//�ڿ��� strechBlt�� ����ϱ� ������ ��ҵ��� ���� ����Ʈ�� ������ ������ ���ݿ���
			//�Ӹ����� ���� ���� �� ������ �����Ѵ�.
			dcText.TextOut(rect.Width() / 2 - letterWidth / 2,
				this->notepadForm->pageSetUpInformation->GetPrintableRect().bottom 
				- text.tmHeight,
				this->notepadForm->pageSetUpInformation->GetFooter());
		}
	}
	//10. �̸����� ������ �� �������� ��µ� ���� �������� �������� �ݺ��Ѵ�.
	Glyph* row = 0;
	Long rowIndex = 0;
	while (rowIndex < this->notepadForm->previewForm->GetNote()->GetLength())
	{
		//10.1 ���� ���� ���Ѵ�.
		row = note->GetAt(rowIndex);
		//10.2 ��µ� �κ��� ��ġ�� ������Ʈ���ش�.
		this->glyphXPos = glyphXPos;
		this->glyphYPos = rowIndex;
		//10.3 ���������� ���� texts�� ����Ѵ�.
		row->Accept(this);
		//10.4 ���� ��ġ�� ������Ų��.
		rowIndex++;
	}
	//11. ��Ʈ���� ��ҵǸ鼭 ���ڰ� �������°� �����ϱ� ���� ��ġ
	this->dc->SetStretchBltMode(HALFTONE);
	//12. �̸����������� Ŭ���̾�Ʈ������ 7/10�� ȸ������ ĥ�Ѵ�.
	CRect fillRect(0, 0, changedRectWidth / 10 * 7, changedRectHeight);
	this->dc->FillRect(&fillRect, CBrush::FromHandle((HBRUSH)GetStockObject(GRAY_BRUSH)));
	//13. ������ ���� ������ �ְ�, ������ ������ �����̸� �Ǵ� �μⰡ�ɿ����� �ʺ� ���̺��� ũ��
	if ((this->notepadForm->pageSetUpInformation != 0 &&
		this->notepadForm->pageSetUpInformation->GetOrientation() != 1)
		|| rect.Width() > rect.Height())
	{
		//13.1 ���ι��⿡ �°� ��ҵ� ũ�⸦ ���������ش�.
		downsizedWidth = rect.Width() / 7;
		downsizedHeight = rect.Height() / 7;
		//13.2 ����Ʈȭ�� ũ�⿡ �°� ������ ���ں�Ʈ���� ���� ȭ��ũ�⿡ �°� ��ҽ��Ѽ�
		//Ŭ���̾�Ʈ�� 10/7 ������ ����Ѵ�.
		this->dc->StretchBlt(changedRectWidth / 10 * 7 / 2 - (downsizedHeight / 2
			* multiplicandWidth * 7016 / rect.Height()),
			centerPoint.y - (downsizedWidth / 2 * multiplicandHeight * 4958 / rect.Width()),
			downsizedHeight * multiplicandWidth * 7016 / rect.Height(),
			downsizedWidth * multiplicandHeight * 4958 / rect.Width(),
			&dcText, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
	}
	//14. ������ ���� ������ ���ų� �μ� ���ɿ����� �ʺ� ���̺��� �۰ų� ������
	else
	{
		//14.1 ����Ʈȭ�� ũ�⿡ �°� ������ ���ں�Ʈ���� ���� ȭ��ũ�⿡ �°� ��ҽ��Ѽ�
		//Ŭ���̾�Ʈ������ 10/7 ������ ����Ѵ�.
		Long textStart = changedRectWidth / 10 * 7 / 2 - (downsizedWidth / 2
			* multiplicandWidth * 4958 / rect.Width());
		Long textEnd = downsizedWidth * multiplicandWidth * 4958 / rect.Width();
		this->dc->StretchBlt(textStart,
			centerPoint.y - (downsizedHeight / 2 * multiplicandHeight * 7016 / rect.Height()),
			textEnd,
			downsizedHeight * multiplicandHeight * 7016 / rect.Height(),
			&dcText, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
	}
	//15. �ؽ�Ʈ�� ����� dc�� �����Ѵ�.
	dcText.SelectObject(oldBMP);
	::DeleteObject(hbmp);
	dcText.DeleteDC();
	//16. �̸����� ������ ���������� ������� ĥ�Ѵ�.
	fillRect = CRect(changedRectWidth / 10 * 7 / 2 + downsizedHeight / 2
		* multiplicandWidth + changedRectWidth / 10 * 7 / 2 - downsizedHeight / 2
		* multiplicandWidth, 0,
		changedRectWidth, changedRectHeight);
	this->dc->FillRect(&fillRect, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
}

//Row
void PreviewVisitor::VisitRow(Glyph* row)
{
	Glyph* letter = 0;//���� �ּ�
	string content;//���� ������ ���� ����
	Long letterWidth = 0;//���ڳʺ�
	Long letterIndex = 0;//�ٿ��� ���� ��ġ
	//1. ����Ʈ�� �´� �۲��� ������ ���ؼ� �����Ѵ�.
	CFont font;
	HFONT oldFont;
	font.CreateFontIndirect(&this->notepadForm->printInformation->GetPrintLogFont());
	oldFont = (HFONT)this->tempDC->SelectObject(font);
	//2. ����Ʈ�� ������ �۲��� ������ ��´�.
	TEXTMETRIC text;
	this->tempDC->GetTextMetrics(&text);
	//3. ���� ���ڰ������� �������� �ݺ��Ѵ�.
	while (letterIndex < row->GetLength())
	{
		//3.1 ���� ���ڸ� ���Ѵ�.
		letter = row->GetAt(letterIndex);
		//3.2 ���� ������ ������ ���Ѵ�.
		content = CString(letter->GetContent().c_str());
		//3.3 ���� ������ �ʺ� ������� �ؼ� �޸��忡�� ������ X��ǥ ��ġ�� ������Ʈ���ش�.
		this->glyphXPos += letterWidth;
		letterWidth = this->tempDC->GetTextExtent(content.c_str()).cx;
		//3.4 ���� ���ڸ� ����Ѵ�.
		letter->Accept(this);
		//3.5 ������ ��ġ�� ������Ų��.
		letterIndex++;
	}
}

//DummyRow
void PreviewVisitor::VisitDummyRow(Glyph* dummyRow)
{
	Glyph* letter = 0;//���� �ּ�
	string content;//���� ������ ���� ����
	Long letterWidth = 0;//���ڳʺ�
	Long letterIndex = 0;//�ٿ��� ���� ��ġ
	//1. ����Ʈ�� �´� �۲��� ������ ���ؼ� �����Ѵ�.
	CFont font;
	HFONT oldFont;
	font.CreateFontIndirect(&this->notepadForm->printInformation->GetPrintLogFont());
	oldFont = (HFONT)this->tempDC->SelectObject(font);
	//2. ����Ʈ�� ������ �۲��� ������ ��´�.
	TEXTMETRIC text;
	this->tempDC->GetTextMetrics(&text);
	//3. ���� ���ڰ������� �������� �ݺ��Ѵ�.
	while (letterIndex < dummyRow->GetLength())
	{
		//3.1 ���� ���ڸ� ���Ѵ�.
		letter = dummyRow->GetAt(letterIndex);
		//3.2 ���� ������ ������ ���Ѵ�.
		content = CString(letter->GetContent().c_str());
		//3.3 ���� ������ �ʺ� ������� �ؼ� �޸��忡�� ������ X��ǥ ��ġ�� ������Ʈ���ش�.
		this->glyphXPos += letterWidth;
		letterWidth = this->tempDC->GetTextExtent(content.c_str()).cx;
		//3.4 ���� ���ڸ� ����Ѵ�.
		letter->Accept(this);
		//3.5 ������ ��ġ�� ������Ų��.
		letterIndex++;
	}
}

//SingleByteLetter
void PreviewVisitor::VisitSingleByteLetter(Glyph* singleByteLetter)
{
	//1. ����Ʈ�� �´� �۲��� ������ ���ؼ� �����Ѵ�.
	CFont font;
	HFONT oldFont;
	font.CreateFontIndirect(&this->notepadForm->printInformation->GetPrintLogFont());
	oldFont = (HFONT)this->tempDC->SelectObject(font);
	//2. ����Ʈ�� ������ �۲� ������ ���Ѵ�.
	TEXTMETRIC text;
	this->tempDC->GetTextMetrics(&text);
	//3. ������ ������ ���Ѵ�.
	CString content = CString(singleByteLetter->GetContent().c_str());
	//4. ���࿡ ���ڰ� �ǹ����̸� ������ ���� 8���� �ٲ��ش�.
	if (content == "\t")
	{
		content = "        ";
	}
	//5. ������ ������� �����Ѵ�.
	this->tempDC->SetBkColor(RGB(255, 255, 255));
	//6. �۲������� �޾� ���ڻ��� ���Ѵ�.
	this->tempDC->SetTextColor(this->notepadForm->font.GetColor());
	//7. ���������� ������ ������
	Long glyphYPos = 0;//�ؽ�Ʈ�� ��µ� y��ǥ
	if (this->notepadForm->pageSetUpInformation != NULL)
	{
		//7.1 ������ �������� ���� ���� ���� ������ y��ǥ�� �����Ѵ�.
		glyphYPos = this->notepadForm->pageSetUpInformation->GetPrintableRect().top;
		//7.2 ������ �������� �Ӹ����� �����Ǿ� ������
		if (this->notepadForm->pageSetUpInformation->GetHeader().Compare("") != 0)
		{
			//7.2.1 �Ӹ��� ���� ���̸�ŭ �߰��� y��ǥ�� ������Ų��.
			glyphYPos += text.tmHeight;
		}
	}
	//8. ���ڸ� ����Ѵ�.
	this->tempDC->TextOut(this->glyphXPos, this->glyphYPos * text.tmHeight + glyphYPos,
		content);
	//9. font�� ��Ʈ�����ȭ���ڿ��� ����Ǿ����� ���� font�� ������ ���� ����� font�� ������ �� ����.
	this->tempDC->SelectObject(oldFont);
	font.DeleteObject();
}

//DobuleByteLetter
void PreviewVisitor::VisitDoubleByteLetter(Glyph* doubleByteLetter)
{
	//1. ����Ʈ�� �´� �۲��� ������ ���ؼ� �����Ѵ�.
	CFont font;
	HFONT oldFont;
	font.CreateFontIndirect(&this->notepadForm->printInformation->GetPrintLogFont());
	oldFont = (HFONT)this->tempDC->SelectObject(font);
	//2. ����Ʈ�� ������ �۲� ������ ���Ѵ�.
	TEXTMETRIC text;
	this->tempDC->GetTextMetrics(&text);
	//3. ������ ������ ���Ѵ�.
	CString content = CString(doubleByteLetter->GetContent().c_str());
	//4. ������ ������� �����Ѵ�.
	this->tempDC->SetBkColor(RGB(255, 255, 255));
	//5. �۲������� �޾� ���ڻ��� ���Ѵ�.
	this->tempDC->SetTextColor(this->notepadForm->font.GetColor());
	//6. ���������� ������ ������
	Long glyphYPos = 0;//�ؽ�Ʈ�� ��µ� y��ǥ
	if (this->notepadForm->pageSetUpInformation != NULL)
	{
		//6.1 ������ �������� ���� ���� ���� ������ y��ǥ�� �����Ѵ�.
		glyphYPos = this->notepadForm->pageSetUpInformation->GetPrintableRect().top;
		//6.2 ������ �������� �Ӹ����� �����Ǿ� ������
		if (this->notepadForm->pageSetUpInformation->GetHeader().Compare("") != 0)
		{
			//6.2.1 �Ӹ��� ���� ���̸�ŭ �߰��� y��ǥ�� ������Ų��.
			glyphYPos += text.tmHeight;
		}
	}
	//7. ���ڸ� ����Ѵ�.
	this->tempDC->TextOut(this->glyphXPos, this->glyphYPos * text.tmHeight + glyphYPos,
		content);
	//8. font�� ��Ʈ�����ȭ���ڿ��� ����Ǿ����� ���� font�� ������ ���� ����� font�� ������ �� ����.
	this->tempDC->SelectObject(oldFont);
	font.DeleteObject();
}

//�Ҹ���
PreviewVisitor::~PreviewVisitor()
{

}