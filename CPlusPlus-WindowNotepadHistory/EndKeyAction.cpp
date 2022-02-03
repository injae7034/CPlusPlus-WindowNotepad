#include "EndKeyAction.h"
#include "Glyph.h"
#include "SelectText.h"

//����Ʈ������
EndKeyAction::EndKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void EndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ���� ���� �� ���������� ĳ���� ���� ��ġ�� �̵���Ų��.
	this->notepadForm->current->Last();
	//2. ���� ������ �����Ѵ�.
	SelectText selectText(this->notepadForm);
	selectText.Undo();
}

//�Ҹ���
EndKeyAction::~EndKeyAction()
{

}