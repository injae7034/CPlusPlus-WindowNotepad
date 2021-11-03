#include "ShiftCtrlDeleteKeyAction.h"
#include "Glyph.h"

//디폴트생성자
ShiftCtrlDeleteKeyAction::ShiftCtrlDeleteKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void ShiftCtrlDeleteKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. OnCommand로 메세지를 보낸다.
	this->notepadForm->SendMessage(WM_COMMAND, ID_SHIFTCTRLDELETEKEYACTIONCOMMAND);
}


//소멸자
ShiftCtrlDeleteKeyAction::~ShiftCtrlDeleteKeyAction()
{

}