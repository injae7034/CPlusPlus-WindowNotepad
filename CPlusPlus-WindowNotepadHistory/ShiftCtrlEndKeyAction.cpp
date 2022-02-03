#include "ShiftCtrlEndKeyAction.h"
#include "Glyph.h"
#include "SelectText.h"

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
	//3. 메모장의 제일 마지막 줄로 이동한다.
	Long currentRowIndex = this->notepadForm->note->Last();
	//4. 메모장의 현재 줄을 변경한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	//5. 현재 줄의 마지막 글자로 이동한다.
	Long currentLetterIndex = this->notepadForm->current->Last();
	//6. 글자를 선택한다.
	SelectText selectText(this->notepadForm);
	selectText.DoNext(previousRowIndex, previousLetterIndex, currentRowIndex,
		currentLetterIndex);
}

//소멸자
ShiftCtrlEndKeyAction::~ShiftCtrlEndKeyAction()
{

}