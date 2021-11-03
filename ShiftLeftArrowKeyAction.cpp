#include "ShiftLeftArrowKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "DummyRow.h"

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
	//5. ���� ���� ��ġ�� 0���� ũ�� �̵� ���� ������ġ�� �̵� ���� ���� ��ġ�� ������
	else if (currentRowIndex > 0 && previousLetterIndex == currentLetterIndex)
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
	//6. ĳ���� �̵��� ��������(ĳ���� ���� ���� ó���� �־ ���̻� �̵��� ���� ����.)
	if (previousLetterIndex == currentLetterIndex)
	{
		//6.1 ���� �ٷ� �̵���Ų��.
		currentRowIndex = this->notepadForm->note->Previous();
		//6.2 ���� �̵��� �־�����(�̵��ϱ� ���� ���� ��Ʈ�� ó�� ���� �ƴϸ�)
		if (previousRowIndex != currentRowIndex)
		{
			//6.2.1 ���� ���� �������� �̵��� �ٷ� �����Ѵ�.
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
			//6.2.2 ���� ���� ���Ѵ�.
			Glyph* currentRow = this->notepadForm->current;
			//6.2.3 ���� ���� ���Ѵ�.
			Glyph* nextRow = this->notepadForm->note->GetAt(currentRowIndex + 1);
			//6.2.4 ���� ���� ��¥ ���̰� ���� ���� ��¥ ���̸�
			if (!dynamic_cast<DummyRow*>(nextRow) && dynamic_cast<DummyRow*>(currentRow))
			{
				//6.2.4.1 ���� �������� �̵��߱� ������ ĳ���� ���� ��ġ�� ���������� �����Ѵ�.
				currentLetterIndex = this->notepadForm->current->Last();
			}
			//6.2.5 ���� ���� ��¥ ���̰� ���� ���� ��¥ ���̸�
			else if (!dynamic_cast<DummyRow*>(nextRow) && !dynamic_cast<DummyRow*>(currentRow))
			{
				//6.2.5.1 ���� �������� �̵��߱� ������ ĳ���� ���� ��ġ�� ���������� �����Ѵ�.
				currentLetterIndex = this->notepadForm->current->Last();
			}
			//6.2.6 �� �̿ܿ���
			else
			{
				//6.2.6.1 ���� �������� �̵��߱� ������ ĳ���� ���� ��ġ�� ���������� �����Ѵ�.
				currentLetterIndex = this->notepadForm->current->Last();
				//6.2.6.2 �������� �̵��� ���� ��¥ ���̱� ������ ������ ������ġ �������� �̵��Ѵ�.
				currentLetterIndex = this->notepadForm->current->Previous();
			}
		}
	}
	//7. ���ڰ� �������̸�
	if (this->notepadForm->isSelecting == true)
	{
		//7.1 �ؽ�Ʈ�� �����Ѵ�.
		this->notepadForm->selectingTexts->DoPrevious(previousRowIndex, previousLetterIndex,
			currentRowIndex, currentLetterIndex);
	}
}

//�Ҹ���
ShiftLeftArrowKeyAction::~ShiftLeftArrowKeyAction()
{

}