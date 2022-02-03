#include "ShiftLeftArrowKeyAction.h"
#include "Glyph.h"
#include "SelectText.h"
#include "SelectText.h"

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
	//4. 캐럿의 이동이 없었으면(캐럿이 줄의 제일 처음에 있어서 더이상 이동할 곳이 없다.)
	if (previousLetterIndex == currentLetterIndex)
	{
		//4.1 이전 줄로 이동시킨다.
		currentRowIndex = this->notepadForm->note->Previous();
		//4.2 줄의 이동이 있었으면(이동하기 전의 줄이 노트의 처음 줄이 아니면)
		if (previousRowIndex != currentRowIndex)
		{
			//4.2.1 현재 줄을 이전으로 이동한 줄로 변경한다.
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
			//4.2.2 줄이 이전으로 이동했기 때문에 캐럿의 현재 위치를 마지막으로 변경한다.
			currentLetterIndex = this->notepadForm->current->Last();
		}
	}
	//5. 텍스트를 선택한다.
	SelectText selectText(this->notepadForm);
	selectText.DoPrevious(previousRowIndex, previousLetterIndex, currentRowIndex,
		currentLetterIndex);
}

//소멸자
ShiftLeftArrowKeyAction::~ShiftLeftArrowKeyAction()
{

}