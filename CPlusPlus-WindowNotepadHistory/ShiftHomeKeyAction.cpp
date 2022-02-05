#include "ShiftHomeKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"

//디폴트생성자
ShiftHomeKeyAction::ShiftHomeKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//전략패턴
void ShiftHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 글자위치를 마지막으로 이동하기 전에 줄의 위치를 저장한다.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//2. 글자위치를 마지막으로 이동하기 전에 글자의 현재 위치를 저장한다.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//3. 현재 줄의 맨 처음으로 글자의 위치를 이동시킨다.
	Long currentLetterIndex = this->notepadForm->current->First();
	//4. 이동 후 줄의 위치를 저장한다.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//5. 이동이 있었으면
	if (previousLetterIndex > currentLetterIndex)
	{
		//5.1 선택이 처음 시작되면
		if (this->notepadForm->isSelecting == false)
		{
			//5.1.1 선택이 진행되고 있는 중으로 상태를 바꾼다.
			this->notepadForm->isSelecting = true;
			//5.1.2 선택이 시작되는 캐럿의 x좌표를 저장한다.
			this->notepadForm->selectedStartXPos = previousLetterIndex;
			//5.1.3 선택이 시작되는 캐럿의 y좌표를 저장한다.
			this->notepadForm->selectedStartYPos = previousRowIndex;
		}
	}
	//6. 선택이 진행 중이면
	if (this->notepadForm->isSelecting == true)
	{
		//6.1 텍스트를 선택한다.
		this->notepadForm->selectingTexts->DoPrevious(previousRowIndex, previousLetterIndex,
			currentRowIndex, currentLetterIndex);
	}
}

//소멸자
ShiftHomeKeyAction::~ShiftHomeKeyAction()
{

}