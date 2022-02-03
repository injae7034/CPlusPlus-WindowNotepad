#include "ShiftCtrlHomeKeyAction.h"
#include "Glyph.h"
#include "SelectText.h"

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
	//6. 글자를 선택한다.
	SelectText selectText(this->notepadForm);
	selectText.DoPrevious(previousRowIndex, previousLetterIndex, currentRowIndex,
		currentLetterIndex);
}

//소멸자
ShiftCtrlHomeKeyAction::~ShiftCtrlHomeKeyAction()
{

}