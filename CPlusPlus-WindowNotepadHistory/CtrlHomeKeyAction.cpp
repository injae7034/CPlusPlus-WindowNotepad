#include "CtrlHomeKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"

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
}

//�Ҹ���
CtrlHomeKeyAction::~CtrlHomeKeyAction()
{

}