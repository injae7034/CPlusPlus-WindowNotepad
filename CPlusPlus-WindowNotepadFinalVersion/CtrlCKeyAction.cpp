#include "CtrlCKeyAction.h"

//����Ʈ������
CtrlCKeyAction::CtrlCKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void CtrlCKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ���ÿ����� ������
	if (this->notepadForm->isSelecting == true)
	{
		//1.1 OnCommand�� �޼����� ������.
		this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_COPY);
	}
}

//�Ҹ���
CtrlCKeyAction::~CtrlCKeyAction()
{

}