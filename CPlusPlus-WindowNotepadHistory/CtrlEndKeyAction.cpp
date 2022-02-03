#include "CtrlEndKeyAction.h"
#include "Glyph.h"
#include "SelectText.h"

//����Ʈ������
CtrlEndKeyAction::CtrlEndKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. �޸����� ���� ���������� ���� ��ġ�� �̵���Ų��.
	Long lastRowIndex = this->notepadForm->note->Last();
	//2. �޸����� ���� ���� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(lastRowIndex);
	//3. ���� ���� �� ���������� ĳ���� ��ġ�� �̵���Ų��.
	this->notepadForm->current->Last();
	//4. ���� ������ �����Ѵ�.
	SelectText selectText(this->notepadForm);
	selectText.Undo();
}

//�Ҹ���
CtrlEndKeyAction::~CtrlEndKeyAction()
{

}