#include "UpArrowKeyAction.h"
#include "Glyph.h"
#include "TextExtent.h"
#include "ScrollController.h"
#include "Scroll.h"

//디폴트생성자
UpArrowKeyAction::UpArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void UpArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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

	//1. 이전으로 이동하기 전에 캐럿의 현재 세로 위치를 저장한다.
	previousRowIndex = this->notepadForm->note->GetCurrent();
	//2. 이전으로 이동하기 전에 캐럿의 현재 가로 위치를 저장한다.
	Long previousCaretIndex = this->notepadForm->current->GetCurrent();
	//3. 캐럿의 현재 세로 위치를 이전 줄로 이동시킨다.
	currentRowIndex = this->notepadForm->note->Previous();
	//4. 이동하기 전 캐럿의 세로 위치와 이동한 후 캐럿의 세로 위치가 서로 다르면(실질적으로 이동을 했으면)
	if (previousRowIndex != currentRowIndex)
	{
		//4.1 현재 줄을 이동한 후 캐럿의 세로 위치가 있는 줄로 변경한다.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
		//4.2 이동하기 전 캐럿의 가로 위치가 0이 아니고 이동한 후 현재줄의 글자개수가 0이 아니면
		//(이동하기 전 캐럿의 가로 위치가 0이면 이동한 후의 현재 캐럿의 가로 위치도 무조건 0이고,
		//이동한 후 현재줄의 글자개수가 0이어도 현재 캐럿의 가로 위치는 무조건 0임,
		//따라서 그 이외의 경우의 수를 if안에서 처리하고 else에서 위의 2경우를 first로 처리함!)
		if (previousCaretIndex != 0 && this->notepadForm->current->GetLength() != 0)
		{
			//4.2.1 이동하기 전 줄의 텍스트 폭을 구한다.
			Long previousRowTextWidth = this->notepadForm->textExtent->
				GetTextWidth(this->notepadForm->note->GetAt(previousRowIndex)
				->GetPartOfContent(previousCaretIndex));
			//4.2.2 캐럿의 현재 가로 위치를 처음으로 이동시킨다.(현재 캐럿의 가로 위치가 어딘지 모르기때문에)
			//i가 0이 되면 GetPartOfContent에서 읽는 텍스트가 없기 때문에 i의 최소값은 1이 되어야함.
			//GetPartOfContent는 캐럿의 위치까지 있는 글자들을 읽는것임!
			//위 if 조건식에서 이미 현재 줄의 글자가 하나도 없는 경우는 걸러져서 들어왔기 때문에
			//현재줄의 글자수는 최소 1개이상은 있는 경우에 대해서 처리함.
			this->notepadForm->current->First();//캐럿의 가로 위치를 맨 처음(0)으로 보냄(원위치)
			//4.2.3 첫번째 글자를 읽기 위해 캐럿을 한칸이동시킨다.
			Long i = this->notepadForm->current->Next();
			//4.2.4 현재 줄의 택스트의 폭을 구한다.
			Long currentRowTextWidth = this->notepadForm->textExtent->
				GetTextWidth(this->notepadForm->current->GetPartOfContent(i).c_str());
			//4.2.5 i(캐럿의 현재 가로위치)가 length(현재줄의 글자개수)보다 작고
			//현재 줄의 텍스트 크기가 이동하기 전 줄의 텍스트 크기보다 작은동안 반복한다.
			while (i < this->notepadForm->current->GetLength()
				&& currentRowTextWidth < previousRowTextWidth)
			{
				//4.2.5.1 i(캐럿의 현재 가로위치)를 증가시킨다
				//(캐럿의 가로 위치를 다음 칸으로 이동시킨다.).
				i = this->notepadForm->current->Next();
				//4.2.5.2 현재 줄의 캐럿의 가로 위치까지의 택스트 폭을 구한다.
				currentRowTextWidth = this->notepadForm->textExtent->
				GetTextWidth(this->notepadForm->current->GetPartOfContent(i).c_str());
			}
			//4.2.6 현재 줄의 텍스트 폭에서 이전 줄의 텍스트 폭을 뺀다.
			Long difference = currentRowTextWidth - previousRowTextWidth;
			//(현재줄과 이전줄의 텍스트크기 차이가 0이면 텍스트 크기가 같다는 의미이기때문에
			//현재 캐럿의 가로 위치는 지금 위치 그대로에 있으면 됨)
			//(현재줄과 이전줄의 텍스트크기 차이가 음수이면 
			//currentRowTextSize.cx < previousRowTextSize.cx이라는 뜻인데 그러면 위의
			//반복문 조건에서 빠져나오려면 결국 i가 현재줄의 글자개수(length)와 같아서 빠져나왔다는 
			//의미이고 그렇다면 i는 현재 줄의 마지막 캐럿 위치라는 뜻이고 
			//이동하기전 줄의 텍스트가 현재 줄의 전체 텍스트보다 더 큰 경우는 현재줄의 캐럿가로위치는
			//언제나 현재줄의 마지막에 있어야 하고 현재 마지막 위치에 있기 때문에 그대로 두면 된다.
			//4.2.7 차이가 0보다 크면(현재줄의 텍스트를 다 읽지 못한 경우)
			//현재줄의 전체 텍스트가 이전줄의 캐럿 가로 위치까지 읽은 텍스트보다 큰 경우
			if (difference > 0)
			{
				//4.2.7.1 캐럿의 현재 가로위치 이전의 글자 폭을 구한다.
				Long letterWidth = this->notepadForm->textExtent->
					GetTextWidth(this->notepadForm->current->GetAt(i - 1)->GetContent());
				Long halfLetterSize = letterWidth / 2;
				//4.2.7.2 차이가 읽은 글자크기의 절반보다 같거나 크면
				if (difference >= halfLetterSize)
				{
					//4.2.7.2.1 캐럿의 현재 가로 위치를 이전으로 이동한다.
					this->notepadForm->current->Previous();
				}
			}
		}
		//4.3 캐럿의 이전 가로 위치가 0 또는 캐럿의 현재 줄의 글자개수가 0개이면
		else
		{
			//4.3.1 현재 캐럿의 가로 위치를 0으로 이동시킨다.
			this->notepadForm->current->First();
		}

		//캐럿 이동 먼저 시킨 후에 스크롤 이동
		//4.4 캐럿의 현재 세로 위치를 구한다.
		currentRowIndex = this->notepadForm->note->GetCurrent();
		//4.5 caret 이동 후에 캐럿의 세로 범위(시작과 끝)을 구한다.(beginCaretYPos, endCaretYPos)
		beginCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
		endCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);
		//4.6 수직스크롤을 이동하기 전에 현재 수직스크롤의 VScrollPos(스크롤의 현재 위치)를 구한다.
		VScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
		//4.7 세로스크롤의 현재위치(VScrollPos)와 현재화면의 세로길이의 합을 구한다.
		VerticalSum = VScrollPos + this->notepadForm->
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
			//4.9.2 수직스크롤의 현재위치를 distance로 이동시킨다.
			this->notepadForm->scrollController->scroll[1]->Move(distance);
			//4.9.3 수직스크롤바의 Thumb를 수직스크롤의 현재 위치로 이동시킨다.
			this->notepadForm->SetScrollPos(SB_VERT,
				this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
		}
		//4.10 캐럿이 이동한 후에 캐럿의 현재 가로위치(caretXPos)를 구한다.
		caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
			current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
		//4.11 수평스크롤이 이동하기전에 수평스크롤의 현재위치(HScrollPos)를 구한다.
		HScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
		//4.12 수평스크롤의 현재위치(HScrollPos)와 현재화면의 가로길이의 합을 구한다.
		HorizontalSum = HScrollPos + this->notepadForm->
			scrollController->scroll[0]->GetPageSize();
		//4.13 캐럿의 현재 가로위치(caretXPos)가 수평스크롤의 현재위치(HScrollPos)보다
			//메모장에서 앞에 있으면
		if (caretXPos < HScrollPos)
		{
			//4.13.1 수평스크롤이 이동할 위치(distance)를 구한다.
			distance = caretXPos - (this->notepadForm->
				scrollController->scroll[0]->GetPageSize() / 5);
			//4.13.2 distance가 음수이면
			if (distance < 0)
			{
				//4.13.2.1 distance를 0으로 바꿔준다.
				distance = 0;
			}
			//4.13.3 수평스크롤의 현재위치를 distance로 이동시킨다.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//4.13.4 수평스크롤바의 Thumb를 수평스크롤의 현재 위치로 이동시킨다.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
		//4.14 캐럿의 현재 가로위치(caretXPos)가 수평스크롤의 현재위치(HScrollPos)와
		//현재화면의 가로길이의 합(HorizontalSum)보다 메모장에서 뒤에 있으면
		else if (caretXPos > HorizontalSum)
		{
			//4.14.1 수평스크롤이 이동할 위치(distance)를 구한다.
			distance = caretXPos - (this->notepadForm->
				scrollController->scroll[0]->GetPageSize() / 5 * 4);
			//4.14.2 수평스크롤바의 Thumb의 최대 이동값(maxScrollPos)를 구한다.
			maxScrollPos = this->notepadForm->scrollController->scroll[0]->GetMax()
				- this->notepadForm->scrollController->scroll[0]->GetPageSize();
			//4.14.3 distance가 maxScrollPos보다 크면
			if (distance > maxScrollPos)
			{
				//4.14.3.1 distance에 maxScrollPos를 대입한다.
				distance = maxScrollPos;
			}
			//4.14.4 수평스크롤의 현재위치를 distance로 이동시킨다.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//4.14.5 수평스크롤바의 Thumb를 수평스크롤의 현재 위치로 이동시킨다.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
	}
}

//소멸자
UpArrowKeyAction::~UpArrowKeyAction()
{

}