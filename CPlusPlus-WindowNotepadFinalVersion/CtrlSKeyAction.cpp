#include "CtrlSKeyAction.h"

//����Ʈ������
CtrlSKeyAction::CtrlSKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void CtrlSKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. OnCommand�� �޼����� ������.
	this->notepadForm->SendMessage(WM_COMMAND, IDM_FILE_SAVE);
}

//�Ҹ���
CtrlSKeyAction::~CtrlSKeyAction()
{

}