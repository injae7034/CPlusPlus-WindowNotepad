#include "DeleteKeyAction.h"
#include "Glyph.h"

//����Ʈ������
DeleteKeyAction::DeleteKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void DeleteKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. OnCommand�� �޼����� ������.
	this->notepadForm->SendMessage(WM_COMMAND, ID_DELETEKEYACTIONCOMMAND);
}

//�Ҹ���
DeleteKeyAction::~DeleteKeyAction()
{

}