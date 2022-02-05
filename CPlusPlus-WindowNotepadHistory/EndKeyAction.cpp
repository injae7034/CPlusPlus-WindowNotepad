#include "EndKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "DummyRow.h"

//����Ʈ������
EndKeyAction::EndKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void EndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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
	//2. ������ ���ڷ� �̵��ϱ� ���� ���� �ٿ��� ������ ��ġ�� �����Ѵ�.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//3. ���� �ٿ��� ������ ���� ��ġ�� �̵��ϰ� ������ ��ġ�� ��ȯ�޴´�.
	Long currentLetterIndex = this->notepadForm->current->Last();
	//4. �ڵ������� ���� ���̸�
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//4.1 ������ ���ڷ� �̵��ϱ� ���� ������ġ�� ������ ������ġ�� ������
		if (previousLetterIndex == currentLetterIndex)
		{
			//4.1.1 ���� ���� ��ġ�� ���Ѵ�.
			Long currentRowIndex = this->notepadForm->note->GetCurrent();
			//4.1.2 ���� ���� ��ġ�� ������ ���� ��ġ�� �ƴϸ�
			Long lastRowIndex = this->notepadForm->note->GetLength() - 1;
			if (currentRowIndex < lastRowIndex)
			{
				//4.1.2.1 ���� ���� ���Ѵ�.
				Glyph* nextRow = this->notepadForm->note->GetAt(currentRowIndex + 1);
				//4.1.2.2 ���� ���� ��¥ ���̸�
				if (dynamic_cast<DummyRow*>(nextRow))
				{
					//4.1.2.2.1 ���� ���� ���� �ٷ� �̵���Ų��.
					currentRowIndex = this->notepadForm->note->Move(currentRowIndex + 1);
					this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
					//4.1.2.2.2 ���� ���� ������ ������ġ�� �̵��Ѵ�.
					currentLetterIndex = this->notepadForm->current->Last();
				}
			}
		}
	}
}

//�Ҹ���
EndKeyAction::~EndKeyAction()
{

}