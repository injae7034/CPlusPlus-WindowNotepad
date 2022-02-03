#include "CtrlHomeKeyAction.h"
#include "Glyph.h"
#include "SelectText.h"

//����Ʈ������
CtrlHomeKeyAction::CtrlHomeKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ��Ʈ���� ���� ���� ó������ �̵���Ų��.
	Long firstRowIndex = this->notepadForm->note->First();
	//2. ���� ���� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(firstRowIndex);
	//3. ���� ���� �� ó������ ĳ���� ��ġ�� �̵���Ų��.
	this->notepadForm->current->First();
	//4. ���� ������ �����Ѵ�.
	SelectText selectText(this->notepadForm);
	selectText.Undo();
}

//�Ҹ���
CtrlHomeKeyAction::~CtrlHomeKeyAction()
{

}