#include "CtrlZKeyAction.h"

//����Ʈ������
CtrlZKeyAction::CtrlZKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void CtrlZKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. OnCommand�� �޼����� ������.
	this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_UNDO);
}

//�Ҹ���
CtrlZKeyAction::~CtrlZKeyAction()
{

}