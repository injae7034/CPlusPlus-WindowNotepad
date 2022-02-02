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
	//1. 현재 화면의 크기를 구한다.
	CRect rect;
	this->notepadForm->GetClientRect(&rect);
	Long pageWidth = rect.Width();
	Long pageHeight = rect.Height();
	Long HScrollPos = 0;
	Long VScrollPos = 0;
	Long beginCaretYPos = 0;
	Long endCaretYPos = 0;
	Long caretXPos = 0;
	Long sum = 0;
	Long distance = 0;
	Long previousRowIndex = 0;
	Long currentRowIndex = 0;
	//2. 이전으로 이동하기 전에 캐럿의 현재 가로위치를 저장한다.
	Long previousIndex = this->notepadForm->current->GetCurrent();
	//3. 이전으로 이동하고 현재 캐럿의 가로위치를 반환받는다.
	Long currentIndex = this->notepadForm->current->Previous();
	//4. 캐럿의 이전 가로위치와 캐럿의 현재 가로위치가 같으면(캐럿의 이동이 없었으면)
	//이동이 없었다는 말은 캐럿이 현재 줄의 제일 처음에 위치해서 더이상 이동할 곳이 없다는 뜻이다.
	if (previousIndex == currentIndex)
	{
		//4.1 이전으로 이동하기 전에 캐럿의 현재 세로 위치를 저장한다.
		previousRowIndex = this->notepadForm->note->GetCurrent();
		//4.2 캐럿의 현재 세로 위치를 이전 줄로 이동시킨다.
		currentRowIndex = this->notepadForm->note->Previous();
		//4.3 캐럿의 이전 세로 위치와 캐럿의 현재 새로 위치가 서로 다르면
		if (previousRowIndex != currentRowIndex)
		{
			//4.3.1 캐럿의 현재 줄을 변경한다.
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
			//4.3.2 캐럿의 현재 가로 위치를 변경한다.
			this->notepadForm->current->Last();
			//4.3.3. 캐럿의 이동 후에 caretYPos(캐럿의 세로 위치)를 구한다.
			//currentRowIndex는 0이 시작이라서 평균높이에 줄의 개수를 곱해야 하므로 +1을 해줘야 한다. 
			endCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);
			//4.3.4 수직스크롤을 이동하기 전에 현재 수직스크롤의 VScrollPos(스크롤의 현재 위치)를 구한다.
			VScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
			//4.3.5 caretYPos가 VScrollPos보다 크면
			if (endCaretYPos > VScrollPos)
			{
				//4.3.5.1 현재화면의 세로 길이와 수직스크롤의 현재 위치의 합을 구한다.
				sum = pageHeight + VScrollPos;
				//4.3.5.2 caretYPos가 sum보다 크면
				if (endCaretYPos > sum)
				{
					//4.3.5.2.1 수직스크롤이 이동할 범위(distance)를 구한다.
					distance = endCaretYPos - pageHeight;
					//4.3.5.2.2 distance만큼 현재 수직스크롤을 이동시킨다.
					this->notepadForm->scrollController->scroll[1]->Move(distance);
					//4.3.5.2.3 수직스크롤바의 수직스크롤을 이동시킨다.
					this->notepadForm->SetScrollPos(SB_VERT,
						this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
				}
			}
			//4.3.6 caretYPos가 VScrollPos보다 작으면
			else if (endCaretYPos < VScrollPos)
			{
				//4.3.6.1 수직스크롤이 이동할 범위(distance)를 구한다.
				//현재 캐럿의 YPos에서 위에 줄까지의 길이를 구한다.
				distance = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
				//4.3.6.2 distance만큼 현재 수직스크롤을 이동시킨다.
				this->notepadForm->scrollController->scroll[1]->Move(distance);
				//4.3.6.3 수직스크롤바의 수직스크롤을 이동시킨다.
				this->notepadForm->SetScrollPos(SB_VERT,
					this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
			}
			//4.3.7 캐럿이 이동한 후에 캐럿의 현재 가로위치(caretXPos)를 구한다.
			caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
				current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
			//4.3.8 스크롤이 이동하기전에 수평스크롤의 현재위치(HScrollPos)를 구한다.
			HScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
			//4.3.9 caretXPos가 HScrollPos보다 크면
			if (caretXPos > HScrollPos)
			{
				//4.3.9.1 현재화면의 가로 길이와 수평스크롤의 현재 위치의 합을 구한다.
				sum = pageWidth + HScrollPos;
				//4.3.9.2 caretXPos가 sum보다 크면
				if (caretXPos > sum)
				{
					//4.3.9.2.1 수평스크롤이 이동할 범위(distance)를 구한다.
					distance = caretXPos - pageWidth;
					//4.3.9.2.2 distance만큼 수평스크롤을 이동시킨다.
					this->notepadForm->scrollController->scroll[0]->Move(distance + 2);
					//4.3.9.2.3 수평스크롤바의 수평스크롤을 이동시킨다.
					this->notepadForm->SetScrollPos(SB_HORZ,
						this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
				}
			}
			//4.3.10 caretXPos가 HScrollPos보가 작으면
			else if (caretXPos < HScrollPos)
			{
				//4.3.10.1 수평스크롤이 이동할 범위(distance)를 구한다.
				distance = caretXPos - pageWidth;
				//4.3.10.2 distance가 음수이면
				if (distance < 0)
				{
					//4.3.10.2.1 distance에 0을 대입해준다.
					distance = 0;
				}
				//4.3.10.3 distance만큼 수평스크롤을 이동시킨다.
				this->notepadForm->scrollController->scroll[0]->Move(distance + 2);
				//4.3.10.4 수평스크롤바의 수평스크롤을 이동시킨다.
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
		//5.2 caret 이동 후에 캐럿의 세로 위치(caretYPos)를 구한다.
		//currentRowIndex는 0이 시작이라서 평균높이에 줄의 개수를 곱해야 하므로 +1을 해줘야 한다. 
		beginCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
		endCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);
		//5.3 수직스크롤을 이동하기 전에 현재 수직스크롤의 VScrollPos(스크롤의 현재 위치)를 구한다.
		VScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
		//5.4 caretYPos가 VScrollPos보다 크면
		if (beginCaretYPos < VScrollPos )
		{
			//5.4.1 현재화면의 세로 길이와 수직스크롤의 현재 위치의 합을 구한다.
			sum = pageHeight + VScrollPos;
			//5.4.2 caretYPos가 sum보다 크면
			if (beginCaretYPos > sum)
			{
				//5.4.2.1 수직스크롤이 이동할 범위(distance)를 구한다.
				distance = beginCaretYPos - pageHeight;
				//5.4.2.2 distance만큼 현재 수직스크롤을 이동시킨다.
				this->notepadForm->scrollController->scroll[1]->Move(distance);
				//5.4.2.3 수직스크롤바의 수직스크롤을 이동시킨다.
				this->notepadForm->SetScrollPos(SB_VERT,
					this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
			}
		}
		//5.5 caretYPos가 VScrollPos보다 작으면
		else if (endCaretYPos > VScrollPos + pageHeight)
		{
			//5.5.1 수직스크롤이 이동할 범위(distance)를 구한다.
			//현재 캐럿의 YPos에서 위에 줄까지의 길이를 구한다.
			distance = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
			//5.5.2 distance만큼 현재 수직스크롤을 이동시킨다.
			this->notepadForm->scrollController->scroll[1]->Move(distance);
			//5.5.3 수직스크롤바의 수직스크롤을 이동시킨다.
			this->notepadForm->SetScrollPos(SB_VERT,
				this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
		}
		//5.6 caret이 이동하기 전의 캐럿의 가로 위치(caretXPos)를 구한다.
		//caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
		//	current->GetPartOfContent(this->notepadForm->current->GetCurrent() + 1));
		//4.3.7 캐럿이 이동한 후에 캐럿의 현재 가로위치(caretXPos)를 구한다.
		caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
			current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
		//5.6 스크롤이 이동하기전에 수평스크롤의 현재위치(HScrollPos)를 구한다.
		HScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
		//5.7 caretXPos가 HScrollPos보다 크면
		if (caretXPos > HScrollPos)
		{
			//5.7.1 현재화면의 가로 길이와 수평스크롤의 현재 위치의 합을 구한다.
			sum = pageWidth + HScrollPos;
			//5.7.2 caretXPos가 sum보다 크면
			if (caretXPos > sum)
			{
				//5.7.2.1 수평스크롤이 이동할 범위(distance)를 구한다.
				distance = caretXPos - (pageWidth / 5);
				//distance = HScrollPos - (pageWidth / 5);
				//5.7.2.2 distance만큼 수평스크롤을 이동시킨다.
				this->notepadForm->scrollController->scroll[0]->Move(distance);
				//5.7.2.3 수평스크롤바의 수평스크롤을 이동시킨다.
				this->notepadForm->SetScrollPos(SB_HORZ,
					this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
			}
		}
		//5.8 caretXPos가 HScrollPos보가 작으면
		else if (caretXPos < HScrollPos)
		{
			//5.8.1 수평스크롤이 이동할 범위(distance)를 구한다.
			distance = caretXPos - (pageWidth / 5);
			//5.8.2 distance가 음수이면
			if (distance < 0)
			{
				//5.8.2.1 distance에 0을 대입해준다.
				distance = 0;
			}
			//5.8.3 distance만큼 수평스크롤을 이동시킨다.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//5.8.4 수평스크롤바의 수평스크롤을 이동시킨다.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
	}
}

//소멸자
LeftArrowKeyAction::~LeftArrowKeyAction()
{

}