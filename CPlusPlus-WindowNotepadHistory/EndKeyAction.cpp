#include "EndKeyAction.h"
#include "Glyph.h"

//����Ʈ������
EndKeyAction::EndKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void EndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ���� ���� �� ���������� ĳ���� ���� ��ġ�� �̵���Ų��.
	this->notepadForm->current->Last();
}

//�Ҹ���
EndKeyAction::~EndKeyAction()
{

}