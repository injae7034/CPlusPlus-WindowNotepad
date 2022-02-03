#include "LeftArrowKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"

//디폴트생성자
LeftArrowKeyAction::LeftArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//전략패턴
void LeftArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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
	//2. 이전으로 이동하기 전에 글자의 위치를 저장한다.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//3. 이전으로 이동하기 전에 줄의 위치를 저장한다.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//4. 이전으로 이동하고 캐럿의 위치를 반환받는다.
	Long currentLetterIndex = this->notepadForm->current->Previous();
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//5. 캐럿의 이동이 없었으면(캐럿이 줄의 제일 처음에 있어서 더이상 이동할 곳이 없다.)
	if (previousLetterIndex == currentLetterIndex)
	{
		//5.1 이전 줄로 이동시킨다.
		currentRowIndex = this->notepadForm->note->Previous();
		//5.2 줄의 이동이 있었으면(이동하기 전의 줄이 노트의 처음 줄이 아니면)
		if (previousRowIndex != currentRowIndex)
		{
			//5.2.1 현재 줄을 이전으로 이동한 줄로 변경한다.
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
			//5.2.2 줄이 이전으로 이동했기 때문에 캐럿의 현재 위치를 마지막으로 변경한다.
			currentLetterIndex = this->notepadForm->current->Last();
		}
	}
}

//소멸자
LeftArrowKeyAction::~LeftArrowKeyAction()
{

}