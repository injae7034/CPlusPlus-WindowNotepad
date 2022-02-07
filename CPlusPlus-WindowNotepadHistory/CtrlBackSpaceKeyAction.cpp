#include "CtrlBackSpaceKeyAction.h"

//디폴트생성자
CtrlBackSpaceKeyAction::CtrlBackSpaceKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlBackSpaceKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. OnCommand로 메세지를 보낸다.
	this->notepadForm->SendMessage(WM_COMMAND, ID_CTRLBACKSPACEKEYACTIONCOMMAND);
}


//소멸자
CtrlBackSpaceKeyAction::~CtrlBackSpaceKeyAction()
{

}