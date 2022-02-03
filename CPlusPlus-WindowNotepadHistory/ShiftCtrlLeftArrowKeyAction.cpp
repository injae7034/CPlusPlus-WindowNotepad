#include "ShiftCtrlLeftArrowKeyAction.h"
#include "Glyph.h"
#include "SelectText.h"

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
	//7. 텍스트를 선택한다.
	SelectText selectText(this->notepadForm);
	selectText.DoPrevious(previousRowIndex, previousLetterIndex, currentRowIndex,
		currentLetterIndex);
}

//소멸자
ShiftCtrlLeftArrowKeyAction::~ShiftCtrlLeftArrowKeyAction()
{

}