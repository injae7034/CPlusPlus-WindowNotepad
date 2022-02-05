#include "ShiftCtrlEndKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"

//����Ʈ������
ShiftCtrlEndKeyAction::ShiftCtrlEndKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void ShiftCtrlEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ������ �ٷ� �̵��ϱ� ���� ���� ��ġ�� �����Ѵ�.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//2. ���������� ���ڷ� �̵��ϱ� ���� ������ ��ġ�� �����Ѵ�.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//3. �޸����� ���� ������ �ٷ� �̵��Ѵ�.
	Long currentRowIndex = this->notepadForm->note->Last();
	//4. �޸����� ���� ���� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	//5. ���� ���� ������ ���ڷ� �̵��Ѵ�.
	Long currentLetterIndex = this->notepadForm->current->Last();
	//6. �̵��� �Ŀ� ���� ��ġ�� ���� �ٸ���
	if (previousRowIndex < currentRowIndex)
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
		if (previousLetterIndex < currentLetterIndex)
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
		this->notepadForm->selectingTexts->DoNext(previousRowIndex, previousLetterIndex,
			currentRowIndex, currentLetterIndex);
	}
}

//�Ҹ���
ShiftCtrlEndKeyAction::~ShiftCtrlEndKeyAction()
{

}