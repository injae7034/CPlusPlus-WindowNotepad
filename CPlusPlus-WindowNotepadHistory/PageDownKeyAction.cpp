#include "PageDownKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"

//디폴트생성자
PageDownKeyAction::PageDownKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void PageDownKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

#if 0
	Long HScrollPos = 0;//가로스크롤의 현재위치
	Long VScrollPos = 0;//세로스크롤의 현재위치
	Long beginCaretYPos = 0;//캐럿의 세로위치 시작지점
	Long endCaretYPos = 0;//캐럿의 세로위치 종료지점
	Long caretXPos = 0;//캐럿의 가로위치
	Long VerticalSum = 0;//세로스크롤의 현재위치 + 현재화면의 세로길이
	Long HorizontalSum = 0;//가로스크롤의 현재위치 + 현재화면의 가로길이
	Long distance = 0;//스크롤이 이동할 위치
	Long maxScrollPos = 0;//스크롤바의 Thumb이 이동할 수 최대 위치
	Long previousRowIndex = 0;//페이지 다음으로 이동하기 전에 현재 줄의 위치
	Long currentRowIndex = 0;//페이지 다음으로 이동하기 전에 현재 줄의 위치
	Long previousCaretIndex = 0;// 페이지 다음으로 이동하기 전에 캐럿의 현재 가로 위치

	//1. 페이지 다음으로 이동하기 전에 캐럿의 현재 세로 위치를 저장한다.
	previousRowIndex = this->notepadForm->note->GetCurrent();
	//2. 페이지 다음으로 이동하기 전에 캐럿의 현재 가로 위치를 저장한다.
	previousCaretIndex = this->notepadForm->current->GetCurrent();
	//3. 현재 화면의 세로 길이를 구한다.
	Long pageHeight = this->notepadForm->scrollController->scroll[1]->GetPageSize();
	//4. 한 페이지에 줄의 개수를 구한다.
	Long rowCountOfPage = this->notepadForm->scrollController->scroll[1]->GetPageSize()
		/ pageHeight;
	//5. 현재 메모장의 총 줄의 개수를 구한다.
	Long rowCountOfNote = this->notepadForm->note->GetLength();
	//6. 현재 캐럿의 위치에서 페이지 단위로 이동할 거리를 정한다.
	currentRowIndex = previousRowIndex;//페이지 단위로 이동하기 전에 현재 줄의 위치를 저장한다.
	Long i = 0;
	while (i < rowCountOfPage && currentRowIndex < rowCountOfNote)
	{
		i++;
		currentRowIndex = this->notepadForm->note->Next();
	}
	//4. 이동하기 전 캐럿의 세로 위치와 이동한 후 캐럿의 세로 위치가 서로 다르면(실질적으로 이동을 했으면)
	if (previousRowIndex != currentRowIndex)
	{

	}
