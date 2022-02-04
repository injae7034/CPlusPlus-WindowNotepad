#include "ShiftRightArrowKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"

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
	//5. ĳ���� �̵��� ��������(ĳ���� ���� �������� ��ġ�ϰ� �־��� ������)
	if (previousLetterIndex == currentLetterIndex)
	{
		//5.1 ���� �ٷ� �̵���Ų��.
		currentRowIndex = this->notepadForm->note->Next();
		//5.2 ���� �̵��� �־�����(�̵��ϱ� ���� ���� ��Ʈ�� ������ ���� �ƴϸ�)
		if (previousRowIndex < currentRowIndex)
		{
			//5.2.1 ���� ���� �������� �̵��� �ٷ� �����Ѵ�.
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
			//5.2.2 ĳ���� ���� ��ġ�� ó������ ������.(���� ���� ���� �ٷ� �Ű����� ������
			//ĳ���� ���� ���� ������ ��ġ���� ���� ���� ó�� ��ġ�� �̵��Ѵ�.)
			currentLetterIndex = this->notepadForm->current->First();
			//5.2.3 ������ ó�� ���۵Ǹ�
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
	}
	//6. ���ڰ� �������̸�
	if (this->notepadForm->isSelecting == true)
	{
		//6.1 ���ڸ� �����Ѵ�.
		this->notepadForm->selectingTexts->DoNext(previousRowIndex, previousLetterIndex,
			currentRowIndex, currentLetterIndex);
	}
}

//�Ҹ���
ShiftRightArrowKeyAction::~ShiftRightArrowKeyAction()
{

}