#include "CtrlOKeyAction.h"

//����Ʈ������
CtrlOKeyAction::CtrlOKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void CtrlOKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. OnCommand�� �޼����� ������.
	this->notepadForm->SendMessage(WM_COMMAND, IDM_FILE_OPEN);
}

//�Ҹ���
CtrlOKeyAction::~CtrlOKeyAction()
{

}