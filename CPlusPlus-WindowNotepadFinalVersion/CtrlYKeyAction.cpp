#include "CtrlYKeyAction.h"

//����Ʈ������
CtrlYKeyAction::CtrlYKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void CtrlYKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. OnCommand�� �޼����� ������.
	this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_REDO);
}

//�Ҹ���
CtrlYKeyAction::~CtrlYKeyAction()
{

}