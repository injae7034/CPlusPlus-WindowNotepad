#include "CtrlFKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "CommandHistory.h"

//디폴트생성자
CtrlFKeyAction::CtrlFKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//전략패턴
void CtrlFKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. OnCommand로 메세지를 보낸다.
	this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_FIND);
}

//소멸자
CtrlFKeyAction::~CtrlFKeyAction()
{

}