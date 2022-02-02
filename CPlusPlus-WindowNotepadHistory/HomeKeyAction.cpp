#include "HomeKeyAction.h"
#include "Glyph.h"

//디폴트생성자
HomeKeyAction::HomeKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void HomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 현재 줄의 맨 처음으로 캐럿의 가로 위치를 이동시킨다.
	this->notepadForm->current->First();
}

//소멸자
HomeKeyAction::~HomeKeyAction()
{

}