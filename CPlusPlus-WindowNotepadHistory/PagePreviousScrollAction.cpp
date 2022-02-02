#include "PagePreviousScrollAction.h"
#include "ScrollController.h"
#include "Scroll.h"

//디폴트생성자
PagePreviousScrollAction::PagePreviousScrollAction(NotepadForm* notepadForm)
	:ScrollAction(notepadForm)
{

}

void PagePreviousScrollAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

	//1. 수평스크롤을 한 페이지 이전으로 이동한다.
	this->notepadForm->scrollController->scroll[0]->PagePrevious();
	//2. 수평스크롤바의 수평스크롤을 한 칸 이전으로 이동시킨다.
	this->notepadForm->SetScrollPos(SB_HORZ,
		this->notepadForm->scrollController->scroll[0]->GetCurrentPos());

}

void PagePreviousScrollAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. 수직스크롤을 한 줄 이전으로 이동한다.
	this->notepadForm->scrollController->scroll[1]->PagePrevious();
	//2. 수직스크롤바의 수직스크롤을 한 줄 이전으로 이동시킨다.
	this->notepadForm->SetScrollPos(SB_VERT,
		this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
}

//소멸자
PagePreviousScrollAction::~PagePreviousScrollAction()
{

}