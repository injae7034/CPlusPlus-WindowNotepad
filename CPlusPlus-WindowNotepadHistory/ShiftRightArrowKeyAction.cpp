#include "ShiftRightArrowKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"

//디폴트생성자
ShiftRightArrowKeyAction::ShiftRightArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//전략 패턴
void ShiftRightArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 다음으로 이동하기 전에 글자의 위치를 저장한다.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//2. 다음으로 이동하기 전에 줄의 위치를 저장한다.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//3. 다음으로 이동하고 현재 글자의 위치를 반환받는다.
	Long currentLetterIndex = this->notepadForm->current->Next();
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//4. 캐럿이 한 줄 내에서 이동했으면
	if (previousLetterIndex < currentLetterIndex)
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
	//5. 캐럿의 이동이 없었으면(캐럿이 줄의 마지막에 위치하고 있었기 때문에)
	if (previousLetterIndex == currentLetterIndex)
	{
		//5.1 다음 줄로 이동시킨다.
		currentRowIndex = this->notepadForm->note->Next();
		//5.2 줄의 이동이 있었으면(이동하기 전의 줄이 노트의 마지막 줄이 아니면)
		if (previousRowIndex < currentRowIndex)
		{
			//5.2.1 현재 줄을 다음으로 이동한 줄로 변경한다.
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
			//5.2.2 캐럿의 현재 위치를 처음으로 보낸다.(현재 줄이 다음 줄로 옮겨졌기 때문에
			//캐럿은 이전 줄의 마지막 위치에서 다음 줄의 처음 위치로 이동한다.)
			currentLetterIndex = this->notepadForm->current->First();
			//5.2.3 선택이 처음 시작되면
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
	}
	//6. 글자가 선택중이면
	if (this->notepadForm->isSelecting == true)
	{
		//6.1 글자를 선택한다.
		this->notepadForm->selectingTexts->DoNext(previousRowIndex, previousLetterIndex,
			currentRowIndex, currentLetterIndex);
	}
}

//소멸자
ShiftRightArrowKeyAction::~ShiftRightArrowKeyAction()
{

}