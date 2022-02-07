#include "PrintInformation.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "DummyRow.h"

PrintInformation::PrintInformation(NotepadForm* notepadForm, LOGFONT printLogFont,
	CDC* printerDC)
{
	this->notepadForm = notepadForm;
	this->printNote = notepadForm->note->Clone();
	this->printLogFont = printLogFont;
	this->printDC = printerDC;
	//�� �������� ���� ���� ���Ѵ�.
	CFont font;
	HFONT oldFont;
	font.CreateFontIndirect(&this->printLogFont);
	oldFont = (HFONT)this->printDC->SelectObject(font);
	TEXTMETRIC text;
	this->printDC->GetTextMetrics(&text);
	this->printableAreaHeight = this->printDC->GetDeviceCaps(VERTRES);
	this->pageRowCount = this->printableAreaHeight / text.tmHeight;

	//1. �޸����� ���ÿ����� ������
	if (this->notepadForm->isSelecting == true)
	{
		//���ÿ����� �����Ѵ�.
		//1. ���õ� ������ ���Ѵ�.
		Long startingRowPos = 0;
		Long startingLetterPos = 0;
		Long endingRowPos = 0;
		Long endingLetterPos = 0;
		this->printNote->CalculateSelectedRange(&startingRowPos,
			&startingLetterPos, &endingRowPos, &endingLetterPos);
		Long rowIndex = startingRowPos;
		Long index = startingLetterPos;
		Long rowCount = this->printNote->GetLength();
		Long letterCount = 0;
		Long letterIndex = 0;
		Glyph* row = 0;
		Glyph* letter = 0;
		bool isSelected = true;
		//2. ������ ���۵� ���� ��ġ�� ������ ������ ���� ��ġ���� �۰ų� �������� �ݺ��Ѵ�.
		while (rowIndex <= endingRowPos && isSelected == true)
		{
			//2.1 ���� ���Ѵ�.
			row = this->printNote->GetAt(rowIndex);
			//2.2 ���� ���ڰ����� ���Ѵ�.
			letterCount = row->GetLength();
			//2.3 ���� ��ġ�� ����ġ��Ų��.
			letterIndex = index;
			//2.4 ������ġ�� ���ڰ������� �������� �ݺ��Ѵ�.
			while (letterIndex < letterCount && isSelected == true)
			{
				//2.4.1 ���ڸ� ���Ѵ�.
				letter = row->GetAt(letterIndex);
				//2.4.2 ���ڰ� ������ �Ǿ�������
				if (letter->IsSelected() == true)
				{
					//2.4.2.1 ���� ���ڸ� ������ �ȵ� ���·� �ٲ��ش�.
					letter->Select(false);
				}
				//2.4.3 ���ڰ� ������ �ȵǾ�������
				else
				{
					isSelected = false;
				}
				//2.4.4 ���� ��ġ�� ������Ų��.
				letterIndex++;
			}
			//2.5 ���� ��ġ�� ������Ų��.
			rowIndex++;
			index = 0;
		}
	}
	Long rowIndex = 0;
	Glyph* row = 0;
	Glyph* previousRow = 0;
	//2. �޸��忡�� �ڵ������� �������̸�
	if (this->notepadForm->isRowAutoChanging == true)
	{
		// ��ü�ڵ������� ����Ѵ�.
		//1. Note�� �� ���� �������� �������� �ݺ��Ѵ�.
		while (rowIndex < this->printNote->GetLength())
		{
			//1.1 �޸��忡�� rowIndex��° ���� ���Ѵ�.
			row = this->printNote->GetAt(rowIndex);
			//1.2 ��¥��(DummyRow)�̸�
			if (dynamic_cast<DummyRow*>(row))
			{
				//1.2.1 ��¥��(DummyRow) ������ ��¥��(Row)�� ���Ѵ�.
				previousRow = this->printNote->GetAt(rowIndex - 1);
				//1.2.2 ��¥��(DummyRow)�� ������ ��¥��(Row)�� ��ģ��.
				row->Join(previousRow);
				//1.2.3 Note���� ��¥��(DummyRow)�� �ּҸ� �����.
				this->printNote->Remove(rowIndex);
			}
			//1.3 ��¥��(DummyRow)�� �ƴϸ�(��¥��(Row)�̸�)
			else
			{
				//1.3.1 ���� �ٷ� �̵��Ѵ�.
				rowIndex++;
			}
		}
	}
	//3. ��ü �ڵ������� �ٽ����ش�.
	Long letterIndex = 0;
	Long rowTextWidth = 0;
	Glyph* glyph = 0;
	rowIndex = 0;
	row = 0;
	previousRow = 0;
	//1. ���� ����Ʈ ������ ȭ���� ũ�⸦ ���Ѵ�.
	CRect rect(0, 0, this->printDC->GetDeviceCaps(HORZRES),
		this->printDC->GetDeviceCaps(VERTRES));
	//2. ���� ȭ���� ���� ���̸� ���Ѵ�.
	Long pageWidth = rect.Width();
	//3. Note�� �� ���� �������� �������� �ݺ��Ѵ�.
	while (rowIndex < this->printNote->GetLength())
	{
		//3.1 �޸��忡�� rowIndex��° ���� ���Ѵ�.
		row = this->printNote->GetAt(rowIndex);
		//3.2 letterIndex�� ����ġ��Ų��.
		letterIndex = 0;
		//3.3 rowTextWidth�� ����ġ��Ų��.
		rowTextWidth = 0;
		//3.4 letterIndex�� rowIndex��° ���� �ѱ��� �������� �������� 
		//�׸��� rowIndex��° ���� ���α��̰� ����ȭ���� ���α��̺��� �������� �ݺ��Ѵ�.
		while (letterIndex < row->GetLength() && rowTextWidth < pageWidth)
		{
			//3.4.1 ������ letterIndex������ ���� ���̸� �����Ѵ�.
			rowTextWidth = this->printDC->GetTextExtent
			(row->GetPartOfContent(letterIndex + 1).c_str()).cx;
			//3.4.2 letterIndex�� ������Ų��.
			letterIndex++;
		}
		//3.5 rowIndex��° ���� ���� ���̰� ���� ȭ���� ���� ���̺��� ũ�ų� ������
		if (rowTextWidth >= pageWidth)
		{
			//3.5.1 letterIndex������ ���̰� ����ȭ���� ���� ����(cx)���� ũ�� ������ 
			//�� ���ù��� ���Դ�. �׷��� ĳ���� �������� �� ĭ �̵��� �ؼ� ���̸� ���
			//����ȭ���� ���� ����(cx)���� �۴�. ĳ��(letterIndex)�� ���� ���ڸ� ���� ��ġ��
			//�ݿ��ϱ� ������ �׻� ���� ���ں��� ��ĭ �ռ� �ִ�
			//�׷��� letterIndex-1���� split�� �ؾ� ȭ���� �Ѵ� ���ڸ� ���� �ٷ� ���� �� �ִ�.
			letterIndex--;
			//3.5.2 rowIndex��° ���� ���� ���̰� ����ȭ���� ���� ���̺��� Ŀ�� ������
			//���ں��� rowIndex��° �ٿ��� letterIndex ���� ��ġ�� �ִ� ���ڵ��� ������.
			//(DummyRow����)
			glyph = row->Split(letterIndex, true);
			//3.5.3 ���ο� ���� rowIndex��° ���� ���� ��ġ�� �����ִ´�.
			rowIndex = this->printNote->Add(rowIndex + 1, glyph);
		}
		//3.6 letterIndex�� rowIndex��° ���� �ѱ��� �������� ũ�ų� ������
		else if (letterIndex >= row->GetLength())
		{
			//3.6.1 ���� �ٷ� �̵��Ѵ�.
			rowIndex++;
		}
	}
}

HBITMAP PrintInformation::CreateBitmap()
{
	CRect rect(0, 0, this->printDC->GetDeviceCaps(HORZRES),
		this->printDC->GetDeviceCaps(VERTRES));
	CDC dcTemp;
	dcTemp.CreateCompatibleDC(this->printDC);
	HBITMAP hbmp = ::CreateCompatibleBitmap(dcTemp, rect.right, rect.bottom);
	HBITMAP oldBMP = (HBITMAP)dcTemp.SelectObject(hbmp);
	dcTemp.FillRect(&rect, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));

	return hbmp;
}

PrintInformation::~PrintInformation()
{
	if (this->printNote != NULL)
	{
		delete this->printNote;
	}
	if (this->printDC != NULL)
	{
		this->printDC->DeleteDC();
	}
}