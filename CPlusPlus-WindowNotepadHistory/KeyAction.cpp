#include "KeyAction.h"

//디폴트생성자
KeyAction::KeyAction(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
}

//전략패턴
void KeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

}

//소멸자
KeyAction::~KeyAction()
{

}