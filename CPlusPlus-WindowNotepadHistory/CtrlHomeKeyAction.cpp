#include "CtrlHomeKeyAction.h"
#include "Glyph.h"

//디폴트생성자
CtrlHomeKeyAction::CtrlHomeKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 메모장의 제일 처음으로 캐럿의 세로 위치를 이동시킨다.
	Long firstIndex = this->notepadForm->note->First();
	//2. 메모장의 현재 줄을 변경한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(firstIndex);
	//3. 현재 줄의 맨 처음으로 캐럿의 가로 위치를 이동시킨다.
	this->notepadForm->current->First();
}

//소멸자
CtrlHomeKeyAction::~CtrlHomeKeyAction()
{

}