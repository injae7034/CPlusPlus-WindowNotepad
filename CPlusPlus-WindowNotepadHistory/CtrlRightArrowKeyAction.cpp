#include "CtrlRightArrowKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"

//디폴트생성자
CtrlRightArrowKeyAction::CtrlRightArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlRightArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	Long HScrollPos = 0;//가로스크롤의 현재위치
	Long VScrollPos = 0;//세로스크롤의 현재위치
	Long beginCaretYPos = 0;//캐럿의 세로위치 시작지점
	Long endCaretYPos = 0;//캐럿의 세로위치 종료지점
	Long caretXPos = 0;//캐럿의 가로위치
	Long VerticalSum = 0;//세로스크롤의 현재위치 + 현재화면의 세로길이
	Long HorizontalSum = 0;//가로스크롤의 현재위치 + 현재화면의 가로길이
	Long distance = 0;//스크롤이 이동할 위치
	Long maxScrollPos = 0;//스크롤바의 Thumb이 이동할 수 최대 위치
	Long previousRowIndex = 0;//이전으로 이동하기 전에 현재 줄의 위치
	Long currentRowIndex = 0;//이전으로 이동한 후에 현재 줄의 위치

	//1. 이동하기 전에 현재 줄의 위치를 구한다.
	previousRowIndex = this->notepadForm->note->GetCurrent();
	//2. Note의 NextWord를 호출한다.
	Long currentCaretindex = this->notepadForm->note->NextWord();
	//3. 이동한 후에 현재 줄의 위치를 구한다.
	currentRowIndex = this->notepadForm->note->GetCurrent();
	//4. 현재줄을 변경한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	//5. 현재줄의 캐럿의 가로 위치를 변경한다.
	Long i = this->notepadForm->current->First();
	while (i < currentCaretindex)
	{
		//5.1 캐럿을 다음으로 이동시킨다.
		i = this->notepadForm->current->Next();
	}
	//6. 이동한 뒤에 현재 캐럿의 세로 위치가 그대로이면(줄이 바뀌지X, 한 줄 내에서 이동함)
	if (previousRowIndex == currentRowIndex)
	{
		//6.1 caret 이동 후에 캐럿의 세로 범위(시작과 끝)을 구한다.(beginCaretYPos, endCaretYPos)
		beginCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
		endCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);
		//6.2 수직스크롤을 이동하기 전에 현재 수직스크롤의 VScrollPos(스크롤의 현재 위치)를 구한다.
		VScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
		//6.3 세로스크롤의 현재위치(VScrollPos)와 현재화면의 세로길이의 합을 구한다.
		VerticalSum = VScrollPos + this->notepadForm->
			scrollController->scroll[1]->GetPageSize();
		//6.4 캐럿의 세로 위치 시작점(beginCaretYPos)이 
		//수직스크롤의 현재위치(VScrollPos)보다 메모장에서 위에 있으면
		if (beginCaretYPos < VScrollPos)
		{
			//6.4.1 수직스크롤이 이동할 위치(distance)를 캐럿의 세로 위치 시작점으로 정한다.
			distance = beginCaretYPos;
			//6.4.2 수직스크의 현재 위치를 distance로 이동시킨다.
			this->notepadForm->scrollController->scroll[1]->Move(distance);
			//6.4.3 수직스크롤바의 Thumb를 수직스크롤의 현재위치로 이동시킨다.
			this->notepadForm->SetScrollPos(SB_VERT,
				this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
		}
		//6.5 캐럿의 세로위치 끝지점(endCaretYPos)이 수직스크롤의 현재 위치(VScrollPos)와 
		//현재화면의 세로길이의 합(VerticalSum)보다 메모장에서 아래에 있으면
		else if (endCaretYPos > VerticalSum)
		{
			//6.5.1 수직스크롤이 이동할 범위(distance)를 구한다.
			//캐럿의 세로 위치 마지막 지점에서 화면의 세로길이의 차로 정한다.
			distance = endCaretYPos - this->notepadForm->
				scrollController->scroll[1]->GetPageSize();
			//6.5.2 수직스크롤의 현재위치를 distance로 이동시킨다.
			this->notepadForm->scrollController->scroll[1]->Move(distance);
			//6.5.3 수직스크롤바의 Thumb를 수직스크롤의 현재 위치로 이동시킨다.
			this->notepadForm->SetScrollPos(SB_VERT,
				this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
		}
		//6.6 현재 캐럿의 가로 위치를 구한다.
		caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
			current->GetPartOfContent(currentCaretindex));
		//6.7 수평스크롤의 현재 위치를 구한다.
		HScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
		//6.8 수평스크롤의 현재위치(HScrollPos)와 현재화면의 가로길이의 합을 구한다.
		HorizontalSum = HScrollPos + this->notepadForm->
			scrollController->scroll[0]->GetPageSize();
		//6.9 캐럿의 현재 가로위치(caretXPos)가 수평스크롤의 현재위치(HScrollPos)보다
			//메모장에서 앞에 있으면
		if (caretXPos < HScrollPos)
		{
			//6.9.1 수평스크롤이 이동할 위치(distance)를 구한다.
			distance = caretXPos - (this->notepadForm->
				scrollController->scroll[0]->GetPageSize() / 5);
			//6.9.2 distance가 음수이면
			if (distance < 0)
			{
				//6.9.2.1 distance를 0으로 바꿔준다.
				distance = 0;
			}
			//6.9.3 수평스크롤의 현재위치를 distance로 이동시킨다.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//6.9.4 수평스크롤바의 Thumb를 수평스크롤의 현재 위치로 이동시킨다.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
		//6.10 캐럿의 현재 가로위치(caretXPos)가 수평스크롤의 현재위치(HScrollPos)와
		//현재화면의 가로길이의 합(HorizontalSum)보다 메모장에서 뒤에 있으면
		else if (caretXPos > HorizontalSum)
		{
			//6.10.1 수평스크롤이 이동할 위치(distance)를 구한다.
			distance = caretXPos - (this->notepadForm->
				scrollController->scroll[0]->GetPageSize() / 5 * 4);
			//6.10.2 수평스크롤바의 Thumb의 최대 이동값(maxScrollPos)를 구한다.
			maxScrollPos = this->notepadForm->scrollController->scroll[0]->GetMax()
				- this->notepadForm->scrollController->scroll[0]->GetPageSize();
			//6.10.3 distance가 maxScrollPos보다 크면
			if (distance > maxScrollPos)
			{
				//6.10.3.1 distance에 maxScrollPos를 대입한다.
				distance = maxScrollPos;
			}
			//6.10.4 수평스크롤의 현재위치를 distance로 이동시킨다.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//6.10.5 수평스크롤바의 Thumb를 수평스크롤의 현재 위치로 이동시킨다.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
	}
	//7. 이동한 뒤에 현재 캐럿의 세로 위치가 바뀌었으면(줄이 바뀜. 현재 줄에서 다음 줄로 이동함.)
	else
	{
		//7.1 캐럿이 이동한 후에 caretYPos(캐럿의 세로 위치)의 범위(시작과 끝)를 구한다.
		beginCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
		endCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);
		//7.2 수직스크롤을 이동하기 전에 현재 수직스크롤의 VScrollPos(스크롤의 현재 위치)를 구한다.
		VScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
		//7.3 세로스크롤의 현재위치(VScrollPos)와 현재화면의 세로길이의 합을 구한다.
		VerticalSum = VScrollPos + this->notepadForm->
			scrollController->scroll[1]->GetPageSize();
		//7.4 캐럿의 세로 위치 시작점(beginCaretYPos)이 
		//수직스크롤의 현재위치(VScrollPos)보다 메모장에서 위에 있으면
		if (beginCaretYPos < VScrollPos)
		{
			//7.4.1 수직스크롤이 이동할 위치(distance)를 캐럿의 세로 위치 시작점으로 정한다.
			distance = beginCaretYPos;
			//7.4.2 수직스크의 현재 위치를 distance로 이동시킨다.
			this->notepadForm->scrollController->scroll[1]->Move(distance);
			//7.4.3 수직스크롤바의 Thumb를 수직스크롤의 현재위치로 이동시킨다.
			this->notepadForm->SetScrollPos(SB_VERT,
				this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
		}
		//7.5 캐럿의 세로위치 끝지점(endCaretYPos)이 수직스크롤의 현재 위치(VScrollPos)와 
		//현재화면의 세로길이의 합(VerticalSum)보다 메모장에서 아래에 있으면
		else if (endCaretYPos > VerticalSum)
		{
			//7.5.1 수직스크롤이 이동할 범위(distance)를 구한다.
			//캐럿의 세로 위치 마지막 지점에서 화면의 세로길이의 차로 정한다.
			distance = endCaretYPos - this->notepadForm->
				scrollController->scroll[1]->GetPageSize();
			//7.5.2 수직스크롤의 현재위치를 distance로 이동시킨다.
			this->notepadForm->scrollController->scroll[1]->Move(distance);
			//7.5.3 수직스크롤바의 Thumb를 수직스크롤의 현재 위치로 이동시킨다.
			this->notepadForm->SetScrollPos(SB_VERT,
				this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
		}
		//7.6 캐럿이 이동한 후에 캐럿의 현재 가로위치(caretXPos)를 구한다.
		caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
			current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
		//7.7 수평스크롤이 이동하기전에 수평스크롤의 현재위치(HScrollPos)를 구한다.
		HScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
		//7.8 수평스크롤의 현재위치(HScrollPos)와 현재화면의 가로길이의 합을 구한다.
		HorizontalSum = HScrollPos + this->notepadForm->
			scrollController->scroll[0]->GetPageSize();
		//7.9 캐럿의 현재 가로위치(caretXPos)가 수평스크롤의 현재위치(HScrollPos)보다
		//메모장에서 앞에 있으면
		if (caretXPos < HScrollPos)
		{
			//7.9.1 수평스크롤이 이동할 위치(distance)를 구한다.
			distance = caretXPos - (this->notepadForm->
				scrollController->scroll[0]->GetPageSize() / 5);
			//7.9.2 distance가 음수이면
			if (distance < 0)
			{
				//7.9.2.1 distance를 0으로 바꿔준다.
				distance = 0;
			}
			//7.9.3 수평스크롤의 현재위치를 distance로 이동시킨다.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//7.9.4 수평스크롤바의 Thumb를 수평스크롤의 현재 위치로 이동시킨다.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
		//7.10 캐럿의 현재 가로위치(caretXPos)가 수평스크롤의 현재위치(HScrollPos)와
		//현재화면의 가로길이의 합(HorizontalSum)보다 메모장에서 뒤에 있으면
		else if (caretXPos > HorizontalSum)
		{
			//7.10.1 수평스크롤이 이동할 위치(distance)를 구한다.
			distance = caretXPos - (this->notepadForm->
				scrollController->scroll[0]->GetPageSize() / 5 * 4);
			//7.10.2 수평스크롤바의 Thumb의 최대 이동값(maxScrollPos)를 구한다.
			maxScrollPos = this->notepadForm->scrollController->scroll[0]->GetMax()
				- this->notepadForm->scrollController->scroll[0]->GetPageSize();
			//7.10.3 distance가 maxScrollPos보다 크면
			if (distance > maxScrollPos)
			{
				//7.10.3.1 distance에 maxScrollPos를 대입한다.
				distance = maxScrollPos;
			}
			//7.10.4 수평스크롤의 현재위치를 distance로 이동시킨다.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//7.10.5 수평스크롤바의 Thumb를 수평스크롤의 현재 위치로 이동시킨다.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
	}

}

//소멸자
CtrlRightArrowKeyAction::~CtrlRightArrowKeyAction()
{

}