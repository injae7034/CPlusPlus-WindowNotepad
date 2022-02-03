#include "CtrlHomeKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"

//디폴트생성자
CtrlHomeKeyAction::CtrlHomeKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 노트에서 줄을 제일 처음으로 이동시킨다.
	Long firstRowIndex = this->notepadForm->note->First();
	//2. 현재 줄을 변경한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(firstRowIndex);
	//3. 현재 줄의 맨 처음으로 캐럿의 위치를 이동시킨다.
	this->notepadForm->current->First();
}

//소멸자
CtrlHomeKeyAction::~CtrlHomeKeyAction()
{

}