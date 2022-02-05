#include "ShiftCtrlDeleteKeyAction.h"
#include "Glyph.h"

//����Ʈ������
ShiftCtrlDeleteKeyAction::ShiftCtrlDeleteKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void ShiftCtrlDeleteKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. OnCommand�� �޼����� ������.
	this->notepadForm->SendMessage(WM_COMMAND, ID_SHIFTCTRLDELETEKEYACTIONCOMMAND);
}


//�Ҹ���
ShiftCtrlDeleteKeyAction::~ShiftCtrlDeleteKeyAction()
{

}