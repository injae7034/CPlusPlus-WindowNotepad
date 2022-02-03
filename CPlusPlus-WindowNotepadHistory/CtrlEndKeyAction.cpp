#include "CtrlEndKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"

//����Ʈ������
CtrlEndKeyAction::CtrlEndKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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
	//2. �޸����� ���� ���������� ���� ��ġ�� �̵���Ų��.
	Long lastRowIndex = this->notepadForm->note->Last();
	//3. �޸����� ���� ���� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(lastRowIndex);
	//4. ���� ���� �� ���������� ĳ���� ��ġ�� �̵���Ų��.
	this->notepadForm->current->Last();
}

//�Ҹ���
CtrlEndKeyAction::~CtrlEndKeyAction()
{

}