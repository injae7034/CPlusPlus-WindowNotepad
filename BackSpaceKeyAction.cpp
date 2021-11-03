#include "BackSpaceKeyAction.h"
#include "Glyph.h"

//����Ʈ������
BackSpaceKeyAction::BackSpaceKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void BackSpaceKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. OnCommand�� �޼����� ������.
	this->notepadForm->SendMessage(WM_COMMAND, ID_BACKSPACEKEYACTIONCOMMAND);
}

//�Ҹ���
BackSpaceKeyAction::~BackSpaceKeyAction()
{

}