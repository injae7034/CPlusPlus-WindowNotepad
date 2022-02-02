#include "CtrlLeftArrowKeyAction.h"
#include "Glyph.h"

//����Ʈ������
CtrlLeftArrowKeyAction::CtrlLeftArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlLeftArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. Note�� PreviousWord�� ȣ���Ѵ�.
	Long currentCaretindex = this->notepadForm->note->PreviousWord();
	//2. �������� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(this->notepadForm->note->GetCurrent());
	//3. �������� ĳ���� ���� ��ġ�� �����Ѵ�.
	Long i = this->notepadForm->current->First();
	while (i < currentCaretindex)
	{
		//3.1 ĳ���� �������� �̵���Ų��.
		i = this->notepadForm->current->Next();
	}
}

//�Ҹ���
CtrlLeftArrowKeyAction::~CtrlLeftArrowKeyAction()
{

}