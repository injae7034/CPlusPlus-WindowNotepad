#include "CtrlEndKeyAction.h"
#include "Glyph.h"

//����Ʈ������
CtrlEndKeyAction::CtrlEndKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. �޸����� ���� ���������� ĳ���� ���� ��ġ�� �̵���Ų��.
	Long lastIndex = this->notepadForm->note->Last();
	//2. �޸����� ���� ���� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(lastIndex);
	//3. ���� ���� �� ���������� ĳ���� ���� ��ġ�� �̵���Ų��.
	this->notepadForm->current->Last();
}

//�Ҹ���
CtrlEndKeyAction::~CtrlEndKeyAction()
{

}