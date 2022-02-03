#include "ShiftCtrlEndKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"

//디폴트생성자
ShiftCtrlEndKeyAction::ShiftCtrlEndKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void ShiftCtrlEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 마지막 줄로 이동하기 전에 줄의 위치를 저장한다.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//2. 마지막으로 글자로 이동하기 전에 글자의 위치를 저장한다.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//3. 선택이 처음 시작되면
	if (this->notepadForm->isSelecting == false)
	{
		//3.1 선택이 진행되고 있는 중으로 상태를 바꾼다.
		this->notepadForm->isSelecting = true;
		//3.2 선택이 시작되는 캐럿의 x좌표를 저장한다.
		this->notepadForm->selectedStartXPos = previousLetterIndex;
		//3.3 선택이 시작되는 캐럿의 y좌표를 저장한다.
		this->notepadForm->selectedStartYPos = previousRowIndex;
	}
	//4. 메모장의 제일 마지막 줄로 이동한다.
	Long currentRowIndex = this->notepadForm->note->Last();
	//5. 메모장의 현재 줄을 변경한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	//6. 현재 줄의 마지막 글자로 이동한다.
	Long currentLetterIndex = this->notepadForm->current->Last();
	//7. 글자를 선택한다.
	this->notepadForm->selectingTexts->DoNext(previousRowIndex, previousLetterIndex,
		currentRowIndex, currentLetterIndex);
}

//소멸자
ShiftCtrlEndKeyAction::~ShiftCtrlEndKeyAction()
{

}