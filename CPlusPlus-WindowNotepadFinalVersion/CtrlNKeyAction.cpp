#include "CtrlNKeyAction.h"

//����Ʈ������
CtrlNKeyAction::CtrlNKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void CtrlNKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. OnCommand�� �޼����� ������.
	this->notepadForm->SendMessage(WM_COMMAND, IDM_FILE_CLOSEANDOPENNEW);
}

//�Ҹ���
CtrlNKeyAction::~CtrlNKeyAction()
{

}