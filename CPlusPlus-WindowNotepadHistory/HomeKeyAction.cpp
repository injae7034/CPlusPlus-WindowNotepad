#include "HomeKeyAction.h"
#include "Glyph.h"

//����Ʈ������
HomeKeyAction::HomeKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void HomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ���� ���� �� ó������ ĳ���� ���� ��ġ�� �̵���Ų��.
	this->notepadForm->current->First();
}

//�Ҹ���
HomeKeyAction::~HomeKeyAction()
{

}