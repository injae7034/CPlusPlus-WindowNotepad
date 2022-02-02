#include "ShiftCtrlBackSpaceKeyAction.h"
#include "Glyph.h"

//디폴트생성자
ShiftCtrlBackSpaceKeyAction::ShiftCtrlBackSpaceKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void ShiftCtrlBackSpaceKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. OnCommand로 메세지를 보낸다.
	this->notepadForm->SendMessage(WM_COMMAND, ID_SHIFTCTRLBACKSPACEKEYACTIONCOMMAND);
}


//소멸자
ShiftCtrlBackSpaceKeyAction::~ShiftCtrlBackSpaceKeyAction()
{

}