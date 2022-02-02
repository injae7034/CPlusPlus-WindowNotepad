#include "CtrlEndKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"

//디폴트생성자
CtrlEndKeyAction::CtrlEndKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	Long distance;
	//1. 메모장의 제일 마지막으로 캐럿의 세로 위치를 이동시킨다.
	Long lastIndex = this->notepadForm->note->Last();
	//2. 메모장의 현재 줄을 변경한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(lastIndex);
	//3. 현재 줄의 맨 마지막으로 캐럿의 가로 위치를 이동시킨다.
	this->notepadForm->current->Last();
	//4. 현재 줄까지 총 세로 길이를 구한다.(캐럿의 세로 위치를 구한다.)
	Long caretYPos = this->notepadForm->textExtent->GetHeight() * (lastIndex + 1);
	//5. 수직스크롤이 이동할 수 있는 마지막으로 이동시킨다.
	Long maxVScroll = caretYPos - this->notepadForm->scrollController->
		scroll[1]->GetPageSize();
	this->notepadForm->scrollController->scroll[1]->Move(maxVScroll);
	//6. 수평스크롤바의 수평스크롤을 이동시킨다.
	this->notepadForm->SetScrollPos(SB_VERT,
		this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
	//7. 현재 줄의 캐럿의 가로 위치를 구한다.
	Long caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
		current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
	//8. 수평스크롤이 이동하기전에 수평스크롤의 현재위치(HScrollPos)를 구한다.
	Long HScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
	//9. 수평스크롤의 현재위치(HScrollPos)와 현재화면의 가로길이의 합을 구한다.
	Long HorizontalSum = HScrollPos + this->notepadForm->
		scrollController->scroll[0]->GetPageSize();
	//10. 캐럿의 현재 가로위치(caretXPos)가 수평스크롤의 현재위치(HScrollPos)보다
		//메모장에서 앞에 있으면
	if (caretXPos < HScrollPos)
	{
		//10.1 수평스크롤이 이동할 위치(distance)를 구한다.
		distance = caretXPos - (this->notepadForm->
			scrollController->scroll[0]->GetPageSize() / 5);
		//10.2 distance가 음수이면
		if (distance < 0)
		{
			//10.2.1 distance를 0으로 바꿔준다.
			distance = 0;
		}
		//10.3 수평스크롤의 현재위치를 distance로 이동시킨다.
		this->notepadForm->scrollController->scroll[0]->Move(distance);
		//10.4 수평스크롤바의 Thumb를 수평스크롤의 현재 위치로 이동시킨다.
		this->notepadForm->SetScrollPos(SB_HORZ,
			this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
	}
	//11. 캐럿의 현재 가로위치(caretXPos)가 수평스크롤의 현재위치(HScrollPos)와
	//현재화면의 가로길이의 합(HorizontalSum)보다 메모장에서 뒤에 있으면
	else if (caretXPos > HorizontalSum)
	{
		//11.1 수평스크롤이 이동할 위치(distance)를 구한다.
		distance = caretXPos - (this->notepadForm->
			scrollController->scroll[0]->GetPageSize() / 5 * 4);
		//11.2 수평스크롤바의 Thumb의 최대 이동값(maxScrollPos)를 구한다.
		Long maxScrollPos = this->notepadForm->scrollController->scroll[0]->GetMax()
			- this->notepadForm->scrollController->scroll[0]->GetPageSize();
		//11.3 distance가 maxScrollPos보다 크면
		if (distance > maxScrollPos)
		{
			//11.3.1 distance에 maxScrollPos를 대입한다.
			distance = maxScrollPos;
		}
		//11.4 수평스크롤의 현재위치를 distance로 이동시킨다.
		this->notepadForm->scrollController->scroll[0]->Move(distance);
		//11.5 수평스크롤바의 Thumb를 수평스크롤의 현재 위치로 이동시킨다.
		this->notepadForm->SetScrollPos(SB_HORZ,
			this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
	}
}

//소멸자
CtrlEndKeyAction::~CtrlEndKeyAction()
{

}