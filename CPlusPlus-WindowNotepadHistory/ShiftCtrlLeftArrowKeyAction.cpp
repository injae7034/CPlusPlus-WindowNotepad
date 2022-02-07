#include "ShiftCtrlLeftArrowKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"

//����Ʈ������
ShiftCtrlLeftArrowKeyAction::ShiftCtrlLeftArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void ShiftCtrlLeftArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. �̵��ϱ� ���� ���� ��ġ�� ���Ѵ�.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//2. �̵��ϱ� ���� ������ ��ġ�� ���Ѵ�.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//3. �ڵ������� ���� ���� �ƴϸ�
	Long currentRowIndex = previousRowIndex;
	Long currentLetterIndex = previousLetterIndex;
	if (this->notepadForm->isRowAutoChanging == false)
	{
		//3.1 ��Ʈ���� �ܾ������ �������� �̵��Ѵ�.
		currentLetterIndex = this->notepadForm->note->PreviousWord();
	}
	//4. �ڵ������� ���� ���̸�
	else
	{
		//4.1 ��Ʈ���� �ڵ������� ���� ���� ���¿��� �ܾ������ ���������� �̵��Ѵ�.
		this->notepadForm->note->
			PreviousWordOnRowAutoChange(currentRowIndex, currentLetterIndex);
	}
	//5. �̵��� �Ŀ� ���� �ٰ� ������ ��ġ�� ���Ѵ�.
	currentRowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	currentLetterIndex = this->notepadForm->current->GetCurrent();
	//6. �ܾ������ �̵��� �Ŀ� ���� ��ġ�� ���� �ٸ���
	if (previousRowIndex > currentRowIndex)
	{
		//6.1 ������ ó�� ���۵Ǹ�
		if (this->notepadForm->isSelecting == false)
		{
			//6.1.1 ������ ����ǰ� �ִ� ������ ���¸� �ٲ۴�.
			this->notepadForm->isSelecting = true;
			//6.1.2 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
			this->notepadForm->selectedStartXPos = previousLetterIndex;
			//6.1.3 ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
			this->notepadForm->selectedStartYPos = previousRowIndex;
		}
	}
	//7. �ܾ������ �̵��� �Ŀ� ���� ��ġ�� ���� ������(�� �� ������ �ܾ���� �̵��� ������)
	else
	{
		//7.1 �ܾ� ������ �̵��� �־�����
		if (previousLetterIndex > currentLetterIndex)
		{
			//7.1.1 ������ ó�� ���۵Ǹ�
			if (this->notepadForm->isSelecting == false)
			{
				//7.1.1.1 ������ ����ǰ� �ִ� ������ ���¸� �ٲ۴�.
				this->notepadForm->isSelecting = true;
				//7.1.1.2 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
				this->notepadForm->selectedStartXPos = previousLetterIndex;
				//7.1.1.3 ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
				this->notepadForm->selectedStartYPos = previousRowIndex;
			}
		}
	}
	//8. ������ ���� ���̸�
	if (this->notepadForm->isSelecting == true)
	{
		//8.1 �ؽ�Ʈ�� �����Ѵ�.
		this->notepadForm->selectingTexts->DoPrevious(previousRowIndex, previousLetterIndex,
			currentRowIndex, currentLetterIndex);
	}
}

//�Ҹ���
ShiftCtrlLeftArrowKeyAction::~ShiftCtrlLeftArrowKeyAction()
{

}