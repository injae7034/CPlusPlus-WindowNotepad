#include "ShiftEndKeyAction.h"
#include "Glyph.h"
#include "SelectText.h"

//디폴트생성자
ShiftEndKeyAction::ShiftEndKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//전략패턴
void ShiftEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 글자위치를 마지막으로 이동하기 전에 줄의 위치를 저장한다.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//2. 글자위치를 마지막으로 이동하기 전에 글자의 현재 위치를 저장한다.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//3. 현재 줄의 맨 마지막으로 글자의 위치를 이동시킨다.
	Long currentLetterIndex = this->notepadForm->current->Last();
	//4. 이동 후 줄의 위치를 저장한다.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//5. 글자를 선택한다.
	SelectText selectText(this->notepadForm);
	selectText.DoNext(previousRowIndex, previousLetterIndex, currentRowIndex,
		currentLetterIndex);
}

//소멸자
ShiftEndKeyAction::~ShiftEndKeyAction()
{

}