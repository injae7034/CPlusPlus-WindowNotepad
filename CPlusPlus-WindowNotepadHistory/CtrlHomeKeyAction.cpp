#include "CtrlHomeKeyAction.h"
#include "Glyph.h"

//����Ʈ������
CtrlHomeKeyAction::CtrlHomeKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. �޸����� ���� ó������ ĳ���� ���� ��ġ�� �̵���Ų��.
	Long firstIndex = this->notepadForm->note->First();
	//2. �޸����� ���� ���� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(firstIndex);
	//3. ���� ���� �� ó������ ĳ���� ���� ��ġ�� �̵���Ų��.
	this->notepadForm->current->First();
}

//�Ҹ���
CtrlHomeKeyAction::~CtrlHomeKeyAction()
{

}