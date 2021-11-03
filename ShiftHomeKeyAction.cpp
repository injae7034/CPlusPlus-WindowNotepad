#include "ShiftHomeKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "DummyRow.h"

//����Ʈ������
ShiftHomeKeyAction::ShiftHomeKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void ShiftHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ������ġ�� ���������� �̵��ϱ� ���� ���� ��ġ�� �����Ѵ�.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//2. ������ġ�� ���������� �̵��ϱ� ���� ������ ���� ��ġ�� �����Ѵ�.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//3. ���� ���� �� ó������ ������ ��ġ�� �̵���Ų��.
	Long currentLetterIndex = this->notepadForm->current->First();
	//4. �̵� �� ���� ��ġ�� �����Ѵ�.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//5. �̵��� �־�����
	if (previousLetterIndex > currentLetterIndex)
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
	//6. �ڵ������� ���� ���̸�
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//6.1 ó�� ���ڷ� �̵��ϱ� ���� ������ġ�� ó�� ������ġ�� ������
		if (previousLetterIndex == currentLetterIndex)
		{
			//6.1.1 ���� ���� ��ġ�� 0���� ũ��
			if (currentRowIndex > 0)
			{
				//6.1.1.1 ���� ���� ���Ѵ�.
				Glyph* currentRow = this->notepadForm->note->GetAt(currentRowIndex);
				//6.1.1.2 ���� ���� ���Ѵ�.
				Glyph* previousRow = this->notepadForm->note->GetAt(currentRowIndex - 1);
				//6.1.1.3 ���� ���� ��¥ ���̰� ���� ���� ��¥ ���̸�
				if (dynamic_cast<DummyRow*>(currentRow) && dynamic_cast<DummyRow*>(previousRow))
				{
					//6.1.1.3.1 ���� ���� ���� �ٷ� �̵���Ų��.
					currentRowIndex = this->notepadForm->note->Move(currentRowIndex - 1);
					previousRowIndex = currentRowIndex;
					this->notepadForm->current = this->notepadForm->
						note->GetAt(currentRowIndex);
					//6.1.1.3.2 ���� ���� ó�� ������ġ�� �̵��Ѵ�.
					previousLetterIndex = this->notepadForm->current->Last();
					currentLetterIndex = this->notepadForm->current->First();
					//6.1.1.3.3 ������ ó�� ���۵Ǹ�
					if (this->notepadForm->isSelecting == false)
					{
						//6.1.1.3.3.1 ������ ����ǰ� �ִ� ������ ���¸� �ٲ۴�.
						this->notepadForm->isSelecting = true;
						//6.1.1.3.3.2 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
						this->notepadForm->selectedStartXPos = previousLetterIndex;
						//6.1.1.3.3.3 ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
						this->notepadForm->selectedStartYPos = previousRowIndex;
					}
				}
				//6.1.1.4 ���� ���� ��¥ ���̰� ���� ���� ��¥ ���̸�
				else if (dynamic_cast<DummyRow*>(currentRow)
					&& !dynamic_cast<DummyRow*>(previousRow))
				{
					//6.1.1.4.1 ���� ���� ���� �ٷ� �̵���Ų��.
					currentRowIndex = this->notepadForm->note->Move(currentRowIndex - 1);
					previousRowIndex = currentRowIndex;
					this->notepadForm->current = this->notepadForm->
						note->GetAt(currentRowIndex);
					//6.1.1.4.2 ���� ���� ó�� ������ġ�� �̵��Ѵ�.
					previousLetterIndex = this->notepadForm->current->Last();
					currentLetterIndex = this->notepadForm->current->First();
					//6.1.1.4.3 ������ ó�� ���۵Ǹ�
					if (this->notepadForm->isSelecting == false)
					{
						//6.1.1.4.3.1 ������ ����ǰ� �ִ� ������ ���¸� �ٲ۴�.
						this->notepadForm->isSelecting = true;
						//6.1.1.4.3.2 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
						this->notepadForm->selectedStartXPos = previousLetterIndex;
						//6.1.1.4.3.3 ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
						this->notepadForm->selectedStartYPos = previousRowIndex;
					}
				}
			}
		}
	}
	//7. ������ ���� ���̸�
	if (this->notepadForm->isSelecting == true)
	{
		//7.1 �ؽ�Ʈ�� �����Ѵ�.
		this->notepadForm->selectingTexts->DoPrevious(previousRowIndex, previousLetterIndex,
			currentRowIndex, currentLetterIndex);
	}
}

//�Ҹ���
ShiftHomeKeyAction::~ShiftHomeKeyAction()
{

}