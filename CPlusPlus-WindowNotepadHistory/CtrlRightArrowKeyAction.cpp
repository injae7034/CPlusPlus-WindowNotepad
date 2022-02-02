#include "CtrlRightArrowKeyAction.h"
#include "Glyph.h"

//����Ʈ������
CtrlRightArrowKeyAction::CtrlRightArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlRightArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. Note�� NextWord�� ȣ���Ѵ�.
	Long currentCaretindex = this->notepadForm->note->NextWord();
	//2. �������� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(this->notepadForm->note->GetCurrent());
	//3. �������� ĳ���� ���� ��ġ�� �����Ѵ�.
	Long i = this->notepadForm->current->First();
	while (i < currentCaretindex)
	{
		i = this->notepadForm->current->Next();
	}
}

//�Ҹ���
CtrlRightArrowKeyAction::~CtrlRightArrowKeyAction()
{

}