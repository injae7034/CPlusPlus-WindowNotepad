#include "CtrlXKeyAction.h"

//디폴트생성자
CtrlXKeyAction::CtrlXKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//전략패턴
void CtrlXKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 선택영역이 있으면
	if (this->notepadForm->isSelecting == true)
	{
		//1.1 OnCommand로 메세지를 보낸다.
		this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_CUT);
	}
}

//소멸자
CtrlXKeyAction::~CtrlXKeyAction()
{

}