#include "CtrlFKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "CommandHistory.h"

//����Ʈ������
CtrlFKeyAction::CtrlFKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void CtrlFKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. OnCommand�� �޼����� ������.
	this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_FIND);
}

//�Ҹ���
CtrlFKeyAction::~CtrlFKeyAction()
{

}