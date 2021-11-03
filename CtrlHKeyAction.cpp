#include "CtrlHKeyAction.h"

//디폴트생성자
CtrlHKeyAction::CtrlHKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//전략패턴
void CtrlHKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. OnCommand로 메세지를 보낸다.
	this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_REPLACE);
}

//소멸자
CtrlHKeyAction::~CtrlHKeyAction()
{

}