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
	Long caretYPos = 0;
	Long caretXPos = 0;
	Long sum = 0;
	Long distance = 0;
	//2. 이전으로 이동하기 전에 캐럿의 현재 가로위치를 저장한다.
	Long previousIndex = this->notepadForm->current->GetCurrent();
	//3. 이전으로 이동하고 현재 캐럿의 가로위치를 반환받는다.
	Long currentIndex = this->notepadForm->current->Previous();
	//4. 캐럿의 이전 가로위치와 캐럿의 현재 가로위치가 같으면(캐럿의 이동이 없었으면)
	//이동이 없었다는 말은 캐럿이 현재 줄의 제일 처음에 위치해서 더이상 이동할 곳이 없다는 뜻이다.
	if (previousIndex == currentIndex)
	{
		//4.1 이전으로 이동하기 전에 캐럿의 현재 세로 위치를 저장한다.
		Long previousRowIndex = this->notepadForm->note->GetCurrent();
		//4.2 캐럿의 현재 세로 위치를 이전 줄로 이동시킨다.
		Long currentRowIndex = this->notepadForm->note->Previous();
		//4.3 캐럿의 이전 세로 위치와 캐럿의 현재 새로 위치가 서로 다르면
		if (previousRowIndex != currentRowIndex)
		{
			//4.3.1 캐럿의 현재 줄을 변경한다.
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
			//4.3.2 캐럿의 현재 가로 위치를 변경한다.
			this->notepadForm->current->Last();

			//4.3.3 현재 줄까지 총 세로 길이를 구한다.(캐럿의 세로 위치를 구한다.)
			//caretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);

			//4.3.3.이전 줄까지의 총 세로 길이를 구한다.(캐럿의 y좌표 시작점을 구한다.)
			//수직스크롤의 현재 위치는 글자들의 높이는 반영되어 있지 않기 때문에 이전줄까지의 길이를 구해야
			//현재 캐럿의 y좌표의 시작점을 알 수 있다 현재 줄까지의 총 세로 길이를 구하면
			//글자들의 높이가 반영이 되어서 수직스크롤의 현재 위치와 비교할 수 없다!
			caretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
			//4.3.4 수직스크롤의 현재 위치를 구한다.
			VScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
			//4.3.5 현재화면의 세로 길이와 수직스크롤의 현재 위치의 합을 구한다.
			sum = pageHeight + VScrollPos;
			//4.3.6 이전 줄까지의 총 세로 길이가 수직스크롤위 현재 위치보다 작거나 또는
			//이전 줄까지의 총 세로 길이가 현재화면의 세로 길이와 수직스크롤의 현재 위치의 합보다
			//크거나 같으면

			if (caretYPos < VScrollPos || caretYPos > sum)
			{

				//4.3.6.1 수직스크롤을 한줄 위로 이동시킨다.
				//this->notepadForm->scrollController->scroll[1]->LinePrevious();
				//caretYPos += pageHeight;//이렇게해야 정확히 한 줄씩 이동함.

				//4.3.6.1 수직스크롤을 caretYPos로 이동시킨다.
				this->notepadForm->scrollController->scroll[1]->Move(caretYPos);
				//4.3.6.2 수직스크롤바의 수직스크롤을 이동시킨다.
				this->notepadForm->SetScrollPos(SB_VERT,
					this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
			}


			//4.3.7 현재 캐럿의 가로 위치(x좌표)를 구한다.
			caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
				current->GetPartOfContent(this->notepadForm->current->GetCurrent()));

			//이 상황에서는 수평스크롤의 현재 위치는 무조건 0이기 때문에 따로 구할 필요가 없다.
			//4.3.7 수평스크롤의 현재 위치를 구한다.
			//currentScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);

			//4.3.8 현재화면크기보다 현재 캐럿의 가로 위치(x좌표)가 더크면
			if (caretXPos > pageWidth)
			{
				//4.3.8.1 캐럿이 이동한 글자의 너비만큼 수평스크롤을 이동시켜준다.
				caretXPos -= pageWidth;//수평스크롤의 현재위치가 누적되어있음!
				this->notepadForm->scrollController->scroll[0]->Move(caretXPos + 2);
				//4.3.8.2 수평스크롤바의 수평스크롤을 이동시킨다.
				this->notepadForm->SetScrollPos(SB_HORZ,
					this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
			}

		}
	}
	//5. 캐럿의 이전 가로위치와 캐럿의 현재 가로위치가 다르면(캐럿의 이동이 있었으면)
	//캐럿이 이동했다는 것은 캐럿이 이동하기 전에 줄의 처음에 있지 않았다는 뜻임
	//따라서 캐럿은 Previous연산을 통해 정상적으로 한 칸 이동했기 때문에
	//수평스크롤만 이동할지 말지 확인하면 됨
	else
	{
		//5.1 현재 캐럿의 x좌표를 구한다.(Previous로 이동한 후의 x좌표)
		caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
			current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
		//5.2 수평스크롤의 현재 위치를 구한다. 
		HScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
		//5.3 현재 캐럿의 위치가 수평스크롤의 현재 위치보다 작으면
		if (caretXPos < HScrollPos)
		{
			//5.3.1 수평스크롤의 이동할 범위를 구한다.
			//<-버튼을 누를 때마다 가로스크롤의 현재 위치에서 누적해 현재 화면의 5분의 1 크기만큼 이동한다. 
			distance = HScrollPos - (pageWidth / 5);
			//5.3.2 캐럿이 이동한 글자의 너비만큼 수평스크롤을 이동시켜준다.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//5.3.3 수평스크롤바의 수평스크롤을 이동시킨다.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
	}
}

//소멸자
LeftArrowKeyAction::~LeftArrowKeyAction()
{

}