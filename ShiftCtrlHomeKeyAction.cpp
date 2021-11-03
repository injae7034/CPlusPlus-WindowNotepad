#include "ShiftCtrlHomeKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"

//����Ʈ������
ShiftCtrlHomeKeyAction::ShiftCtrlHomeKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void ShiftCtrlHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ó�� �ٷ� �̵��ϱ� ���� ���� ��ġ�� �����Ѵ�.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//2. ó�� ���ڷ� �̵��ϱ� ���� ������ ��ġ�� �����Ѵ�.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//3. ���� ó�� �ٷ� �̵��Ѵ�.
	Long currentRowIndex = this->notepadForm->note->First();
	//4. ���� ���� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	//5. ���� ó�� ���ڷ� �̵��Ѵ�.
	Long currentLetterIndex = this->notepadForm->current->First();
	//6. ù�ٷ� �̵��� �Ŀ� ���� ��ġ�� �̵��ϱ� ���� ���� ��ġ�� ���� �ٸ���
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
	//7. ù�ٷ� �̵��� �Ŀ� ���� ��ġ�� �̵��ϱ� ���� ���� ��ġ�� ���� ����, ������ġ�� ���� �ٸ���
	else if (previousRowIndex == currentRowIndex && previousLetterIndex > currentLetterIndex)
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
	//8. ������ ���� ���̸�
	if (this->notepadForm->isSelecting == true)
	{
		//8.1 �ؽ�Ʈ�� �����Ѵ�.
		this->notepadForm->selectingTexts->DoPrevious(previousRowIndex, previousLetterIndex,
			currentRowIndex, currentLetterIndex);
	}
}

//�Ҹ���
ShiftCtrlHomeKeyAction::~ShiftCtrlHomeKeyAction()
{

}