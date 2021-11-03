#include "NextScrollAction.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"

//디폴트생성자
NextScrollAction::NextScrollAction(NotepadForm* notepadForm)
	:ScrollAction(notepadForm)
{

}

void NextScrollAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{	
	//1. 수평스크롤을 한 칸 다음으로 이동한다.
	this->notepadForm->scrollController->scroll[0]->LineNext();
	//2. 수평스크롤바의 수평스크롤을 한 칸 다음으로 이동시킨다.
	this->notepadForm->SetScrollPos(SB_HORZ,
		this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
	
}

void NextScrollAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. 수직스크롤을 한 줄 다음으로 이동한다.
	this->notepadForm->scrollController->scroll[1]->LineNext();
	//2. 수직스크롤바의 수직스크롤을 마지막으로 이동시킨다.
	this->notepadForm->SetScrollPos(SB_VERT,
		this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
	
}

//소멸자
NextScrollAction::~NextScrollAction()
{

}