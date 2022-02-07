#include "CtrlCKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "CommandHistory.h"

//����Ʈ������
CtrlCKeyAction::CtrlCKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void CtrlCKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. OnCommand�� �޼����� ������.
	this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_COPY);
}

//�Ҹ���
CtrlCKeyAction::~CtrlCKeyAction()
{

}