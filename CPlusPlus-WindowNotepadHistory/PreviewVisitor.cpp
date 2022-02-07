#include "PreviewVisitor.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "TextExtent.h"
#include "Font.h"
#include "PrintInformation.h"
#include "PreviewForm.h"

//����Ʈ������
PreviewVisitor::PreviewVisitor(NotepadForm* notepadForm, CDC* dc,
	Long glyphXPos, Long glyphYPos)
	:GlyphVisitor(notepadForm, dc, glyphXPos, glyphYPos)
{
	this->tempDC = 0;
}

//���������
PreviewVisitor::PreviewVisitor(const PreviewVisitor& source)
	:GlyphVisitor(source)
{
	this->tempDC = source.tempDC;
}

//ġȯ������
PreviewVisitor& PreviewVisitor::operator=(const PreviewVisitor& source)
{
	GlyphVisitor::operator=(source);

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
	//3. ����Ʈ�� ������ ������ ȸ������ ĥ�Ѵ�.
	CDC dcBgc;
	dcBgc.CreateCompatibleDC(this->dc);
	HBITMAP hbmpBgc = ::CreateCompatibleBitmap(dcBgc, rect.Width(), rect.Height());
	HBITMAP oldBMPBgc= (HBITMAP)dcBgc.SelectObject(hbmpBgc);
	dcBgc.FillRect(&rect, CBrush::FromHandle((HBRUSH)GetStockObject(GRAY_BRUSH)));
	//4. ����Ʈ�� ��Ʈ�� ���Ѵ�.
	Glyph* printNote = this->notepadForm->printInformation->GetPrintNote();
	//5. �� �������� �μ��� ���� ���� ���Ѵ�.
	Long pageRowCount = this->notepadForm->printInformation->GetPageRowCount();
	//6. ��Ʈ���� �����Ѵ�.(����Ʈ ȭ���� �޸��� ȭ�麸�� ��ũ�� ������ �޸��� ȭ�鿡�� ����Ʈ ȭ����
	//����� �����ֱ� ���ؼ��� ��Ҹ� ��������ϴµ� ��ҽ�Ű�� ���ؼ��� ��Ʈ���� �����ؾ��Ѵ�.)
	CDC dcText;
	dcText.CreateCompatibleDC(this->dc);
	HBITMAP hbmp = ::CreateCompatibleBitmap(dcText, rect.Width(), rect.Height());
	HBITMAP oldBMP = (HBITMAP)dcText.SelectObject(hbmp);
	//�ؽ�Ʈ�� ��µ� �κ��� �Ͼ������ ĥ�Ѵ�.
	dcText.FillRect(&rect, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	this->tempDC = &dcText;
	dcText.SetMapMode(MM_ANISOTROPIC);
	dcText.SetWindowExt(12, 12);
	dcText.SetViewportExt(5, 5);
	//7. �� �������� �μ��� ���� �������� �������� �׸��� ����Ʈ�� ��Ʈ�� ���� �������� �������� �ݺ��Ѵ�.
	Glyph* row = 0;
	Long rowIndex = 0;
	Long i = 0;
	while (i < pageRowCount && rowIndex < printNote->GetLength())
	{
		//7.1 ���� ���� ���Ѵ�.
		row = note->GetAt(rowIndex);
		//7.2 ��µ� �κ��� ��ġ�� ������Ʈ���ش�.
		this->glyphXPos = 0;
		this->glyphYPos = i;
		//7.3 ���������� ���� texts�� ����Ѵ�.
		row->Accept(this);
		//7.4 ���� ��ġ�� ������Ų��.
		rowIndex++;
		i++;
	}
	//8. ��Ʈ���� ��ҵǸ鼭 ���ڰ� �������°� �����ϱ� ���� ��ġ
	this->dc->SetStretchBltMode(HALFTONE);
	//9. �̸����� ���� ���߾��� ���Ѵ�.
	CRect clientRect;
	this->notepadForm->previewForm->GetClientRect(&clientRect);
	CPoint centerPoint = clientRect.CenterPoint();
	//10. Ŭ���̾�Ʈ ������ ũ�Ⱑ �ٲ� ��ŭ�� ������ ���Ѵ�. 
	double changedRectWidth = clientRect.Width();
	double changedRectHeight = clientRect.Height();
	double multiplicandWidth = changedRectWidth 
		/ this->notepadForm->previewForm->GetOriginalRectWidth();
	double multiplicandHeight = changedRectHeight 
		/ this->notepadForm->previewForm->GetOriginalRectHeight();
	//11. ��Ʈ�� ��� ũ�⸦ ���Ѵ�.
	double downsizedWidth = rect.Width() / 10;
	double downsizedHeight = rect.Height() / 10;
	
	this->dc->StretchBlt(0, 0, rect.Width(), rect.Height(), &dcBgc,
		0, 0, rect.Width(), rect.Height(), SRCCOPY);
	dcBgc.SelectObject(oldBMPBgc);
	::DeleteObject(hbmpBgc);
	dcBgc.DeleteDC();
	
	//11. ����Ʈȭ�� ũ�⿡ �°� ������ ��Ʈ���� ���� ȭ��ũ�⿡ �°� ��ҽ��Ѽ� �̸����� ���� �߾ӿ� ����Ѵ�.
	this->dc->StretchBlt(centerPoint.x - downsizedWidth / 2 * multiplicandWidth,
		centerPoint.y - downsizedHeight / 2 * multiplicandHeight,
		downsizedWidth * multiplicandWidth, downsizedHeight * multiplicandHeight,
		&dcText, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
	//12. ��Ʈ���� �����Ѵ�.
	dcText.SelectObject(oldBMP);
	//13.HBITMAP�� CDC�� �����.
	::DeleteObject(hbmp);
	dcText.DeleteDC();
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
	//7. ���ڸ� ����Ѵ�.
	this->tempDC->TextOut(this->glyphXPos, this->glyphYPos * text.tmHeight, content);
	//8. font�� ��Ʈ�����ȭ���ڿ��� ����Ǿ����� ���� font�� ������ ���� ����� font�� ������ �� ����.
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
	//4. ���࿡ ���ڰ� �ǹ����̸� ������ ���� 8���� �ٲ��ش�.
	if (content == "\t")
	{
		content = "        ";
	}
	//5. ������ ������� �����Ѵ�.
	this->tempDC->SetBkColor(RGB(255, 255, 255));
	//6. �۲������� �޾� ���ڻ��� ���Ѵ�.
	this->tempDC->SetTextColor(this->notepadForm->font.GetColor());
	//7. ���ڸ� ����Ѵ�.
	this->tempDC->TextOut(this->glyphXPos, this->glyphYPos * text.tmHeight, content);
	//8. font�� ��Ʈ�����ȭ���ڿ��� ����Ǿ����� ���� font�� ������ ���� ����� font�� ������ �� ����.
	this->tempDC->SelectObject(oldFont);
	font.DeleteObject();
}

//�Ҹ���
PreviewVisitor::~PreviewVisitor()
{

}