#include "BackSpaceKeyAction.h"
#include "Glyph.h"

//디폴트생성자
BackSpaceKeyAction::BackSpaceKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//전략패턴
void BackSpaceKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. OnCommand로 메세지를 보낸다.
	this->notepadForm->SendMessage(WM_COMMAND, ID_BACKSPACEKEYACTIONCOMMAND);
}

//소멸자
BackSpaceKeyAction::~BackSpaceKeyAction()
{

}