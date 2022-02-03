#include "CtrlAKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"

//디폴트생성자
CtrlAKeyAction::CtrlAKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//전략패턴
void CtrlAKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 이전에 있던 글자 선택을 해제한다.
	this->notepadForm->selectingTexts->Undo();
	//2. 마지막 줄로 이동하기 전에 줄의 위치를 저장한다.
	Long previousRowIndex = 0;
	//3. 마지막으로 글자로 이동하기 전에 글자의 위치를 저장한다.
	Long previousLetterIndex = 0;
	//4. 선택이 진행되고 있는 중으로 상태를 바꾼다.
	this->notepadForm->isSelecting = true;
	//5 선택이 시작되는 캐럿의 x좌표를 저장한다.
	this->notepadForm->selectedStartXPos = previousLetterIndex;
	//6. 선택이 시작되는 캐럿의 y좌표를 저장한다.
	this->notepadForm->selectedStartYPos = previousRowIndex;
	//7. 메모장의 제일 마지막 줄로 이동한다.
	Long currentRowIndex = this->notepadForm->note->Last();
	//8. 메모장의 현재 줄을 변경한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	//9. 현재 줄의 마지막 글자로 이동한다.
	Long currentLetterIndex = this->notepadForm->current->Last();
	//10. 전체 글자를 선택한다.
	this->notepadForm->selectingTexts->DoNext(previousRowIndex, previousLetterIndex,
		currentRowIndex, currentLetterIndex);
}

//소멸자
CtrlAKeyAction::~CtrlAKeyAction()
{

}