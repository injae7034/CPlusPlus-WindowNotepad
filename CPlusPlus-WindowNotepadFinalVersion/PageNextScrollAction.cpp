#include "PageNextScrollAction.h"
#include "ScrollController.h"
#include "Scroll.h"

//디폴트생성자
PageNextScrollAction::PageNextScrollAction(NotepadForm* notepadForm)
	:ScrollAction(notepadForm)
{

}

void PageNextScrollAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

	//1. 수평스크롤을 한 페이지 이전으로 이동한다.
	this->notepadForm->scrollController->scroll[0]->PageNext();
	//2. 수평스크롤바의 수평스크롤을 한 칸 이전으로 이동시킨다.
	this->notepadForm->SetScrollPos(SB_HORZ,
		this->notepadForm->scrollController->scroll[0]->GetCurrentPos());

}

void PageNextScrollAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. 수직스크롤을 한 줄 다음으로 이동한다.
	this->notepadForm->scrollController->scroll[1]->PageNext();
	//2. 수직스크롤바의 수직스크롤을 한 줄 다음으로 이동시킨다.
	this->notepadForm->SetScrollPos(SB_VERT,
		this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
}

//소멸자
PageNextScrollAction::~PageNextScrollAction()
{

}