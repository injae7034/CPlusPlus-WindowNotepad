#include "HomeKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"

//디폴트생성자
HomeKeyAction::HomeKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void HomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 현재 줄의 맨 처음으로 캐럿의 가로 위치를 이동시킨다.
	Long currentScrollPos = this->notepadForm->current->First();
	//2. 수평스크롤을 처음으로 이동시켜준다.
	this->notepadForm->scrollController->scroll[0]->Move(currentScrollPos);
	//3. 수평스크롤바의 수평스크롤을 이동시킨다.
	this->notepadForm->SetScrollPos(SB_HORZ,
		this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
}

//소멸자
HomeKeyAction::~HomeKeyAction()
{

}