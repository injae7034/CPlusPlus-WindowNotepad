#include "CtrlDeleteKeyAction.h"

//����Ʈ������
CtrlDeleteKeyAction::CtrlDeleteKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlDeleteKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. OnCommand�� �޼����� ������.
	this->notepadForm->SendMessage(WM_COMMAND, ID_CTRLDELETEKEYACTIONCOMMAND);
}


//�Ҹ���
CtrlDeleteKeyAction::~CtrlDeleteKeyAction()
{

}