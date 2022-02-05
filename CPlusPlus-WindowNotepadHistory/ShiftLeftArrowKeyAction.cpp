#include "ShiftLeftArrowKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"

//����Ʈ������
ShiftLeftArrowKeyAction::ShiftLeftArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void ShiftLeftArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. �������� �̵��ϱ� ���� ������ ��ġ�� �����Ѵ�.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//2. �������� �̵��ϱ� ���� ���� ��ġ�� �����Ѵ�.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//3. �������� �̵��ϰ� ĳ���� ��ġ�� ��ȯ�޴´�.
	Long currentLetterIndex = this->notepadForm->current->Previous();
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//4. ĳ���� �� �� ������ �̵�������
	if (previousLetterIndex > currentLetterIndex)
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
	//5. ĳ���� �̵��� ��������(ĳ���� ���� ���� ó���� �־ ���̻� �̵��� ���� ����.)
	if (previousLetterIndex == currentLetterIndex)
	{
		//5.1 ���� �ٷ� �̵���Ų��.
		currentRowIndex = this->notepadForm->note->Previous();
		//5.2 ���� �̵��� �־�����(�̵��ϱ� ���� ���� ��Ʈ�� ó�� ���� �ƴϸ�)
		if (previousRowIndex > currentRowIndex)
		{
			//5.2.1 ���� ���� �������� �̵��� �ٷ� �����Ѵ�.
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
			//5.2.2 ���� �������� �̵��߱� ������ ĳ���� ���� ��ġ�� ���������� �����Ѵ�.
			currentLetterIndex = this->notepadForm->current->Last();
			//5.2.3 ������ ó�� ���۵Ǹ�
			if (this->notepadForm->isSelecting == false)
			{
				//5.2.3.1 ������ ����ǰ� �ִ� ������ ���¸� �ٲ۴�.
				this->notepadForm->isSelecting = true;
				//5.2.3.2 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
				this->notepadForm->selectedStartXPos = previousLetterIndex;
				//5.2.3.3 ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
				this->notepadForm->selectedStartYPos = previousRowIndex;
			}
		}
	}
	//6. ���ڰ� �������̸�
	if (this->notepadForm->isSelecting == true)
	{
		//6.1 �ؽ�Ʈ�� �����Ѵ�.
		this->notepadForm->selectingTexts->DoPrevious(previousRowIndex, previousLetterIndex,
			currentRowIndex, currentLetterIndex);
	}
}

//�Ҹ���
ShiftLeftArrowKeyAction::~ShiftLeftArrowKeyAction()
{

}