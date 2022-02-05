#include "ShiftCtrlLeftArrowKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"

//디폴트생성자
ShiftCtrlLeftArrowKeyAction::ShiftCtrlLeftArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//전략패턴
void ShiftCtrlLeftArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 이동하기 전에 줄의 위치를 구한다.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//2. 이동하기 전에 글자의 위치를 구한다.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//3. 노트에서 단어단위로 왼쪽으로 이동한다.
	Long currentLetterIndex = this->notepadForm->note->PreviousWord();
	//4. 이동한 후에 줄의 위치를 구한다.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//5. 현재 줄을 이동한 후의 줄로 변경한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	//6. 줄에서 캐럿을 단어단위로 이동한다.
	this->notepadForm->current->Move(currentLetterIndex);
	//7. 단어단위로 이동한 후에 줄의 위치가 서로 다르면
	if (previousRowIndex > currentRowIndex)
	{
		//7.1 선택이 처음 시작되면
		if (this->notepadForm->isSelecting == false)
		{
			//7.1.1 선택이 진행되고 있는 중으로 상태를 바꾼다.
			this->notepadForm->isSelecting = true;
			//7.1.2 선택이 시작되는 캐럿의 x좌표를 저장한다.
			this->notepadForm->selectedStartXPos = previousLetterIndex;
			//7.1.3 선택이 시작되는 캐럿의 y좌표를 저장한다.
			this->notepadForm->selectedStartYPos = previousRowIndex;
		}
	}
	//8. 단어단위로 이동한 후에 줄의 위치가 서로 같으면(한 줄 내에서 단어단위 이동이 있으면)
	else
	{
		//8.1 단어 단위로 이동이 있었으면
		if (previousLetterIndex > currentLetterIndex)
		{
			//8.1.1 선택이 처음 시작되면
			if (this->notepadForm->isSelecting == false)
			{
				//8.1.1.1 선택이 진행되고 있는 중으로 상태를 바꾼다.
				this->notepadForm->isSelecting = true;
				//8.1.1.2 선택이 시작되는 캐럿의 x좌표를 저장한다.
				this->notepadForm->selectedStartXPos = previousLetterIndex;
				//8.1.1.3 선택이 시작되는 캐럿의 y좌표를 저장한다.
				this->notepadForm->selectedStartYPos = previousRowIndex;
			}
		}
	}
	//9. 선택이 진행 중이면
	if (this->notepadForm->isSelecting == true)
	{
		//9.1 텍스트를 선택한다.
		this->notepadForm->selectingTexts->DoPrevious(previousRowIndex, previousLetterIndex,
			currentRowIndex, currentLetterIndex);
	}
}

//소멸자
ShiftCtrlLeftArrowKeyAction::~ShiftCtrlLeftArrowKeyAction()
{

}