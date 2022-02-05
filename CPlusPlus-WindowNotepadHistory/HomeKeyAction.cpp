#include "HomeKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "DummyRow.h"

//����Ʈ������
HomeKeyAction::HomeKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void HomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ������ ����ǰ� �ִ� ���̾�����
	if (this->notepadForm->isSelecting == true)
	{
		//1.1. ���õ� �ؽ�Ʈ�� ���������Ѵ�.(������ ������.)
		this->notepadForm->selectingTexts->Undo();
		//1.2 ������ ���� ���·� �ٲ۴�.
		this->notepadForm->isSelecting = false;
		//1.3 ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
		this->notepadForm->selectedStartXPos = 0;
		//1.4 ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
		this->notepadForm->selectedStartYPos = 0;
	}
	//2. ó�� ���ڷ� �̵��ϱ� ���� ���� �ٿ��� ������ ��ġ�� �����Ѵ�.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//3. ���� ���� �� ó������ ������ ��ġ�� �̵���Ų��.
	Long currentLetterIndex = this->notepadForm->current->First();
	//4. �ڵ������� ���� ���̸�
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//4.1 ó�� ���ڷ� �̵��ϱ� ���� ������ġ�� ó�� ������ġ�� ������
		if (previousLetterIndex == currentLetterIndex)
		{
			//4.1.1 ���� ���� ��ġ�� ���Ѵ�.
			Long currentRowIndex = this->notepadForm->note->GetCurrent();
			//4.1.2 ���� ���� ��ġ�� 0���� ũ��
			if (currentRowIndex > 0)
			{
				//4.1.2.1 ���� ���� ���Ѵ�.
				Glyph* currentRow = this->notepadForm->note->GetAt(currentRowIndex);
				//4.1.2.2 ���� ���� ���Ѵ�.
				Glyph* previousRow = this->notepadForm->note->GetAt(currentRowIndex - 1);
				//4.1.2.3 ���� ���� ��¥ ���̰� ���� ���� ��¥ ���̸�
				if (dynamic_cast<DummyRow*>(currentRow) && dynamic_cast<DummyRow*>(previousRow))
				{
					//4.1.2.3.1 ���� ���� ���� �ٷ� �̵���Ų��.
					currentRowIndex = this->notepadForm->note->Move(currentRowIndex - 1);
					this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
					//4.1.2.3.2 ���� ���� ó�� ������ġ�� �̵��Ѵ�.
					currentLetterIndex = this->notepadForm->current->First();
				}
				//4.1.2.4 ���� ���� ��¥ ���̰� ���� ���� ��¥ ���̸�
				else if (dynamic_cast<DummyRow*>(currentRow)
					&& !dynamic_cast<DummyRow*>(previousRow))
				{
					//4.1.2.4.1 ���� ���� ���� �ٷ� �̵���Ų��.
					currentRowIndex = this->notepadForm->note->Move(currentRowIndex - 1);
					this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
					//4.1.2.4.2 ���� ���� ó�� ������ġ�� �̵��Ѵ�.
					currentLetterIndex = this->notepadForm->current->First();
				}	
			}
		}
	}
}

//�Ҹ���
HomeKeyAction::~HomeKeyAction()
{

}