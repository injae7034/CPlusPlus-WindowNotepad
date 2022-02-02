#include "CtrlShiftSKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "CommandHistory.h"

//디폴트생성자
CtrlShiftSKeyAction::CtrlShiftSKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//전략패턴
void CtrlShiftSKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. OnCommand로 메세지를 보낸다.
	this->notepadForm->SendMessage(WM_COMMAND, IDM_FILE_SAVEDIFFERENTNAME);
}

//소멸자
CtrlShiftSKeyAction::~CtrlShiftSKeyAction()
{

}