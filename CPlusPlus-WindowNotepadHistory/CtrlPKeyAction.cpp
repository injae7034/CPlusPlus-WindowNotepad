#include "CtrlPKeyAction.h"

//����Ʈ������
CtrlPKeyAction::CtrlPKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void CtrlPKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. OnCommand�� �޼����� ������.
	this->notepadForm->SendMessage(WM_COMMAND, IDM_FILE_PRINT);
}

//�Ҹ���
CtrlPKeyAction::~CtrlPKeyAction()
{

}