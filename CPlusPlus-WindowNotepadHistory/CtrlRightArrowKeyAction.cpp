#include "CtrlRightArrowKeyAction.h"
#include "Glyph.h"

#include "SelectingTexts.h"

//디폴트생성자
CtrlRightArrowKeyAction::CtrlRightArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlRightArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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
	//2. 이동하기 전에 줄의 위치를 구한다.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//3. 이동하기 전에 글자의 위치를 구한다.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//4. 노트에서 단어단위로 오른쪽으로 이동한다.
	Long currentLetterIndex = this->notepadForm->note->NextWord();
	//5. 이동한 후에 줄의 위치를 구한다.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//6. 현재 줄을 이동한 후의 줄로 변경한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	//7. 줄에서 캐럿을 단어단위로 이동한다.
	this->notepadForm->current->Move(currentLetterIndex);
}

//소멸자
CtrlRightArrowKeyAction::~CtrlRightArrowKeyAction()
{

}