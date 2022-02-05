#include "ShiftUpArrowKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "TextExtent.h"

//디폴트생성자
ShiftUpArrowKeyAction::ShiftUpArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//전략패턴
void ShiftUpArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 이전으로 이동하기 전에 줄의 위치를 저장한다.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//2. 이전으로 이동하기 전에 글자의 현재 위치를 저장한다.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//3. 이전 줄로 이동하고, 이전 줄의 위치와 글자위치를 저장한다.
	Long currentRowIndex = this->notepadForm->note->Previous();
	Long currentLetterIndex = this->notepadForm->current->GetCurrent();
	//4. 이동하기전 줄의 위치와 이동 후 줄의 위치가 다르면(실질적으로 이동을 했으면)
	if (previousRowIndex != currentRowIndex)
	{
		//4.1 현재 줄을 이전으로 이동한 후의 줄로 변경한다.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
		//4.2 이동하기 전에 캐럿의 위치가 줄의 제일 처음이 아니고 이전으로 이동한 후 줄의 개수가 0이 아니면
		//(이동하기 전에 캐럿의 위치가 0이면 이동한 후의 현재 캐럿의 위치도 무조건 0이고,
		//이동한 후 현재 줄의 글자개수가 0이어도 현재 캐럿의 가로 위치는 무조건 0이기 때문에
		//그 이외의 경우의 수를 if안에서 처리하고 else에서 위의 2경우를 First로 처리하면 된다!)
		if (previousLetterIndex != 0 && this->notepadForm->current->GetLength() != 0)
		{
			//4.2.1 이전으로 이동하기 전 이전 캐럿의 위치까지 줄의 텍스트 폭을 구한다.
			Long previousRowTextWidth = this->notepadForm->textExtent->
				GetTextWidth(this->notepadForm->note->GetAt(previousRowIndex)
					->GetPartOfContent(previousLetterIndex));
			//4.2.2 캐럿의 현재 위치를 처음으로 이동시킨다.(현재 캐럿의 위치가 어딘지 모르기때문에)
			//i가 0이 되면 GetPartOfContent에서 읽는 텍스트가 없기 때문에 i의 최소값은 1이 되어야함.
			//GetPartOfContent는 캐럿의 위치까지 있는 글자들을 읽는것임!
			//위 if 조건식에서 이미 현재 줄의 글자가 하나도 없는 경우는 걸러져서 들어왔기 때문에
			//현재줄의 글자수는 최소 1개이상은 있는 경우에 대해서 처리함.
			this->notepadForm->current->First();//캐럿의 가로 위치를 맨 처음(0)으로 보냄(원위치)
			//4.2.3 첫번째 글자를 읽기 위해 캐럿을 한칸이동시킨다.
			currentLetterIndex = this->notepadForm->current->Next();
			//4.2.4 현재 줄에서 캐럿의 위치(i)까지 텍스트의 폭을 구한다.
			Long currentRowTextWidth = this->notepadForm->textExtent->
				GetTextWidth(this->notepadForm->current->GetPartOfContent
				(currentLetterIndex).c_str());
			//4.2.5 캐럿의 현재 위치(i)가 현재 줄의 글자개수(length)보다 작고
			//캐럿의 현재 위치까지 텍스트 크기가 이동하기 전 줄의 텍스트 크기보다 작은동안 반복한다.
			while (currentLetterIndex < this->notepadForm->current->GetLength()
				&& currentRowTextWidth < previousRowTextWidth)
			{
				//4.2.5.1 캐럿의 현재 위치를 다음 칸으로 이동시킨다.
				currentLetterIndex = this->notepadForm->current->Next();
				//4.2.5.2 캐럿의 위치까지 텍스트 폭을 구한다.
				currentRowTextWidth = this->notepadForm->textExtent->
					GetTextWidth(this->notepadForm->current->GetPartOfContent
					(currentLetterIndex).c_str());
			}
			//4.2.6 현재 캐럿의 위치까지 텍스트 폭에서 이전 줄의 텍스트 폭을 뺀다.
			Long difference = currentRowTextWidth - previousRowTextWidth;
			//(현재줄과 이전줄의 텍스트크기 차이가 0이면 텍스트 크기가 같다는 의미이기때문에
			//현재 캐럿의 위치는 지금 위치 그대로에 있으면 됨)
			//(현재줄과 이전줄의 텍스트크기 차이가 음수이면 
			//currentRowTextSize.cx < previousRowTextSize.cx이라는 뜻인데 그러면 위의
			//반복문 조건에서 빠져나오려면 결국 i가 현재줄의 글자개수(length)와 같아서 빠져나왔다는 
			//의미이고 그렇다면 i는 현재 줄의 마지막 캐럿 위치라는 뜻이고 
			//이동하기전 줄의 텍스트가 현재 줄의 전체 텍스트보다 더 큰 경우는 현재 줄의 캐럿위치는
			//언제나 현재ㅍ줄의 마지막에 있어야 하고 i가 현재 마지막 위치에 있기 때문에 그대로 두면 된다.
			//4.2.7 차이가 0보다 크면(현재 줄의 텍스트를 다 읽지 못한 경우)
			//현재 줄의 캐럿 위치까지 텍스트가 이전 줄의 캐럿 위치까지 읽은 텍스트보다 큰 경우
			if (difference > 0)
			{
				//4.2.7.1 캐럿의 현재 위치 이전까지의 글자 폭을 구한다.
				Long letterWidth = this->notepadForm->textExtent->
					GetTextWidth(this->notepadForm->current->
						GetAt(currentLetterIndex - 1)->GetContent());
				Long halfLetterSize = letterWidth / 2;
				//4.2.7.2 차이가 읽은 글자크기의 절반보다 같거나 크면
				if (difference >= halfLetterSize)
				{
					//4.2.7.2.1 캐럿의 현재 가로 위치를 이전으로 이동한다.
					currentLetterIndex = this->notepadForm->current->Previous();
				}
			}
		}
		//4.3 캐럿의 이전 위치가 0 또는 캐럿의 현재 줄의 글자개수가 0개이면
		else
		{
			//4.3.1 현재 캐럿의 위치를 0으로 이동시킨다.
			currentLetterIndex = this->notepadForm->current->First();
		}
		//4.4 선택이 처음 시작되면
		if (this->notepadForm->isSelecting == false)
		{
			//4.4.1 선택이 진행되고 있는 중으로 상태를 바꾼다.
			this->notepadForm->isSelecting = true;
			//4.4.2 선택이 시작되는 캐럿의 x좌표를 저장한다.
			this->notepadForm->selectedStartXPos = previousLetterIndex;
			//4.4.3 선택이 시작되는 캐럿의 y좌표를 저장한다.
			this->notepadForm->selectedStartYPos = previousRowIndex;
		}
	}
	//5. 선택이 진행 중이면
	if (this->notepadForm->isSelecting == true)
	{
		//5.1 텍스트를 선택한다.
		this->notepadForm->selectingTexts->DoPrevious(previousRowIndex, previousLetterIndex,
			currentRowIndex, currentLetterIndex);
	}
}

//소멸자
ShiftUpArrowKeyAction::~ShiftUpArrowKeyAction()
{

}