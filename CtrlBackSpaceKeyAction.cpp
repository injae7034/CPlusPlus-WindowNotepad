#include "CtrlBackSpaceKeyAction.h"

//����Ʈ������
CtrlBackSpaceKeyAction::CtrlBackSpaceKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlBackSpaceKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. OnCommand�� �޼����� ������.
	this->notepadForm->SendMessage(WM_COMMAND, ID_CTRLBACKSPACEKEYACTIONCOMMAND);
}


//�Ҹ���
CtrlBackSpaceKeyAction::~CtrlBackSpaceKeyAction()
{

}