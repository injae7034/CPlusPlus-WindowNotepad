#include "CtrlRightArrowKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "DummyRow.h"

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
	//2. 메모장에서 현재 줄의 위치를 구한다.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//3. 메모장에서 현재 글자의 위치를 구한다.
	Long currentLetterIndex = this->notepadForm->note->GetAt(currentRowIndex)->GetCurrent();
	//4. 자동개행이 진행 중이 아니면
	if (this->notepadForm->isRowAutoChanging == false)
	{
		//4.1 노트에서 단어단위로 오른쪽으로 이동한다.
		currentLetterIndex = this->notepadForm->note->NextWord();
		//4.2 이동한 후에 줄의 위치를 구한다.
		currentRowIndex = this->notepadForm->note->GetCurrent();
		//4.3 메모장의 현재 줄을 이동한 후의 줄로 변경한다.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	}
	//5. 자동개행이 진행 중이면
	else
	{
		//5.1 노트에서 자동개행 진행 중인 상태에서 단어단위로 오른쪽으로 이동한다.
		this->notepadForm->note->NextWordOnRowAutoChange(currentRowIndex, currentLetterIndex);
		//5.2 이동한 후에 줄의 위치를 구한다.
		currentRowIndex = this->notepadForm->note->GetCurrent();
		//5.3 메모장의 현재 줄을 이동한 후의 줄로 변경한다.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	}
}

//소멸자
CtrlRightArrowKeyAction::~CtrlRightArrowKeyAction()
{

}