#include "DownArrowKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "TextExtent.h"

//디폴트생성자
DownArrowKeyAction::DownArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void DownArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 선택이 진행되고 있는 중이었으면
	if (this->notepadForm->isSelecting == true)
	{
		//1.1. 선택된 텍스트를 선택해제한다.(선택을 끝낸다.)
		this->notepadForm->selectingTexts->Undo();
		//1.2 선택이 끝난 상태로 바꾼다.
		this->notepadForm->isSelecting = false;
		//1.3 선택이 끝났기 때문에 캐럿의 x좌표를 0으로 저장한다.
		this->notepadForm->selectedStartXPos = 0;
		//1.4 선택이 끝났기 때문에 캐럿의 y좌표를 0으로 저장한다.
		this->notepadForm->selectedStartYPos = 0;
	}
	//2. 다음으로 이동하기 전에 줄의 위치를 저장한다.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//3. 다음으로 이동하기 전에 글자의 현재 위치를 저장한다.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//4. 다음 줄로 이동하고, 다음 줄의 위치와 글자위치를 저장한다.
	Long currentRowIndex = this->notepadForm->note->Next();
	Long currentLetterIndex= this->notepadForm->current->GetCurrent();
	//5. 다음으로 이동하기 전 줄의 위치와 이동한 후 줄의 위치가 서로 다르면(실질적으로 이동을 했으면)
	if (previousRowIndex != currentRowIndex)
	{
		//5.1 현재 줄을 이동한 후의 줄로 변경한다.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
		//5.2 다음으로 이동하기 전 캐럿의 가로 위치가 0이 아니고 이동한 후 현재줄의 글자개수가 0이 아니면
		//(다음으로 이동하기 전에 캐럿의 위치가 0이면 다음으로 이동한 후의 현재 캐럿의 위치도 무조건 0이고,
		//이동한 후 현재 줄의 글자개수가 0이어도 현재 캐럿의 가로 위치는 무조건 0이기 때문에
		//그 이외의 경우의 수를 if안에서 처리하고 else에서 위의 2경우를 First로 처리하면 된다!)
		if (previousLetterIndex != 0 && this->notepadForm->current->GetLength() != 0)
		{
			//5.2.1 이동하기 전 줄의 이전 캐럿의 위치까지 텍스트 폭을 구한다.
			Long previousRowTextWidth = this->notepadForm->textExtent->
				GetTextWidth(this->notepadForm->note->GetAt(previousRowIndex)
					->GetPartOfContent(previousLetterIndex));
			//5.2.2 캐럿의 현재 위치를 처음으로 이동시킨다.
			this->notepadForm->current->First();//캐럿의 위치를 맨 처음(0)으로 보냄
			//5.2.3 첫번째 글자를 읽기 위해 캐럿을 맨처음에서 다음으로 이동시킨다.
			Long i = this->notepadForm->current->Next();
			//5.2.4 캐럿의 현재 위치까지 텍스트의 폭을 구한다.
			Long currentRowTextWidth = this->notepadForm->textExtent->
				GetTextWidth(this->notepadForm->current->GetPartOfContent(i).c_str());
			//5.2.5 캐럿의 현재 위치(i)가 현재 줄의 글자개수(length)보다 작고
			//캐럿의 현재 위치까지 텍스트 크기가 이동하기 전 줄의 텍스트 크기보다 작은동안 반복한다.
			while (i < this->notepadForm->current->GetLength()
				&& currentRowTextWidth < previousRowTextWidth)
			{
				//5.2.5.1 캐럿의 위치를 다음 칸으로 이동시킨다.
				i = this->notepadForm->current->Next();
				//5.2.5.2 캐럿의 위치까지의 텍스트 폭을 구한다.
				currentRowTextWidth = this->notepadForm->textExtent->
					GetTextWidth(this->notepadForm->current->GetPartOfContent(i).c_str());
			}
			//5.2.6 캐럿의 위치까지의 텍스트 폭에서 이전 줄의 텍스트 폭을 뺀다.
			Long difference = currentRowTextWidth - previousRowTextWidth;
			//5.2.7 차이가 0보다 크면(현재줄의 텍스트를 다 읽지 못한 경우)
			if (difference > 0)
			{
				//5.2.7.1 캐럿의 현재 가로위치 이전의 글자 폭을 구한다.
				Long letterWidth = this->notepadForm->textExtent->
					GetTextWidth(this->notepadForm->current->GetAt(i - 1)->GetContent());
				Long halfLetterSize = letterWidth / 2;
				//5.2.7.2 차이가 읽은 글자크기의 절반보다 같거나 크면
				if (difference >= halfLetterSize)
				{
					//5.2.7.2.1 캐럿의 현재 가로 위치를 이전으로 이동한다.
					currentLetterIndex= this->notepadForm->current->Previous();
				}
			}
		}
		//5.3 캐럿의 이전 가로 위치가 0 또는 캐럿의 현재 줄의 글자개수가 0개이면
		else
		{
			//5.3.1 현재 캐럿의 가로 위치를 0으로 이동시킨다.
			currentLetterIndex = this->notepadForm->current->First();
		}
	}
}

//소멸자
DownArrowKeyAction::~DownArrowKeyAction()
{

}