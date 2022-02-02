#include "CtrlNKeyAction.h"

//디폴트생성자
CtrlNKeyAction::CtrlNKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//전략패턴
void CtrlNKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. OnCommand로 메세지를 보낸다.
	this->notepadForm->SendMessage(WM_COMMAND, IDM_FILE_CLOSEANDOPENNEW);
}

//소멸자
CtrlNKeyAction::~CtrlNKeyAction()
{

}