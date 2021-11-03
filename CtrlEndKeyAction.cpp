#include "CtrlEndKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"

//디폴트생성자
CtrlEndKeyAction::CtrlEndKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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
	//2. 메모장의 제일 마지막으로 줄의 위치를 이동시킨다.
	Long lastRowIndex = this->notepadForm->note->Last();
	//3. 메모장의 현재 줄을 변경한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(lastRowIndex);
	//4. 현재 줄의 맨 마지막으로 캐럿의 위치를 이동시킨다.
	this->notepadForm->current->Last();
}

//소멸자
CtrlEndKeyAction::~CtrlEndKeyAction()
{

}