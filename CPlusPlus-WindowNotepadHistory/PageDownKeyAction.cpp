#include "PageDownKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"

//디폴트생성자
PageDownKeyAction::PageDownKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void PageDownKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 수직스크롤을 한 줄 다음으로 이동한다.
	this->notepadForm->scrollController->scroll[1]->PageNext();
	//2. 수직스크롤바의 수직스크롤을 한 줄 다음으로 이동시킨다.
	this->notepadForm->SetScrollPos(SB_VERT,
		this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
}

//소멸자
PageDownKeyAction::~PageDownKeyAction()
{

}