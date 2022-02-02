#include "LeftArrowKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"

//디폴트생성자
LeftArrowKeyAction::LeftArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void LeftArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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

	//1. 이전으로 이동하기 전에 캐럿의 현재 가로위치를 저장한다.
	Long previousIndex = this->notepadForm->current->GetCurrent();
	//2. 이전으로 이동하고 현재 캐럿의 가로위치를 반환받는다.
	Long currentIndex = this->notepadForm->current->Previous();
	//3. 캐럿의 이전 가로위치와 캐럿의 현재 가로위치가 같으면(캐럿의 이동이 없었으면)
	//이동이 없었다는 말은 캐럿이 현재 줄의 제일 처음에 위치해서 더이상 이동할 곳이 없다는 뜻이다.
	if (previousIndex == currentIndex)
	{
		//3.1 이전으로 이동하기 전에 캐럿의 현재 줄의 위치를 저장한다.
		previousRowIndex = this->notepadForm->note->GetCurrent();
		//3.2 현재 줄의 위치를 이전 줄로 이동시킨다.
		currentRowIndex = this->notepadForm->note->Previous();
		//3.3 이전 줄의 위치와 캐럿의 현재 줄의 위치가 서로 다르면
		if (previousRowIndex != currentRowIndex)
		{
			//3.3.1 notepadForm의 현재 줄을 변경한다.
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
			//3.3.2 현재 줄이 바뀌었기 때문에 캐럿의 현재 가로 위치를 마지막으로 변경한다.
			this->notepadForm->current->Last();
			//3.3.3. 캐럿이 이동한 후에 caretYPos(캐럿의 세로 위치)의 범위(시작과 끝)를 구한다.
			beginCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
			endCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);
			//3.3.4 수직스크롤을 이동하기 전에 현재 수직스크롤의 VScrollPos(스크롤의 현재 위치)를 구한다.
			VScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
			//3.3.5 세로스크롤의 현재위치(VScrollPos)와 현재화면의 세로길이의 합을 구한다.
			VerticalSum = VScrollPos + this->notepadForm->
				scrollController->scroll[1]->GetPageSize();
			//3.3.6 캐럿의 세로 위치 시작점(beginCaretYPos)이 
			//수직스크롤의 현재위치(VScrollPos)보다 메모장에서 위에 있으면
			if (beginCaretYPos < VScrollPos)
			{
				//3.3.6.1 수직스크롤이 이동할 위치(distance)를 캐럿의 세로 위치 시작점으로 정한다.
				distance = beginCaretYPos;
				//3.3.6.2 수직스크의 현재 위치를 distance로 이동시킨다.
				this->notepadForm->scrollController->scroll[1]->Move(distance);
				//3.3.6.3 수직스크롤바의 Thumb를 수직스크롤의 현재위치로 이동시킨다.
				this->notepadForm->SetScrollPos(SB_VERT,
					this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
			}
			//3.3.7 캐럿의 세로위치 끝지점(endCaretYPos)이 수직스크롤의 현재 위치(VScrollPos)와 
			//현재화면의 세로길이의 합(VerticalSum)보다 메모장에서 아래에 있으면
			else if (endCaretYPos > VerticalSum)
			{
				//3.3.7.1 수직스크롤이 이동할 범위(distance)를 구한다.
				//캐럿의 세로 위치 마지막 지점에서 화면의 세로길이의 차로 정한다.
				distance = endCaretYPos - this->notepadForm->
					scrollController->scroll[1]->GetPageSize();
				//3.3.7.2 수직스크롤의 현재위치를 distance로 이동시킨다.
				this->notepadForm->scrollController->scroll[1]->Move(distance);
				//3.3.7.3 수직스크롤바의 Thumb를 수직스크롤의 현재 위치로 이동시킨다.
				this->notepadForm->SetScrollPos(SB_VERT,
					this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
			}
			//3.3.8 캐럿이 이동한 후에 캐럿의 현재 가로위치(caretXPos)를 구한다.
			caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
				current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
			//3.3.9 수평스크롤이 이동하기전에 수평스크롤의 현재위치(HScrollPos)를 구한다.
			HScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
			//3.3.10 수평스크롤의 현재위치(HScrollPos)와 현재화면의 가로길이의 합을 구한다.
			HorizontalSum = HScrollPos + this->notepadForm->
				scrollController->scroll[0]->GetPageSize();
			//3.3.11 캐럿의 현재 가로위치(caretXPos)가 수평스크롤의 현재위치(HScrollPos)보다
			//메모장에서 앞에 있으면
			if (caretXPos < HScrollPos)
			{
				//3.3.11.1 수평스크롤이 이동할 위치(distance)를 구한다.
				distance = caretXPos - (this->notepadForm->
					scrollController->scroll[0]->GetPageSize() / 5);
				//3.3.11.2 distance가 음수이면
				if (distance < 0)
				{
					//3.3.11.2.1 distance를 0으로 바꿔준다.
					distance = 0;
				}
				//3.3.11.3 수평스크롤의 현재위치를 distance로 이동시킨다.
				this->notepadForm->scrollController->scroll[0]->Move(distance);
				//3.3.11.4 수평스크롤바의 Thumb를 수평스크롤의 현재 위치로 이동시킨다.
				this->notepadForm->SetScrollPos(SB_HORZ,
					this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
			}
			//3.3.12 캐럿의 현재 가로위치(caretXPos)가 수평스크롤의 현재위치(HScrollPos)와
			//현재화면의 가로길이의 합(HorizontalSum)보다 메모장에서 뒤에 있으면
			else if (caretXPos > HorizontalSum)
			{
				//3.3.12.1 수평스크롤이 이동할 위치(distance)를 구한다.
				distance = caretXPos - (this->notepadForm->
					scrollController->scroll[0]->GetPageSize() / 5 * 4);
				//3.3.12.2 수평스크롤바의 Thumb의 최대 이동값(maxScrollPos)를 구한다.
				maxScrollPos = this->notepadForm->scrollController->scroll[0]->GetMax()
					- this->notepadForm->scrollController->scroll[0]->GetPageSize();
				//3.3.12.3 distance가 maxScrollPos보다 크면
				if (distance > maxScrollPos)
				{
					//3.3.12.3.1 distance에 maxScrollPos를 대입한다.
					distance = maxScrollPos;
				}
				//3.3.12.4 수평스크롤의 현재위치를 distance로 이동시킨다.
				this->notepadForm->scrollController->scroll[0]->Move(distance);
				//3.3.12.5 수평스크롤바의 Thumb를 수평스크롤의 현재 위치로 이동시킨다.
				this->notepadForm->SetScrollPos(SB_HORZ,
					this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
			}
		}
	}
	//5. 캐럿의 이전 가로위치와 캐럿의 현재 가로위치가 다르면(캐럿이 한 줄내에서 이동했으면)
	//캐럿이 이동했다는 것은 캐럿이 이동하기 전에 줄의 처음에 있지 않았다는 뜻임
	else
	{
		//5.1 캐럿의 현재 세로 위치를 구한다.
		currentRowIndex = this->notepadForm->note->GetCurrent();
		//5.2 caret 이동 후에 캐럿의 세로 범위(시작과 끝)을 구한다.(beginCaretYPos, endCaretYPos)
		beginCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
		endCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);
		//5.3 수직스크롤을 이동하기 전에 현재 수직스크롤의 VScrollPos(스크롤의 현재 위치)를 구한다.
		VScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
		//5.4 세로스크롤의 현재위치(VScrollPos)와 현재화면의 세로길이의 합을 구한다.
		VerticalSum = VScrollPos + this->notepadForm->
			scrollController->scroll[1]->GetPageSize();
		//5.5 캐럿의 세로 위치 시작점(beginCaretYPos)이 
		//수직스크롤의 현재위치(VScrollPos)보다 메모장에서 위에 있으면
		if (beginCaretYPos < VScrollPos)
		{
			//5.5.1 수직스크롤이 이동할 위치(distance)를 캐럿의 세로 위치 시작점으로 정한다.
			distance = beginCaretYPos;
			//5.5.2 수직스크의 현재 위치를 distance로 이동시킨다.
			this->notepadForm->scrollController->scroll[1]->Move(distance);
			//5.5.3 수직스크롤바의 Thumb를 수직스크롤의 현재위치로 이동시킨다.
			this->notepadForm->SetScrollPos(SB_VERT,
				this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
		}
		//5.6 캐럿의 세로위치 끝지점(endCaretYPos)이 수직스크롤의 현재 위치(VScrollPos)와 
		//현재화면의 세로길이의 합(VerticalSum)보다 메모장에서 아래에 있으면
		else if (endCaretYPos > VerticalSum)
		{
			//5.6.1 수직스크롤이 이동할 범위(distance)를 구한다.
			//캐럿의 세로 위치 마지막 지점에서 화면의 세로길이의 차로 정한다.
			distance = endCaretYPos - this->notepadForm->
				scrollController->scroll[1]->GetPageSize();
			//5.6.2 수직스크롤의 현재위치를 distance로 이동시킨다.
			this->notepadForm->scrollController->scroll[1]->Move(distance);
			//5.6.3 수직스크롤바의 Thumb를 수직스크롤의 현재 위치로 이동시킨다.
			this->notepadForm->SetScrollPos(SB_VERT,
				this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
		}
		//5.7 캐럿이 이동한 후에 캐럿의 현재 가로위치(caretXPos)를 구한다.
		caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
			current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
		//5.8 수평스크롤이 이동하기전에 수평스크롤의 현재위치(HScrollPos)를 구한다.
		HScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
		//5.9 수평스크롤의 현재위치(HScrollPos)와 현재화면의 가로길이의 합을 구한다.
		HorizontalSum = HScrollPos + this->notepadForm->
			scrollController->scroll[0]->GetPageSize();
		//5.10 캐럿의 현재 가로위치(caretXPos)가 수평스크롤의 현재위치(HScrollPos)보다
			//메모장에서 앞에 있으면
		if (caretXPos < HScrollPos)
		{
			//5.10.1 수평스크롤이 이동할 위치(distance)를 구한다.
			distance = caretXPos - (this->notepadForm->
				scrollController->scroll[0]->GetPageSize() / 5);
			//5.10.2 distance가 음수이면
			if (distance < 0)
			{
				//5.10.2.1 distance를 0으로 바꿔준다.
				distance = 0;
			}
			//5.10.3 수평스크롤의 현재위치를 distance로 이동시킨다.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//5.10.4 수평스크롤바의 Thumb를 수평스크롤의 현재 위치로 이동시킨다.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
		//5.11 캐럿의 현재 가로위치(caretXPos)가 수평스크롤의 현재위치(HScrollPos)와
		//현재화면의 가로길이의 합(HorizontalSum)보다 메모장에서 뒤에 있으면
		else if (caretXPos > HorizontalSum)
		{
			//5.11.1 수평스크롤이 이동할 위치(distance)를 구한다.
			distance = caretXPos - (this->notepadForm->
				scrollController->scroll[0]->GetPageSize() / 5 * 4);
			//5.11.2 수평스크롤바의 Thumb의 최대 이동값(maxScrollPos)를 구한다.
			maxScrollPos = this->notepadForm->scrollController->scroll[0]->GetMax()
				- this->notepadForm->scrollController->scroll[0]->GetPageSize();
			//5.11.3 distance가 maxScrollPos보다 크면
			if (distance > maxScrollPos)
			{
				//5.11.3.1 distance에 maxScrollPos를 대입한다.
				distance = maxScrollPos;
			}
			//5.11.4 수평스크롤의 현재위치를 distance로 이동시킨다.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//5.11.5 수평스크롤바의 Thumb를 수평스크롤의 현재 위치로 이동시킨다.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
	}
}

//소멸자
LeftArrowKeyAction::~LeftArrowKeyAction()
{

}