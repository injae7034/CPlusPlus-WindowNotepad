#include "ShiftCtrlHomeKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"

//디폴트생성자
ShiftCtrlHomeKeyAction::ShiftCtrlHomeKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//전략패턴
void ShiftCtrlHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 처음 줄로 이동하기 전에 줄의 위치를 저장한다.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//2. 처음 글자로 이동하기 전에 글자의 위치를 저장한다.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//3. 제일 처음 줄로 이동한다.
	Long currentRowIndex = this->notepadForm->note->First();
	//4. 현재 줄을 변경한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	//5. 제일 처음 글자로 이동한다.
	Long currentLetterIndex = this->notepadForm->current->First();
	//6. 단어단위로 이동한 후에 줄의 위치가 서로 다르면
	if (previousRowIndex > currentRowIndex)
	{
		//6.1 선택이 처음 시작되면
		if (this->notepadForm->isSelecting == false)
		{
			//6.1.1 선택이 진행되고 있는 중으로 상태를 바꾼다.
			this->notepadForm->isSelecting = true;
			//6.1.2 선택이 시작되는 캐럿의 x좌표를 저장한다.
			this->notepadForm->selectedStartXPos = previousLetterIndex;
			//6.1.3 선택이 시작되는 캐럿의 y좌표를 저장한다.
			this->notepadForm->selectedStartYPos = previousRowIndex;
		}
	}
	//7. 단어단위로 이동한 후에 줄의 위치가 서로 같으면(한 줄 내에서 단어단위 이동이 있으면)
	else
	{
		//7.1 단어 단위로 이동이 있었으면
		if (previousLetterIndex > currentLetterIndex)
		{
			//7.1.1 선택이 처음 시작되면
			if (this->notepadForm->isSelecting == false)
			{
				//7.1.1.1 선택이 진행되고 있는 중으로 상태를 바꾼다.
				this->notepadForm->isSelecting = true;
				//7.1.1.2 선택이 시작되는 캐럿의 x좌표를 저장한다.
				this->notepadForm->selectedStartXPos = previousLetterIndex;
				//7.1.1.3 선택이 시작되는 캐럿의 y좌표를 저장한다.
				this->notepadForm->selectedStartYPos = previousRowIndex;
			}
		}
	}
	//8. 선택이 진행 중이면
	if (this->notepadForm->isSelecting == true)
	{
		//8.1 텍스트를 선택한다.
		this->notepadForm->selectingTexts->DoPrevious(previousRowIndex, previousLetterIndex,
			currentRowIndex, currentLetterIndex);
	}
}

//소멸자
ShiftCtrlHomeKeyAction::~ShiftCtrlHomeKeyAction()
{

}