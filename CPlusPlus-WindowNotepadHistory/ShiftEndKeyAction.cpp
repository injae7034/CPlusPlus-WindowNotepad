#include "ShiftEndKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "DummyRow.h"

//����Ʈ������
ShiftEndKeyAction::ShiftEndKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void ShiftEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ������ ���ڷ� �̵��ϱ� ���� ���� �ٿ��� ������ ��ġ�� �����Ѵ�.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//2. ���� �ٿ��� ������ ���� ��ġ�� �̵��ϰ� ������ ��ġ�� ��ȯ�޴´�.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	Long currentLetterIndex = this->notepadForm->current->Last();
	//3. �̵��� �־�����
	if (previousLetterIndex < currentLetterIndex)
	{
		//3.1 ������ ó�� ���۵Ǹ�
		if (this->notepadForm->isSelecting == false)
		{
			//3.1.1 ������ ����ǰ� �ִ� ������ ���¸� �ٲ۴�.
			this->notepadForm->isSelecting = true;
			//3.1.2 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
			this->notepadForm->selectedStartXPos = previousLetterIndex;
			//3.1.3 ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
			this->notepadForm->selectedStartYPos = previousRowIndex;
		}
	}
	//4. �ڵ������� ���� ���̸�
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//4.1 ������ ���ڷ� �̵��ϱ� ���� ������ġ�� ������ ������ġ�� ������
		if (previousLetterIndex == currentLetterIndex)
		{
			//4.1.1 ���� ���� ��ġ�� ������ ���� ��ġ�� �ƴϸ�
			Long lastRowIndex = this->notepadForm->note->GetLength() - 1;
			if (currentRowIndex < lastRowIndex)
			{
				//4.1.1.1 ���� ���� ���Ѵ�.
				Glyph* nextRow = this->notepadForm->note->GetAt(currentRowIndex + 1);
				//4.1.1.2 ���� ���� ��¥ ���̸�
				if (dynamic_cast<DummyRow*>(nextRow))
				{
					//4.1.1.2.1 ���� ���� ���� �ٷ� �̵���Ų��.
					currentRowIndex = this->notepadForm->note->Move(currentRowIndex + 1);
					previousRowIndex = currentRowIndex;
					this->notepadForm->current = this->notepadForm->note
						->GetAt(currentRowIndex);
					//4.1.1.2.2 ������ ���� ��ġ�� �̵��ϱ� ���� ó�� ������ ��ġ�� ���Ѵ�.
					previousLetterIndex = this->notepadForm->current->First();
					//4.1.1.2.3 ���� ���� ������ ������ġ�� �̵��Ѵ�.
					currentLetterIndex = this->notepadForm->current->Last();
					//4.1.1.2.4 ������ ó�� ���۵Ǹ�
					if (this->notepadForm->isSelecting == false)
					{
						//4.1.1.2.4.1 ������ ����ǰ� �ִ� ������ ���¸� �ٲ۴�.
						this->notepadForm->isSelecting = true;
						//4.1.1.2.4.2 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
						this->notepadForm->selectedStartXPos = previousLetterIndex;
						//4.1.1.2.4.3 ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
						this->notepadForm->selectedStartYPos = previousRowIndex;
					}
				}
			}
		}
	}
	//5. ������ ���� ���̸�
	if (this->notepadForm->isSelecting == true)
	{
		//5.1 �ؽ�Ʈ�� �����Ѵ�.
		this->notepadForm->selectingTexts->DoNext(previousRowIndex, previousLetterIndex,
			currentRowIndex, currentLetterIndex);
	}
}

//�Ҹ���
ShiftEndKeyAction::~ShiftEndKeyAction()
{

}