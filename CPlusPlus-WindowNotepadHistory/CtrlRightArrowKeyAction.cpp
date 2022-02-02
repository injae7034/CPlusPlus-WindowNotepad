#include "CtrlRightArrowKeyAction.h"
#include "Glyph.h"

//디폴트생성자
CtrlRightArrowKeyAction::CtrlRightArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlRightArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. Note의 NextWord를 호출한다.
	Long currentCaretindex = this->notepadForm->note->NextWord();
	//2. 현재줄을 변경한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(this->notepadForm->note->GetCurrent());
	//3. 현재줄의 캐럿의 가로 위치를 변경한다.
	Long i = this->notepadForm->current->First();
	while (i < currentCaretindex)
	{
		i = this->notepadForm->current->Next();
	}
}

//소멸자
CtrlRightArrowKeyAction::~CtrlRightArrowKeyAction()
{

}