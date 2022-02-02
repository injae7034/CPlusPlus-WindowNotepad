#include "EndKeyAction.h"
#include "Glyph.h"

//디폴트생성자
EndKeyAction::EndKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void EndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 현재 줄의 맨 마지막으로 캐럿의 가로 위치를 이동시킨다.
	this->notepadForm->current->Last();
}

//소멸자
EndKeyAction::~EndKeyAction()
{

}