#endif
#if 0
	Long distance;
	//1. 노트에서 현재 줄의 위치를 구한다.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//2. 현재 줄에서 캐럿의 세로 범위(시작과 끝)을 구한다.(beginCaretYPos, endCaretYPos)
	Long beginCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
	Long endCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);
	//4.6 수직스크롤을 이동하기 전에 현재 수직스크롤의 VScrollPos(스크롤의 현재 위치)를 구한다.
	Long VScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
	//4.7 세로스크롤의 현재위치(VScrollPos)와 현재화면의 세로길이의 합을 구한다.
	Long VerticalSum = VScrollPos + this->notepadForm->
		scrollController->scroll[1]->GetPageSize();
	//4.8 캐럿의 세로 위치 시작점(beginCaretYPos)이 
	//수직스크롤의 현재위치(VScrollPos)보다 메모장에서 위에 있으면
	if (beginCaretYPos < VScrollPos)
	{
		//4.8.1 수직스크롤이 이동할 위치(distance)를 캐럿의 세로 위치 시작점으로 정한다.
		distance = beginCaretYPos;
		//4.8.2 수직스크의 현재 위치를 distance로 이동시킨다.
		this->notepadForm->scrollController->scroll[1]->Move(distance);
		//4.8.3 수직스크롤바의 Thumb를 수직스크롤의 현재위치로 이동시킨다.
		this->notepadForm->SetScrollPos(SB_VERT,
			this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
	}
	//4.9 캐럿의 세로위치 끝지점(endCaretYPos)이 수직스크롤의 현재 위치(VScrollPos)와 
	//현재화면의 세로길이의 합(VerticalSum)보다 메모장에서 아래에 있으면
	else if (endCaretYPos > VerticalSum)
	{
		//4.9.1 수직스크롤이 이동할 범위(distance)를 구한다.
		//캐럿의 세로 위치 마지막 지점에서 화면의 세로길이의 차로 정한다.
		distance = endCaretYPos - this->notepadForm->
			scrollController->scroll[1]->GetPageSize();
		//4.9.2 아래로 이동할 때 위에 글자가 안잘리도록 하기 위해서 여백을 구한다.
		Long pageSize = this->notepadForm->scrollController->scroll[1]->GetPageSize();
		Long letterHeight = this->notepadForm->textExtent->GetHeight();
		Long blank = pageSize - (pageSize / letterHeight * letterHeight);
		//4.9.3 수직스크롤의 현재위치를 distance + 여백(blank)으로 이동시킨다.
		this->notepadForm->scrollController->scroll[1]->Move(distance + blank);
		//4.9.3 수직스크롤바의 Thumb를 수직스크롤의 현재 위치로 이동시킨다.
		this->notepadForm->SetScrollPos(SB_VERT,
			this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
	}
#endif
#if 0
	Long distance;
	//1. 노트에서 현재 줄의 위치를 구한다.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//2. 현재 줄에서 캐럿의 세로 범위(시작과 끝)을 구한다.(beginCaretYPos, endCaretYPos)
	Long beginCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
	Long endCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);
	//3. 수직스크롤을 이동하기 전에 현재 수직스크롤의 VScrollPos(스크롤의 현재 위치)를 구한다.
	Long VScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
	//4. 세로스크롤의 현재위치(VScrollPos)와 현재화면의 세로길이의 합을 구한다.
	Long VerticalSum = VScrollPos + this->notepadForm->
		scrollController->scroll[1]->GetPageSize();
	//5. 캐럿의 세로 위치 시작점(beginCaretYPos)이 
	//수직스크롤의 현재위치(VScrollPos)보다 메모장에서 위에 있으면
	if (beginCaretYPos < VScrollPos)
	{
		//5.1 수직스크롤이 이동할 위치(distance)를 캐럿의 세로 위치 시작점으로 정한다.
		distance = endCaretYPos;
		//5.2 수직스크의 현재 위치를 distance로 이동시킨다.
		this->notepadForm->scrollController->scroll[1]->Move(distance);
		//5.3 수직스크롤바의 Thumb를 수직스크롤의 현재위치로 이동시킨다.
		this->notepadForm->SetScrollPos(SB_VERT,
			this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
	}
	//4.9 캐럿의 세로위치 끝지점(endCaretYPos)이 수직스크롤의 현재 위치(VScrollPos)와 
	//현재화면의 세로길이의 합(VerticalSum)보다 메모장에서 아래에 있으면
	else if (endCaretYPos > VerticalSum)
	{
		//4.9.1 수직스크롤이 이동할 범위(distance)를 구한다.
		//캐럿의 세로 위치 마지막 지점에서 화면의 세로길이의 차로 정한다.
		distance = endCaretYPos - this->notepadForm->
			scrollController->scroll[1]->GetPageSize();
		//4.9.2 아래로 이동할 때 위에 글자가 안잘리도록 하기 위해서 여백을 구한다.
		Long pageSize = this->notepadForm->scrollController->scroll[1]->GetPageSize();
		Long letterHeight = this->notepadForm->textExtent->GetHeight();
		Long blank = pageSize - (pageSize / letterHeight * letterHeight);
		//4.9.3 수직스크롤의 현재위치를 distance + 여백(blank)으로 이동시킨다.
		this->notepadForm->scrollController->scroll[1]->Move(distance + blank);
		//4.9.3 수직스크롤바의 Thumb를 수직스크롤의 현재 위치로 이동시킨다.
		this->notepadForm->SetScrollPos(SB_VERT,
			this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
	}
#endif
#if 0
	Long distance;
	//1. 노트에서 현재 줄의 위치를 구한다.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//2. 현재 줄에서 캐럿의 세로 범위(시작과 끝)을 구한다.(beginCaretYPos, endCaretYPos)
	Long beginCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
	Long endCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);
	//3. 수직스크롤을 이동하기 전에 현재 수직스크롤의 VScrollPos(스크롤의 현재 위치)를 구한다.
	Long VScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
	//4. 세로스크롤의 현재위치(VScrollPos)와 현재화면의 세로길이의 합을 구한다.
	Long VerticalSum = VScrollPos + this->notepadForm->
		scrollController->scroll[1]->GetPageSize();
	//5. 캐럿의 세로 위치 시작점(beginCaretYPos)이 
	//수직스크롤의 현재위치(VScrollPos)보다 메모장에서 위에 있으면
	if (beginCaretYPos < VScrollPos)
	{
		//5.1 수직스크롤이 이동할 위치(distance)를 캐럿의 세로 위치 시작점으로 정한다.
		distance = beginCaretYPos;
		//5.2 수직스크의 현재 위치를 distance로 이동시킨다.
		this->notepadForm->scrollController->scroll[1]->Move(distance);
		//5.3 수직스크롤바의 Thumb를 수직스크롤의 현재위치로 이동시킨다.
		this->notepadForm->SetScrollPos(SB_VERT,
			this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
	}
	//4.9 캐럿의 세로위치 끝지점(endCaretYPos)이 수직스크롤의 현재 위치(VScrollPos)와 
	//현재화면의 세로길이의 합(VerticalSum)보다 메모장에서 아래에 있으면
	else if (endCaretYPos > VerticalSum)
	{
		//4.9.1 수직스크롤이 이동할 범위(distance)를 구한다.
		//캐럿의 세로 위치 마지막 지점에서 화면의 세로길이의 차로 정한다.
		distance = endCaretYPos - this->notepadForm->
			scrollController->scroll[1]->GetPageSize();
		//4.9.2 아래로 이동할 때 위에 글자가 안잘리도록 하기 위해서 여백을 구한다.
		Long pageSize = this->notepadForm->scrollController->scroll[1]->GetPageSize();
		Long letterHeight = this->notepadForm->textExtent->GetHeight();
		Long blank = pageSize - (pageSize / letterHeight * letterHeight);
		//4.9.3 수직스크롤의 현재위치를 distance + 여백(blank)으로 이동시킨다.
		this->notepadForm->scrollController->scroll[1]->Move(distance + blank);
		//4.9.3 수직스크롤바의 Thumb를 수직스크롤의 현재 위치로 이동시킨다.
		this->notepadForm->SetScrollPos(SB_VERT,
			this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
	}
#endif

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

	//캐럿을 이동시키기 전에 먼저 현재 캐럿이 있는 곳으로 스크롤을 이동시킨다.
	//1. 캐럿의 현재 세로 위치를 구한다.
	currentRowIndex = this->notepadForm->note->GetCurrent();
	//2. caret 이동 후에 캐럿의 세로 범위(시작과 끝)을 구한다.(beginCaretYPos, endCaretYPos)
	beginCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
	endCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);
	//3. 수직스크롤을 이동하기 전에 현재 수직스크롤의 VScrollPos(스크롤의 현재 위치)를 구한다.
	VScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
	//4. 수직스크롤의 현재위치(VScrollPos)와 현재화면의 세로길이의 합을 구한다.
	VerticalSum = VScrollPos + this->notepadForm->
		scrollController->scroll[1]->GetPageSize();
	//5. 캐럿의 세로 위치 시작점(beginCaretYPos)이 
	//수직스크롤의 현재위치(VScrollPos)보다 메모장에서 위에 있으면
	if (beginCaretYPos < VScrollPos)
	{
		//5.1 수직스크롤이 이동할 위치(distance)를 캐럿의 세로 위치 시작점으로 정한다.
		distance = beginCaretYPos;
		//5.2 수직스크의 현재 위치를 distance로 이동시킨다.
		this->notepadForm->scrollController->scroll[1]->Move(distance);
		//5.3 수직스크롤바의 Thumb를 수직스크롤의 현재위치로 이동시킨다.
		this->notepadForm->SetScrollPos(SB_VERT,
			this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
	}
	//6. 캐럿의 세로위치 끝지점(endCaretYPos)이 수직스크롤의 현재 위치(VScrollPos)와 
	//현재화면의 세로길이의 합(VerticalSum)보다 메모장에서 아래에 있으면
	else if (endCaretYPos > VerticalSum)
	{
		//6.1 수직스크롤이 이동할 범위(distance)를 구한다.
		//캐럿의 세로 위치 마지막 지점에서 화면의 세로길이의 차로 정한다.
		distance = endCaretYPos - this->notepadForm->
			scrollController->scroll[1]->GetPageSize();
		//6.2 아래로 이동할 때 위에 글자가 안잘리도록 하기 위해서 여백을 구한다.
		Long pageSize = this->notepadForm->scrollController->scroll[1]->GetPageSize();
		Long letterHeight = this->notepadForm->textExtent->GetHeight();
		Long blank = pageSize - (pageSize / letterHeight * letterHeight);
		//6.3 수직스크롤의 현재위치를 distance + 여백(blank)으로 이동시킨다.
		this->notepadForm->scrollController->scroll[1]->Move(distance + blank);
		//6.4 수직스크롤바의 Thumb를 수직스크롤의 현재 위치로 이동시킨다.
		this->notepadForm->SetScrollPos(SB_VERT,
			this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
	}
	//7. 캐럿이 이동한 후에 캐럿의 현재 가로위치(caretXPos)를 구한다.
	caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
		current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
	//8. 수평스크롤이 이동하기전에 수평스크롤의 현재위치(HScrollPos)를 구한다.
	HScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
	//9. 수평스크롤의 현재위치(HScrollPos)와 현재화면의 가로길이의 합을 구한다.
	HorizontalSum = HScrollPos + this->notepadForm->
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
		maxScrollPos = this->notepadForm->scrollController->scroll[0]->GetMax()
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

	//스크롤을 캐럿이 있는 곳으로 이동시킨 다음에 다시 스크롤을 페이지단위로 이동시킨다.
	//12. 페이지 단위로 이동하기 전에 수직스크롤의 현재 위치를 저장한다.
	Long previousVPos = this->notepadForm->scrollController->scroll[1]->GetCurrentPos();
	//13. 수직스크롤을 다음 페이지로 이동시키고 이동된 수직스크롤의 현재 위치를 반환받는다.
	Long currentVPos = this->notepadForm->scrollController->scroll[1]->PageNext();
	//14. 수직스크롤에 이동이 있으면(previousVPos와 currentVPos가 다르면 이동이 있었다는 뜻임)
	if (previousVPos != currentVPos)
	{
		//14.1 수직스크롤바의 수직스크롤을 수직스크롤의 현재 위치로 이동시킨다.
		this->notepadForm->SetScrollPos(SB_VERT,
			this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
		//14.2 메모장에서 캐럿이 이동할 거리를 구한다.
		distance = currentVPos - previousVPos;
		//14.3 캐럿이 이동할 줄의 개수를 구한다.
		Long MovingRowCount = distance / this->notepadForm->textExtent->GetHeight();
		//14.4 이동하기 전에 현재 줄의 위치를 구한다.
		previousRowIndex = this->notepadForm->note->GetCurrent();
		//14.5 이동하기 전의 캐럿의 가로 위치를 구한다.
		Long previousCaretIndex = this->notepadForm->current->GetCurrent();
		//14.6 캐럿의 세로 위치를 이동시킨다.
		currentRowIndex = previousRowIndex + MovingRowCount;
		currentRowIndex = this->notepadForm->note->Move(currentRowIndex);
		//14.7 이동한 후의 현재줄을 변경한다.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
		//14.8 이동하기 전 캐럿의 가로 위치가 0이 아니고 이동한 후 현재줄의 글자개수가 0이 아니면
		if (previousCaretIndex != 0 && this->notepadForm->current->GetLength() != 0)
		{
			//14.8.1 이동하기 전 줄의 텍스트 폭을 구한다.
			Long previousRowTextWidth = this->notepadForm->textExtent->
				GetTextWidth(this->notepadForm->note->GetAt(previousRowIndex)
					->GetPartOfContent(previousCaretIndex));
			//14.8.2 캐럿의 현재 가로 위치를 처음으로 이동시킨다.
			this->notepadForm->current->First();//캐럿의 가로 위치를 맨 처음(0)으로 보냄
			//14.8.3 첫번째 글자를 읽기 위해 캐럿을 1 증가시킨다.
			Long i = this->notepadForm->current->Next();
			//14.8.4 현재 줄의 택스트의 폭을 구한다.
			Long currentRowTextWidth = this->notepadForm->textExtent->
				GetTextWidth(this->notepadForm->current->GetPartOfContent(i).c_str());
			//14.8.5 i(캐럿의 현재 가로위치)가 length(현재줄의 글자개수)보다 작고
			//현재 줄의 텍스트 크기가 이동하기 전 줄의 텍스트 크기보다 작은동안 반복한다.
			while (i < this->notepadForm->current->GetLength()
				&& currentRowTextWidth < previousRowTextWidth)
			{
				//14.8.5.1 i(캐럿의 현재 가로위치)를 증가시킨다
				//(캐럿의 가로 위치를 다음 칸으로 이동시킨다.).
				i = this->notepadForm->current->Next();
				//14.8.5.2 현재 줄의 캐럿의 가로 위치까지의 택스트 폭을 구한다.
				currentRowTextWidth = this->notepadForm->textExtent->
					GetTextWidth(this->notepadForm->current->GetPartOfContent(i).c_str());
			}
			//14.8.6 현재 줄의 텍스트 폭에서 이전 줄의 텍스트 폭을 뺀다.
			Long difference = currentRowTextWidth - previousRowTextWidth;
			//14.8.7 차이가 0보다 크면(현재줄의 텍스트를 다 읽지 못한 경우)
			if (difference > 0)
			{
				//14.8.7.1 캐럿의 현재 가로위치 이전의 글자 폭을 구한다.
				Long letterWidth = this->notepadForm->textExtent->
					GetTextWidth(this->notepadForm->current->GetAt(i - 1)->GetContent());
				Long halfLetterSize = letterWidth / 2;
				//14.8.7.2 차이가 읽은 글자크기의 절반보다 같거나 크면
				if (difference >= halfLetterSize)
				{
					//14.8.7.2.1 캐럿의 현재 가로 위치를 이전으로 이동한다.
					this->notepadForm->current->Previous();
				}
			}
		}
		//14.9 캐럿의 이전 가로 위치가 0 또는 캐럿의 현재 줄의 글자개수가 0개이면
		else
		{
			//14.9.1 현재 캐럿의 가로 위치를 0으로 이동시킨다.
			this->notepadForm->current->First();
		}

		//14.10 캐럿이 이동한 후에 캐럿의 현재 가로위치(caretXPos)를 구한다.
		caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
			current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
		//14.11 수평스크롤이 이동하기전에 수평스크롤의 현재위치(HScrollPos)를 구한다.
		HScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
		//14.12 수평스크롤의 현재위치(HScrollPos)와 현재화면의 가로길이의 합을 구한다.
		HorizontalSum = HScrollPos + this->notepadForm->
			scrollController->scroll[0]->GetPageSize();
		//14.13 캐럿의 현재 가로위치(caretXPos)가 수평스크롤의 현재위치(HScrollPos)보다
			//메모장에서 앞에 있으면
		if (caretXPos < HScrollPos)
		{
			//14.13.1 수평스크롤이 이동할 위치(distance)를 구한다.
			distance = caretXPos - (this->notepadForm->
				scrollController->scroll[0]->GetPageSize() / 5);
			//14.13.2 distance가 음수이면
			if (distance < 0)
			{
				//14.13.2.1 distance를 0으로 바꿔준다.
				distance = 0;
			}
			//14.13.3 수평스크롤의 현재위치를 distance로 이동시킨다.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//14.13.4 수평스크롤바의 Thumb를 수평스크롤의 현재 위치로 이동시킨다.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
		//14.14 캐럿의 현재 가로위치(caretXPos)가 수평스크롤의 현재위치(HScrollPos)와
		//현재화면의 가로길이의 합(HorizontalSum)보다 메모장에서 뒤에 있으면
		else if (caretXPos > HorizontalSum)
		{
			//14.14.1 수평스크롤이 이동할 위치(distance)를 구한다.
			distance = caretXPos - (this->notepadForm->
				scrollController->scroll[0]->GetPageSize() / 5 * 4);
			//14.14.2 수평스크롤바의 Thumb의 최대 이동값(maxScrollPos)를 구한다.
			maxScrollPos = this->notepadForm->scrollController->scroll[0]->GetMax()
				- this->notepadForm->scrollController->scroll[0]->GetPageSize();
			//14.14.3 distance가 maxScrollPos보다 크면
			if (distance > maxScrollPos)
			{
				//14.14.3.1 distance에 maxScrollPos를 대입한다.
				distance = maxScrollPos;
			}
			//14.14.4 수평스크롤의 현재위치를 distance로 이동시킨다.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//14.14.5 수평스크롤바의 Thumb를 수평스크롤의 현재 위치로 이동시킨다.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
	}
}

//소멸자
PageDownKeyAction::~PageDownKeyAction()
{

}