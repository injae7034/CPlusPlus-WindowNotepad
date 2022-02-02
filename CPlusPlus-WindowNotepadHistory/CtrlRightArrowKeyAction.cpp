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
	Long caretXPos = 0;
	Long caretYPos = 0;
	Long HScrollPos = 0;
	Long YScrollPos = 0;
	Long partScrollWidth = 0;
	Long partScrollHeight = 0;
	Long distance = 0;
	//1. 현재 화면의 크기를 구한다.
	CRect rect;
	this->notepadForm->GetClientRect(&rect);
	Long pageWidth = rect.Width();
	Long pageHeight = rect.Height();
	//2. 이동하기 전에 현재 줄의 위치를 구한다.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//3. Note의 NextWord를 호출한다.
	Long currentCaretindex = this->notepadForm->note->NextWord();
	//4. 이동한 후에 현재 줄의 위치를 구한다.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//5. 현재줄을 변경한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	//6. 현재줄의 캐럿의 가로 위치를 변경한다.
	Long i = this->notepadForm->current->First();
	while (i < currentCaretindex)
	{
		i = this->notepadForm->current->Next();
	}
	//7. 이동한 뒤에 현재 캐럿의 세로 위치가 그대로이면(줄이 바뀌지X, 한 줄 내에서 이동함)
	//줄이 바뀌지 않았기 때문에 수직스크롤을 신경쓸 필요가 없고, 수평스크롤의 이동만 신경쓰면 됨)
	if (previousRowIndex == currentRowIndex)
	{
		//7.1 현재 캐럿의 가로 위치를 구한다.
		caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
			current->GetPartOfContent(currentCaretindex));
		//7.2 수평스크롤의 현재 위치를 구한다.
		HScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
		//7.3 현재 화면의 가로길이와 수평스크롤의 현재 위치와의 합을 구한다.
		partScrollWidth = pageWidth + HScrollPos;
		//7.4 현재 캐럿의 가로위치가 현재화면의 가로길이와 수평스크롤의 현재위치의 합보다 크면
		if (caretXPos > partScrollWidth)
		{
			//7.4.1 수평스크롤이 이동할 범위를 정한다.
			//아래 식대로 하면 누적이 없이 distance값이 일정하게 나오고 Move연산은 덧셈연산이 
			//아니라 해당위치로 이동하는 연산이다 그래서 distance의 값을 구할 때 애초에 누적이 된 값을
			//구해줘야함. 누적된 값을 구하기 위해서는 HScrollPos값은 빼지 않고 그대로 둬야
			//HScrollPos값에 careXPos와 pageWidth를 뺀 값이 누적이 되어 distance에 저장됨
			//HScrollPos까지 빼주면 HScrollPos가 0일때는 문제가 없지만 HScrollPos가 0이 아닐 경우
			//distance값이 누적이 없고 Move는 계속 일정한 위치에 머물게 된다.
			//distance = caretXPos - (pageWidth + HScrollPos) + (pageWidth / 5);

			distance = caretXPos - pageWidth + (pageWidth / 5);
			//distance = caretXPos + (pageWidth / 5);
			//7.4.2 수평스크롤을 이동시켜준다.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//7.4.3 수평스크롤바의 수평스크롤을 이동시킨다.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
	}
	//8. 이동한 뒤에 현재 캐럿의 세로 위치가 바뀌었으면(줄이 바뀜. 현재 줄에서 다음 줄로 이동함.)
	//줄에서 줄로 이동하므로 수직스크롤을 신경써야하며 줄바뀌고 가로이동도 있기 때문에 수평스크롤도 신경써야함)
	else
	{
		//8.1 캐럿의 세로 위치를 구한다.(글자높이 X 현재줄의 위치)
		caretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);
		//8.2 수직스크롤의 현재 위치를 구한다.
		YScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
		//8.3 현재화면의 세로길이와 수직스크롤의 현재위치의 합을 구한다.
		partScrollHeight = pageHeight + YScrollPos;
		//8.4 현재 캐럿의 세로 위치가 현재 화면의 세로길이와 수직스크롤의 현재위치보다 크면
		if (caretYPos > partScrollHeight)//partScrollHeight보다 커야 정확하게 수직스크롤이 이동가능함
		{
			//8.4.1 수직스크롤이 이동할 범위를 정한다.
			//distance = caretYPos - partScrollHeight;// 이렇게하면 누적이 없어서 제자리걸음함
			
			//지금 여기에 문제가있음!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			//distance = caretYPos - pageHeight;//이렇게해야 YScrollPos가 계속 누적된 값을 구함.
			distance = caretYPos - pageHeight;
			//8.4.2 수직스크롤을 이동시킨다.
			this->notepadForm->scrollController->scroll[1]->Move(distance);
			//8.4.3 수직스크롤바의 수직스크롤을 이동시킨다.
			this->notepadForm->SetScrollPos(SB_VERT,
				this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
		}
		//8.5 수평스크롤을 처음으로 이동시킨다.(줄이 바뀌었기 때문에)
		this->notepadForm->scrollController->scroll[0]->First();
		//8.6 수평스크롤바의 수평스크롤을 이동시킨다.
		this->notepadForm->SetScrollPos(SB_HORZ,
			this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		//8.7 현재 캐럿의 가로 위치를 구한다.
		caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
			current->GetPartOfContent(currentCaretindex));
		//8.8 현재 캐럿의 가로 위치가 현재 화면의 가로 길이보다 크면
		if (caretXPos > pageWidth)
		{
			//8.8.1 수평스크롤이 이동할 범위를 정한다.
			distance = caretXPos - pageWidth + (pageWidth / 5);
			//distance = caretXPos + (pageWidth / 5);
			//8.8.2 수평스크롤을 이동시켜준다.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//8.8.3 수평스크롤바의 수평스크롤을 이동시킨다.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
	}

}

//소멸자
CtrlRightArrowKeyAction::~CtrlRightArrowKeyAction()
{

}