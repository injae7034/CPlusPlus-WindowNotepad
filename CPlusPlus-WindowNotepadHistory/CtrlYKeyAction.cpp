#include "CtrlYKeyAction.h"

//디폴트생성자
CtrlYKeyAction::CtrlYKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//전략패턴
void CtrlYKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. OnCommand로 메세지를 보낸다.
	this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_REDO);
}

//소멸자
CtrlYKeyAction::~CtrlYKeyAction()
{

}