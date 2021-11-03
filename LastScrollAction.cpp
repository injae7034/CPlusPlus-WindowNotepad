#include "LastScrollAction.h"
#include "ScrollController.h"
#include "Scroll.h"

//디폴트생성자
LastScrollAction::LastScrollAction(NotepadForm* notepadForm)
	:ScrollAction(notepadForm)
{

}

//OnHScroll
void LastScrollAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. 수평스크롤을 마지막으로 이동한다.
	this->notepadForm->scrollController->scroll[0]->Last();
	//2. 수평스크롤바의 수평스크롤을 마지막으로 이동시킨다.
	this->notepadForm->SetScrollPos(SB_HORZ, 
		this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
}

//OnVScroll
void LastScrollAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. 수직스크롤을 마지막으로 이동한다.
	this->notepadForm->scrollController->scroll[1]->Last();
	//2. 수직스크롤바의 수직스크롤을 마지막으로 이동시킨다.
	this->notepadForm->SetScrollPos(SB_VERT, 
		this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
}

//소멸자
LastScrollAction::~LastScrollAction()
{

}