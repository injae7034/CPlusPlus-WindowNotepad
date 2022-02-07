#include "CtrlCKeyAction.h"

//디폴트생성자
CtrlCKeyAction::CtrlCKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//전략패턴
void CtrlCKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 선택영역이 있으면
	if (this->notepadForm->isSelecting == true)
	{
		//1.1 OnCommand로 메세지를 보낸다.
		this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_COPY);
	}
}

//소멸자
CtrlCKeyAction::~CtrlCKeyAction()
{

}