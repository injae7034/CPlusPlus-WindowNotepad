#include "CtrlLeftArrowKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "DummyRow.h"

//����Ʈ������
CtrlLeftArrowKeyAction::CtrlLeftArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlLeftArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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
	//2. �޸��忡�� ���� ���� ��ġ�� ���Ѵ�.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//3. �޸��忡�� ���� ������ ��ġ�� ���Ѵ�.
	Long currentLetterIndex = this->notepadForm->note->GetAt(currentRowIndex)->GetCurrent();
	//4. �ڵ������� ���� ���� �ƴϸ�
	if (this->notepadForm->isRowAutoChanging == false)
	{
		//4.1 �̵��ϱ� ���� ���� ��ġ�� ���Ѵ�.
		Long previousRowIndex = currentRowIndex;
		//4.2 �̵��ϱ� ���� ������ ��ġ�� ���Ѵ�.
		Long previousLetterIndex = currentLetterIndex;
		//4.3 ��Ʈ���� �ܾ������ �������� �̵��Ѵ�.
		currentLetterIndex = this->notepadForm->note->PreviousWord();
		//4.4 �̵��� �Ŀ� ���� ��ġ�� ���Ѵ�.
		currentRowIndex = this->notepadForm->note->GetCurrent();
		//4.5 ���� ���� �̵��� ���� �ٷ� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	}
	//5. �ڵ������� ���� ���̸�
	else
	{
		//5.1 ��Ʈ���� �ڵ����� ���� ���� ���¿��� �ܾ������ ���������� �̵��Ѵ�.
		this->notepadForm->note->
			PreviousWordOnRowAutoChange(currentRowIndex, currentLetterIndex);
		//5.2 �̵��� �Ŀ� ���� ��ġ�� ���Ѵ�.
		currentRowIndex = this->notepadForm->note->GetCurrent();
		//5.3 �޸����� ���� ���� �̵��� ���� �ٷ� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	}
}

//�Ҹ���
CtrlLeftArrowKeyAction::~CtrlLeftArrowKeyAction()
{

}