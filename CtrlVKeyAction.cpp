#include "CtrlVKeyAction.h"

//����Ʈ������
CtrlVKeyAction::CtrlVKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void CtrlVKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. OnCommand�� �޼����� ������.
	this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_PASTE);
}

//�Ҹ���
CtrlVKeyAction::~CtrlVKeyAction()
{

}