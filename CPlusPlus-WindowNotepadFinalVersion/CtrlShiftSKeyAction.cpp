#include "CtrlShiftSKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "CommandHistory.h"

//����Ʈ������
CtrlShiftSKeyAction::CtrlShiftSKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void CtrlShiftSKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. OnCommand�� �޼����� ������.
	this->notepadForm->SendMessage(WM_COMMAND, IDM_FILE_SAVEDIFFERENTNAME);
}

//�Ҹ���
CtrlShiftSKeyAction::~CtrlShiftSKeyAction()
{

}