#include "ShiftCtrlBackSpaceKeyAction.h"
#include "Glyph.h"

//����Ʈ������
ShiftCtrlBackSpaceKeyAction::ShiftCtrlBackSpaceKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void ShiftCtrlBackSpaceKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. OnCommand�� �޼����� ������.
	this->notepadForm->SendMessage(WM_COMMAND, ID_SHIFTCTRLBACKSPACEKEYACTIONCOMMAND);
}


//�Ҹ���
ShiftCtrlBackSpaceKeyAction::~ShiftCtrlBackSpaceKeyAction()
{

}