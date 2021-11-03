#include "CtrlDeleteKeyAction.h"

//디폴트생성자
CtrlDeleteKeyAction::CtrlDeleteKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlDeleteKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. OnCommand로 메세지를 보낸다.
	this->notepadForm->SendMessage(WM_COMMAND, ID_CTRLDELETEKEYACTIONCOMMAND);
}


//소멸자
CtrlDeleteKeyAction::~CtrlDeleteKeyAction()
{

}