#include "PageUpKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"

//디폴트생성자
PageUpKeyAction::PageUpKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void PageUpKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 캐럿을 이동시키기 전에 먼저 현재 캐럿이 있는 곳으로 스크롤을 이동시킨다.
	this->notepadForm->Notify();

	//스크롤을 캐럿이 있는 곳으로 이동시킨 캐럿이 있는 곳을 기준삼아 다시 스크롤을 페이지단위로 이동시킨다.
	//2. 페이지 단위로 이동하기 전에 수직스크롤의 현재 위치를 저장한다.
	Long previousVPos = this->notepadForm->scrollController->scroll[1]->GetCurrentPos();
	//3. 수직스크롤을 이전 페이지로 이동시키고 이동된 수직스크롤의 현재 위치를 반환받는다.
	Long currentVPos = this->notepadForm->scrollController->scroll[1]->PagePrevious();
	//4. 수직스크롤에 이동이 있으면(previousVPos와 currentVPos가 다르면 이동이 있었다는 뜻임)
	if (previousVPos != currentVPos)
	{
		//4.1 수직스크롤바의 수직스크롤을 한 페이지 이전으로 이동시킨다.
		this->notepadForm->SetScrollPos(SB_VERT,
			this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
		//4.2 Note에서 캐럿이 실제 이동할 거리를 구한다.
		Long distance = previousVPos - currentVPos;
		//4.3 캐럿이 이동할 줄의 개수를 구한다.
		Long MovingRowCount = distance / this->notepadForm->textExtent->GetHeight();
		//4.4 이동하기 전에 현재 줄의 위치를 구한다.
		Long previousRowIndex = this->notepadForm->note->GetCurrent();
		//4.5 이동하기 전의 캐럿의 가로 위치를 구한다.
		Long previousCaretIndex = this->notepadForm->current->GetCurrent();
		//4.6 캐럿의 세로 위치를 이동시킨다.
		Long currentRowIndex = previousRowIndex - MovingRowCount;
		currentRowIndex = this->notepadForm->note->Move(currentRowIndex);
		//4.7 이동한 후의 현재줄을 변경한다.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
		//4.8 이동하기 전 캐럿의 가로 위치가 0이 아니고 이동한 후 현재줄의 글자개수가 0이 아니면
		//(이동하기 전 캐럿의 가로 위치가 0이면 이동한 후의 현재 캐럿의 가로 위치도 무조건 0이고,
		//이동한 후 현재줄의 글자개수가 0이어도 현재 캐럿의 가로 위치는 무조건 0임,
		//따라서 그 이외의 경우의 수를 if안에서 처리하고 else에서 위의 2경우를 first로 처리함!)
		if (previousCaretIndex != 0 && this->notepadForm->current->GetLength() != 0)
		{
			//4.8.1 이동하기 전 줄의 텍스트 폭을 구한다.
			Long previousRowTextWidth = this->notepadForm->textExtent->
				GetTextWidth(this->notepadForm->note->GetAt(previousRowIndex)
					->GetPartOfContent(previousCaretIndex));
			//4.8.2 캐럿의 현재 가로 위치를 처음으로 이동시킨다.(현재 캐럿의 가로 위치가 어딘지 모르기때문에)
			//i가 0이 되면 GetPartOfContent에서 읽는 텍스트가 없기 때문에 i의 최소값은 1이 되어야함.
			//GetPartOfContent는 캐럿의 위치까지 있는 글자들을 읽는것임!
			//위 if 조건식에서 이미 현재 줄의 글자가 하나도 없는 경우는 걸러져서 들어왔기 때문에
			//현재줄의 글자수는 최소 1개이상은 있는 경우에 대해서 처리함.
			this->notepadForm->current->First();//캐럿의 가로 위치를 맨 처음(0)으로 보냄(원위치)
			//4.8.3 첫번째 글자를 읽기 위해 캐럿을 한칸이동시킨다.
			Long i = this->notepadForm->current->Next();
			//4.8.4 현재 줄의 텍스트의 폭을 구한다.
			Long currentRowTextWidth = this->notepadForm->textExtent->
				GetTextWidth(this->notepadForm->current->GetPartOfContent(i).c_str());
			//4.8.5 i(캐럿의 현재 가로위치)가 length(현재줄의 글자개수)보다 작고
			//현재 줄의 텍스트 크기가 이동하기 전 줄의 텍스트 크기보다 작은동안 반복한다.
			while (i < this->notepadForm->current->GetLength()
				&& currentRowTextWidth < previousRowTextWidth)
			{
				//4.8.5.1 i(캐럿의 현재 가로위치)를 증가시킨다
				//(캐럿의 가로 위치를 다음 칸으로 이동시킨다.).
				i = this->notepadForm->current->Next();
				//4.8.5.2 현재 줄의 캐럿의 가로 위치까지의 택스트 폭을 구한다.
				currentRowTextWidth = this->notepadForm->textExtent->
					GetTextWidth(this->notepadForm->current->GetPartOfContent(i).c_str());
			}
			//4.8.6 현재 줄의 텍스트 폭에서 이전 줄의 텍스트 폭을 뺀다.
			Long difference = currentRowTextWidth - previousRowTextWidth;
			//(현재줄과 이전줄의 텍스트크기 차이가 0이면 텍스트 크기가 같다는 의미이기때문에
			//현재 캐럿의 가로 위치는 지금 위치 그대로에 있으면 됨)
			//(현재줄과 이전줄의 텍스트크기 차이가 음수이면 
			//currentRowTextSize.cx < previousRowTextSize.cx이라는 뜻인데 그러면 위의
			//반복문 조건에서 빠져나오려면 결국 i가 현재줄의 글자개수(length)와 같아서 빠져나왔다는 
			//의미이고 그렇다면 i는 현재 줄의 마지막 캐럿 위치라는 뜻이고 
			//이동하기전 줄의 텍스트가 현재 줄의 전체 텍스트보다 더 큰 경우는 현재줄의 캐럿가로위치는
			//언제나 현재줄의 마지막에 있어야 하고 현재 마지막 위치에 있기 때문에 그대로 두면 된다.
			//4.8.7 차이가 0보다 크면(현재줄의 텍스트를 다 읽지 못한 경우)
			//현재줄의 전체 텍스트가 이전줄의 캐럿 가로 위치까지 읽은 텍스트보다 큰 경우
			if (difference > 0)
			{
				//4.8.7.1 캐럿의 현재 가로위치 이전의 글자 폭을 구한다.
				Long letterWidth = this->notepadForm->textExtent->
					GetTextWidth(this->notepadForm->current->GetAt(i - 1)->GetContent());
				Long halfLetterSize = letterWidth / 2;
				//4.8.7.2 차이가 읽은 글자크기의 절반보다 같거나 크면
				if (difference >= halfLetterSize)
				{
					//4.8.7.2.1 캐럿의 현재 가로 위치를 이전으로 이동한다.
					this->notepadForm->current->Previous();
				}
			}
		}
		//4.9 캐럿의 이전 가로 위치가 0 또는 캐럿의 현재 줄의 글자개수가 0개이면
		else
		{
			//4.9.1 현재 캐럿의 가로 위치를 0으로 이동시킨다.
			this->notepadForm->current->First();
		}
	}
}

//소멸자
PageUpKeyAction::~PageUpKeyAction()
{

}