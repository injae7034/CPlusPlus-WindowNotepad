#include "HomeKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"

//디폴트생성자
HomeKeyAction::HomeKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void HomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	Long VScrollPos = 0;//세로스크롤의 현재위치
	Long beginCaretYPos = 0;//캐럿의 세로위치 시작지점
	Long endCaretYPos = 0;//캐럿의 세로위치 종료지점
	Long VerticalSum = 0;//세로스크롤의 현재위치 + 현재화면의 세로길이
	Long distance = 0;//스크롤이 이동할 위치
	Long currentRowIndex = 0;//이전으로 이동한 후에 현재 줄의 위치

	//1. 현재 줄의 맨 처음으로 캐럿의 가로 위치를 이동시킨다.
	Long currentScrollPos = this->notepadForm->current->First();
	//2. 수평스크롤을 처음으로 이동시켜준다.
	this->notepadForm->scrollController->scroll[0]->Move(currentScrollPos);
	//3. 수평스크롤바의 수평스크롤을 이동시킨다.
	this->notepadForm->SetScrollPos(SB_HORZ,
		this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
	//4. 캐럿의 현재 세로 위치를 구한다.
	currentRowIndex = this->notepadForm->note->GetCurrent();
	//5. caret 이동 후에 캐럿의 세로 범위(시작과 끝)을 구한다.(beginCaretYPos, endCaretYPos)
	beginCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
	endCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);
	//6. 수직스크롤을 이동하기 전에 현재 수직스크롤의 VScrollPos(스크롤의 현재 위치)를 구한다.
	VScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
	//7. 수직스크롤의 현재위치(VScrollPos)와 현재화면의 세로길이의 합을 구한다.
	VerticalSum = VScrollPos + this->notepadForm->
		scrollController->scroll[1]->GetPageSize();
	//8. 캐럿의 세로 위치 시작점(beginCaretYPos)이 
	//수직스크롤의 현재위치(VScrollPos)보다 메모장에서 위에 있으면
	if (beginCaretYPos < VScrollPos)
	{
		//8.1 수직스크롤이 이동할 위치(distance)를 캐럿의 세로 위치 시작점으로 정한다.
		distance = beginCaretYPos;
		//8.2 수직스크의 현재 위치를 distance로 이동시킨다.
		this->notepadForm->scrollController->scroll[1]->Move(distance);
		//8.3 수직스크롤바의 Thumb를 수직스크롤의 현재위치로 이동시킨다.
		this->notepadForm->SetScrollPos(SB_VERT,
			this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
	}
	//9. 캐럿의 세로위치 끝지점(endCaretYPos)이 수직스크롤의 현재 위치(VScrollPos)와 
	//현재화면의 세로길이의 합(VerticalSum)보다 메모장에서 아래에 있으면
	else if (endCaretYPos > VerticalSum)
	{
		//9.1 수직스크롤이 이동할 범위(distance)를 구한다.
		//캐럿의 세로 위치 마지막 지점에서 화면의 세로길이의 차로 정한다.
		distance = endCaretYPos - this->notepadForm->
			scrollController->scroll[1]->GetPageSize();
		//9.2 아래로 이동할 때 위에 글자가 안잘리도록 하기 위해서 여백을 구한다.
		Long pageSize = this->notepadForm->scrollController->scroll[1]->GetPageSize();
		Long letterHeight = this->notepadForm->textExtent->GetHeight();
		Long blank = pageSize - (pageSize / letterHeight * letterHeight);
		//9.3 수직스크롤의 현재위치를 distance + 여백(blank)으로 이동시킨다.
		this->notepadForm->scrollController->scroll[1]->Move(distance + blank);
		//9.3 수직스크롤바의 Thumb를 수직스크롤의 현재 위치로 이동시킨다.
		this->notepadForm->SetScrollPos(SB_VERT,
			this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
	}
}

//소멸자
HomeKeyAction::~HomeKeyAction()
{

}