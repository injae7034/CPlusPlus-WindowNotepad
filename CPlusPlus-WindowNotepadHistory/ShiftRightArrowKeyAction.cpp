#include "ShiftRightArrowKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "DummyRow.h"

//����Ʈ������
ShiftRightArrowKeyAction::ShiftRightArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//���� ����
void ShiftRightArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. �������� �̵��ϱ� ���� ������ ��ġ�� �����Ѵ�.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//2. �������� �̵��ϱ� ���� ���� ��ġ�� �����Ѵ�.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//3. �������� �̵��ϰ� ���� ������ ��ġ�� ��ȯ�޴´�.
	Long currentLetterIndex = this->notepadForm->current->Next();
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	Long lastRowIndex = this->notepadForm->note->GetLength() - 1;
	//4. ĳ���� �� �� ������ �̵�������
	if (previousLetterIndex < currentLetterIndex)
	{
		//4.1 ������ ó�� ���۵Ǹ�
		if (this->notepadForm->isSelecting == false)
		{
			//4.1.1 ������ ����ǰ� �ִ� ������ ���¸� �ٲ۴�.
			this->notepadForm->isSelecting = true;
			//4.1.2 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
			this->notepadForm->selectedStartXPos = previousLetterIndex;
			//4.1.3 ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
			this->notepadForm->selectedStartYPos = previousRowIndex;
		}
	}
	//5. ���� ���� ��ġ�� ������ ���� ��ġ���� �۰� �̵� ���� ������ġ�� �̵� ���� ������ġ�� ������
	else if (currentRowIndex < lastRowIndex && previousLetterIndex == currentLetterIndex)
	{
		//5.1 ������ ó�� ���۵Ǹ�
		if (this->notepadForm->isSelecting == false)
		{
			//5.1.1 ������ ����ǰ� �ִ� ������ ���¸� �ٲ۴�.
			this->notepadForm->isSelecting = true;
			//5.1.2 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
			this->notepadForm->selectedStartXPos = previousLetterIndex;
			//5.1.3 ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
			this->notepadForm->selectedStartYPos = previousRowIndex;
		}
	}
	//6. ���� ���� ������ ���� �ƴϸ�
	if (currentRowIndex < lastRowIndex)
	{
		//6.1 ���� ���� ���Ѵ�.
		Glyph* nextRow = this->notepadForm->note->GetAt(currentRowIndex + 1);
		//6.2 ���� ���� ��¥ ���̸�
		if (dynamic_cast<DummyRow*>(nextRow))
		{
			//6.2.1 ���� ������ġ�� ������ �������� ������
			Long letterIndex = this->notepadForm->current->GetLength();
			if (currentLetterIndex == letterIndex)
			{
				//6.2.1.1 ���� �ٷ� �̵���Ų��.
				currentRowIndex = this->notepadForm->note->Next();
				//6.2.1.2 ���� ���� �������� �̵��� �ٷ� �����Ѵ�.
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
				//6.2.1.3 ĳ���� ���� ��ġ�� ó������ ������.(���� ���� ���� �ٷ� �Ű����� ������
				//ĳ���� ���� ���� ������ ��ġ���� ���� ���� ó�� ��ġ�� �̵��Ѵ�.)
				currentLetterIndex = this->notepadForm->current->First();
			}
		}
		//6.3 ���� ���� ��¥ ���̸�
		else
		{
			//6.3.1 ĳ���� �̵��� ��������(ĳ���� ���� �������� ��ġ�ϰ� �־��� ������)
			if (previousLetterIndex == currentLetterIndex)
			{
				//6.3.1.1 ���� �ٷ� �̵���Ų��.
				currentRowIndex = this->notepadForm->note->Next();
				//6.3.1.2 ���� �̵��� �־�����(�̵��ϱ� ���� ���� ��Ʈ�� ������ ���� �ƴϸ�)
				if (previousRowIndex != currentRowIndex)
				{
					//6.3.1.2.1 ���� ���� �������� �̵��� �ٷ� �����Ѵ�.
					this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
					//6.3.1.2.2 ĳ���� ���� ��ġ�� ó������ ������.(���� ���� ���� �ٷ� �Ű����� ������
					//ĳ���� ���� ���� ������ ��ġ���� ���� ���� ó�� ��ġ�� �̵��Ѵ�.)
					currentLetterIndex = this->notepadForm->current->First();
				}
			}
		}
	}
	//7. ���ڰ� �������̸�
	if (this->notepadForm->isSelecting == true)
	{
		//7.1 ���ڸ� �����Ѵ�.
		this->notepadForm->selectingTexts->DoNext(previousRowIndex, previousLetterIndex,
			currentRowIndex, currentLetterIndex);
	}
}

//�Ҹ���
ShiftRightArrowKeyAction::~ShiftRightArrowKeyAction()
{

}