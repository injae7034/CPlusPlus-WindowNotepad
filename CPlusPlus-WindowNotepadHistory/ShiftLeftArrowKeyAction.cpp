#include "ShiftLeftArrowKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"

//디폴트생성자
ShiftLeftArrowKeyAction::ShiftLeftArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//전략패턴
void ShiftLeftArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 이전으로 이동하기 전에 글자의 위치를 저장한다.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//2. 이전으로 이동하기 전에 줄의 위치를 저장한다.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//3. 이전으로 이동하고 캐럿의 위치를 반환받는다.
	Long currentLetterIndex = this->notepadForm->current->Previous();
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//4. 캐럿이 한 줄 내에서 이동했으면
	if (previousLetterIndex > currentLetterIndex)
	{
		//4.1 선택이 처음 시작되면
		if (this->notepadForm->isSelecting == false)
		{
			//4.1.1 선택이 진행되고 있는 중으로 상태를 바꾼다.
			this->notepadForm->isSelecting = true;
			//4.1.2 선택이 시작되는 캐럿의 x좌표를 저장한다.
			this->notepadForm->selectedStartXPos = previousLetterIndex;
			//4.1.3 선택이 시작되는 캐럿의 y좌표를 저장한다.
			this->notepadForm->selectedStartYPos = previousRowIndex;
		}
	}
	//5. 캐럿의 이동이 없었으면(캐럿이 줄의 제일 처음에 있어서 더이상 이동할 곳이 없다.)
	if (previousLetterIndex == currentLetterIndex)
	{
		//5.1 이전 줄로 이동시킨다.
		currentRowIndex = this->notepadForm->note->Previous();
		//5.2 줄의 이동이 있었으면(이동하기 전의 줄이 노트의 처음 줄이 아니면)
		if (previousRowIndex > currentRowIndex)
		{
			//5.2.1 현재 줄을 이전으로 이동한 줄로 변경한다.
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
			//5.2.2 줄이 이전으로 이동했기 때문에 캐럿의 현재 위치를 마지막으로 변경한다.
			currentLetterIndex = this->notepadForm->current->Last();
			//5.2.3 선택이 처음 시작되면
			if (this->notepadForm->isSelecting == false)
			{
				//5.2.3.1 선택이 진행되고 있는 중으로 상태를 바꾼다.
				this->notepadForm->isSelecting = true;
				//5.2.3.2 선택이 시작되는 캐럿의 x좌표를 저장한다.
				this->notepadForm->selectedStartXPos = previousLetterIndex;
				//5.2.3.3 선택이 시작되는 캐럿의 y좌표를 저장한다.
				this->notepadForm->selectedStartYPos = previousRowIndex;
			}
		}
	}
	//6. 글자가 선택중이면
	if (this->notepadForm->isSelecting == true)
	{
		//6.1 텍스트를 선택한다.
		this->notepadForm->selectingTexts->DoPrevious(previousRowIndex, previousLetterIndex,
			currentRowIndex, currentLetterIndex);
	}
}

//소멸자
ShiftLeftArrowKeyAction::~ShiftLeftArrowKeyAction()
{

}