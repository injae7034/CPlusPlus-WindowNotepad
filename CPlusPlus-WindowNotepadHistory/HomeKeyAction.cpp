#include "HomeKeyAction.h"
#include "Glyph.h"
#include "SelectText.h"

//디폴트생성자
HomeKeyAction::HomeKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void HomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 현재 줄의 맨 처음으로 캐럿의 위치를 이동시킨다.
	this->notepadForm->current->First();
	//2. 글자 선택을 해제한다.
	SelectText selectText(this->notepadForm);
	selectText.Undo();
}

//소멸자
HomeKeyAction::~HomeKeyAction()
{

}