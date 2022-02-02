#include "CtrlEndKeyAction.h"
#include "Glyph.h"

//디폴트생성자
CtrlEndKeyAction::CtrlEndKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 메모장의 제일 마지막으로 캐럿의 세로 위치를 이동시킨다.
	Long lastIndex = this->notepadForm->note->Last();
	//2. 메모장의 현재 줄을 변경한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(lastIndex);
	//3. 현재 줄의 맨 마지막으로 캐럿의 가로 위치를 이동시킨다.
	this->notepadForm->current->Last();
}

//소멸자
CtrlEndKeyAction::~CtrlEndKeyAction()
{

}