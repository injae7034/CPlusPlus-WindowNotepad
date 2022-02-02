#include "PreviousScrollAction.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"

//디폴트생성자
PreviousScrollAction::PreviousScrollAction(NotepadForm* notepadForm)
	:ScrollAction(notepadForm)
{

}

void PreviousScrollAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	
	//1. 수평스크롤을 한 칸 이전으로 이동한다.
	this->notepadForm->scrollController->scroll[0]->LinePrevious();
	//2. 수평스크롤바의 수평스크롤을 한 칸 이전으로 이동시킨다.
	this->notepadForm->SetScrollPos(SB_HORZ,
		this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
	
}

void PreviousScrollAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. 수직스크롤을 한 줄 이전으로 이동한다.
	this->notepadForm->scrollController->scroll[1]->LinePrevious();
	//2. 수직스크롤바의 수직스크롤을 한 줄 이전으로 이동시킨다.
	this->notepadForm->SetScrollPos(SB_VERT,
		this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
}

//소멸자
PreviousScrollAction::~PreviousScrollAction()
{

}