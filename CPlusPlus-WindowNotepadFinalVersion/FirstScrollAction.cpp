#include "FirstScrollAction.h"
#include "ScrollController.h"
#include "Scroll.h"

//디폴트생성자
FirstScrollAction::FirstScrollAction(NotepadForm* notepadForm)
	:ScrollAction(notepadForm)
{

}

void FirstScrollAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. 수평스크롤을 처음으로 이동한다.
	this->notepadForm->scrollController->scroll[0]->First();	
	//2. 수평스크롤바의 수평스크롤을 처음으로 이동시킨다.
	this->notepadForm->SetScrollPos(SB_HORZ, 
		this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
}

void FirstScrollAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. 수직스크롤을 처음으로 이동한다.
	this->notepadForm->scrollController->scroll[1]->First();
	//2. 수직스크롤바의 수직스크롤을 처음으로 이동시킨다.
	this->notepadForm->SetScrollPos(SB_VERT,
		this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
}

//소멸자
FirstScrollAction::~FirstScrollAction()
{

}