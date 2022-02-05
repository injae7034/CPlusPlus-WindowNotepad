#include "ShiftCtrlRightArrowKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"

//����Ʈ������
ShiftCtrlRightArrowKeyAction::ShiftCtrlRightArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//���� ����
void ShiftCtrlRightArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. �̵��ϱ� ���� ���� ��ġ�� ���Ѵ�.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//2. �̵��ϱ� ���� ������ ��ġ�� ���Ѵ�.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//3. ��Ʈ���� �ܾ������ ���������� �̵��Ѵ�.
	Long currentLetterIndex = this->notepadForm->note->NextWord();
	//4. �̵��� �Ŀ� ���� ��ġ�� ���Ѵ�.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//5. ���� ���� �̵��� ���� �ٷ� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	//6. �ٿ��� ĳ���� �ܾ������ �̵��Ѵ�.
	this->notepadForm->current->Move(currentLetterIndex);
	//7. �ܾ������ �̵��� �Ŀ� ���� ��ġ�� ���� �ٸ���
	if (previousRowIndex < currentRowIndex)
	{
		//7.1 ������ ó�� ���۵Ǹ�
		if (this->notepadForm->isSelecting == false)
		{
			//7.1.1 ������ ����ǰ� �ִ� ������ ���¸� �ٲ۴�.
			this->notepadForm->isSelecting = true;
			//7.1.2 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
			this->notepadForm->selectedStartXPos = previousLetterIndex;
			//7.1.3 ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
			this->notepadForm->selectedStartYPos = previousRowIndex;
		}
	}
	//8. �ܾ������ �̵��� �Ŀ� ���� ��ġ�� ���� ������(�� �� ������ �ܾ���� �̵��� ������)
	else
	{
		//8.1 �ܾ� ������ �̵��� �־�����
		if (previousLetterIndex < currentLetterIndex)
		{
			//8.1.1 ������ ó�� ���۵Ǹ�
			if (this->notepadForm->isSelecting == false)
			{
				//8.1.1.1 ������ ����ǰ� �ִ� ������ ���¸� �ٲ۴�.
				this->notepadForm->isSelecting = true;
				//8.1.1.2 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
				this->notepadForm->selectedStartXPos = previousLetterIndex;
				//8.1.1.3 ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
				this->notepadForm->selectedStartYPos = previousRowIndex;
			}
		}
	}
	//9. ������ ���� ���̸�
	if (this->notepadForm->isSelecting == true)
	{
		//9.1 �ؽ�Ʈ�� �����Ѵ�.
		this->notepadForm->selectingTexts->DoNext(previousRowIndex, previousLetterIndex,
			currentRowIndex, currentLetterIndex);
	}
}

//�Ҹ���
ShiftCtrlRightArrowKeyAction::~ShiftCtrlRightArrowKeyAction()
{

}