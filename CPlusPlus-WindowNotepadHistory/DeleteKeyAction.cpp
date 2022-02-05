#include "DeleteKeyAction.h"
#include "Glyph.h"

//디폴트생성자
DeleteKeyAction::DeleteKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void DeleteKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. OnCommand로 메세지를 보낸다.
	this->notepadForm->SendMessage(WM_COMMAND, ID_DELETEKEYACTIONCOMMAND);
}

//소멸자
DeleteKeyAction::~DeleteKeyAction()
{

}