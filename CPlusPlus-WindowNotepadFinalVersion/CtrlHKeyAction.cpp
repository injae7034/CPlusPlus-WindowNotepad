#include "CtrlHKeyAction.h"

//����Ʈ������
CtrlHKeyAction::CtrlHKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void CtrlHKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. OnCommand�� �޼����� ������.
	this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_REPLACE);
}

//�Ҹ���
CtrlHKeyAction::~CtrlHKeyAction()
{

}