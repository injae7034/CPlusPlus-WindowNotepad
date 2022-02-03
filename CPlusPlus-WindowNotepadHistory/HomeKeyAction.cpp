#include "HomeKeyAction.h"
#include "Glyph.h"
#include "SelectText.h"

//����Ʈ������
HomeKeyAction::HomeKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void HomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ���� ���� �� ó������ ĳ���� ��ġ�� �̵���Ų��.
	this->notepadForm->current->First();
	//2. ���� ������ �����Ѵ�.
	SelectText selectText(this->notepadForm);
	selectText.Undo();
}

//�Ҹ���
HomeKeyAction::~HomeKeyAction()
{

}