#include "CtrlHomeKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"

//����Ʈ������
CtrlHomeKeyAction::CtrlHomeKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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
	//2. ��Ʈ���� ���� ���� ó������ �̵���Ų��.
	Long firstRowIndex = this->notepadForm->note->First();
	//3. ���� ���� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(firstRowIndex);
	//4. ���� ���� �� ó������ ĳ���� ��ġ�� �̵���Ų��.
	this->notepadForm->current->First();
}

//�Ҹ���
CtrlHomeKeyAction::~CtrlHomeKeyAction()
{

}