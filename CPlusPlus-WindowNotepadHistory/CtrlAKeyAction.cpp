#include "CtrlAKeyAction.h"
#include "Glyph.h"
#include "SelectText.h"

//디폴트생성자
CtrlAKeyAction::CtrlAKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//전략패턴
void CtrlAKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 이전에 있던 글자 선택을 해제한다.
	SelectText selectText(this->notepadForm);
	selectText.Undo();
	//2. 마지막 줄로 이동하기 전에 줄의 위치를 저장한다.
	Long previousRowIndex = 0;
	//3. 마지막으로 글자로 이동하기 전에 글자의 위치를 저장한다.
	Long previousLetterIndex = 0;
	//4. 메모장의 제일 마지막 줄로 이동한다.
	Long currentRowIndex = this->notepadForm->note->Last();
	//5. 메모장의 현재 줄을 변경한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	//6. 현재 줄의 마지막 글자로 이동한다.
	Long currentLetterIndex = this->notepadForm->current->Last();
	//7. 전체 글자를 선택한다.
	selectText.DoNext(previousRowIndex, previousLetterIndex, currentRowIndex,
		currentLetterIndex);
	
}

//소멸자
CtrlAKeyAction::~CtrlAKeyAction()
{

}