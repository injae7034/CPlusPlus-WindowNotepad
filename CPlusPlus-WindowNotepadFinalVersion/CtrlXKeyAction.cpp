#include "CtrlXKeyAction.h"

//����Ʈ������
CtrlXKeyAction::CtrlXKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void CtrlXKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ���ÿ����� ������
	if (this->notepadForm->isSelecting == true)
	{
		//1.1 OnCommand�� �޼����� ������.
		this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_CUT);
	}
}

//�Ҹ���
CtrlXKeyAction::~CtrlXKeyAction()
{

}