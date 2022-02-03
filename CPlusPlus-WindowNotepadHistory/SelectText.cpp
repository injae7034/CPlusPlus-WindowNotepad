#include "SelectText.h"
#include "NotepadForm.h"
#include "Glyph.h"

//����Ʈ������
SelectText::SelectText(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
}

//���������� �����ϴ�
void SelectText::DoNext(Long previousRowIndex, Long previousLetterIndex, Long currentRowIndex,
	Long currentLetterIndex)
{
	Glyph* row = 0;
	Glyph* letter = 0;
	Long letterCount = 0;
	Long letterIndex = 0;
	//1. �̵��ϱ� ���� ���� ��ġ�� �̵��� ���� ���� ��ġ���� �۰ų� ���� ���� �ݺ��Ѵ�.
	while (previousRowIndex <= currentRowIndex)
	{
		//1.1 ���� ���Ѵ�.
		row = this->notepadForm->note->GetAt(previousRowIndex);
		//1.2 �̵��ϱ� �� ���� ��ġ���� �̵��� �� ���� ��ġ���� ���� ������
		if (previousRowIndex != currentRowIndex)
		{
			//1.2.1 ���� ���� ������ �����Ѵ�.
			letterCount = row->GetLength();
		}
		//1.3  �̵��ϱ� �� ���� ��ġ���� �̵��� �� ���� ��ġ���� ������
		else
		{
			//1.3.1 �̵��� �� ���� ��ġ�� �����Ѵ�.
			letterCount = currentLetterIndex;
		}
		//1.4 ���� ��ġ�� ����ġ��Ų��.
		letterIndex = previousLetterIndex;
		//1.5 ���� ��ġ�� letterCount���� �������� �ݺ��Ѵ�.
		while (letterIndex < letterCount)
		{
			//1.5.1 ���ڸ� ���Ѵ�.
			letter = row->GetAt(letterIndex);
			//1.5.2 ���ڰ� ������ �ȵǾ�������
			if (letter->IsSelected() == false)
			{
				///1.5.2.1 ���ڸ� ������ �� ���·� �ٲ��ش�.
				letter->Select(true);
			}
			//1.5.3 ���ڰ� ������ �Ǿ� ������
			else
			{
				//1.5.3.1 ���ڸ� ������ �ȵ� ���·� �ٲ��ش�.
				letter->Select(false);
			}
			//1.5.4 ���� ��ġ�� ������Ų��.
			letterIndex++;
		}
		//1.6 ���� ��ġ�� ������Ų��.
		previousRowIndex++;
		previousLetterIndex = 0;
	}
}

//�������� �����ϴ�
void SelectText::DoPrevious(Long previousRowIndex, Long previousLetterIndex, Long currentRowIndex,
	Long currentLetterIndex)
{
	Glyph* row = 0;
	Glyph* letter = 0;
	Long letterCount = 0;
	Long letterIndex = 0;
	//1. �̵��ϱ� ���� ���� ��ġ�� �̵��� ���� ���� ��ġ���� �۰ų� ���� ���� �ݺ��Ѵ�.
	while (currentRowIndex <= previousRowIndex)
	{
		//1.1 ���� ���Ѵ�.
		row = this->notepadForm->note->GetAt(currentRowIndex);
		//1.2 �̵��ϱ� �� ���� ��ġ���� �̵��� �� ���� ��ġ���� ���� ������
		if (currentRowIndex != previousRowIndex)
		{
			//1.2.1 ���� ���� ������ �����Ѵ�.
			letterCount = row->GetLength();
		}
		//1.3  �̵��ϱ� �� ���� ��ġ���� �̵��� �� ���� ��ġ���� ������
		else
		{
			//1.3.1 �̵��� �� ���� ��ġ�� �����Ѵ�.
			letterCount = previousLetterIndex;
		}
		//1.4 ���� ��ġ�� ����ġ��Ų��.
		letterIndex = currentLetterIndex;
		//1.5 ���� ��ġ�� letterCount���� �������� �ݺ��Ѵ�.
		while (letterIndex < letterCount)
		{
			//1.5.1 ���ڸ� ���Ѵ�.
			letter = row->GetAt(letterIndex);
			//1.5.2 ���ڰ� ������ �ȵǾ�������
			if (letter->IsSelected() == false)
			{
				///1.5.2.1 ���ڸ� ������ �� ���·� �ٲ��ش�.
				letter->Select(true);
			}
			//1.5.3 ���ڰ� ������ �Ǿ� ������
			else
			{
				//1.5.3.1 ���ڸ� ������ �ȵ� ���·� �ٲ��ش�.
				letter->Select(false);
			}
			//1.5.4 ���� ��ġ�� ������Ų��.
			letterIndex++;
		}
		//1.6 ���� ��ġ�� ������Ų��.
		currentRowIndex++;
		currentLetterIndex = 0;
	}
}

//������ �����ϴ�
void SelectText::Undo()
{
	//1. ���õ� ������ ���Ѵ�.
	Long startingRowPos = 0;
	Long startingLetterPos = 0;
	Long endingRowPos = 0;
	Long endingLetterPos = 0;
	this->notepadForm->note->CalculateSelectedRange(&startingRowPos,
		&startingLetterPos, &endingRowPos, &endingLetterPos);
	Long rowIndex = startingRowPos;
	Long index = startingLetterPos;
	Long rowCount = this->notepadForm->note->GetLength();
	Long letterCount = 0;
	Long letterIndex = 0;
	Glyph* row = 0;
	Glyph* letter = 0;
	bool isSelected = true;
	//2. ������ ���۵� ���� ��ġ�� ������ ������ ���� ��ġ���� �۰ų� �������� �ݺ��Ѵ�.
	while (rowIndex <= endingRowPos && isSelected == true)
	{
		//2.1 ���� ���Ѵ�.
		row = this->notepadForm->note->GetAt(rowIndex);
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

//�Ҹ���
SelectText::~SelectText()
{

}