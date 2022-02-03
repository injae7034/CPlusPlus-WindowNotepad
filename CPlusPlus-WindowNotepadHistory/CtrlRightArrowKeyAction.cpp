#include "CtrlRightArrowKeyAction.h"
#include "Glyph.h"

#include "SelectText.h"

//����Ʈ������
CtrlRightArrowKeyAction::CtrlRightArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlRightArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. �̵��ϱ� ���� ���� ��ġ�� ���Ѵ�.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//2. �̵��ϱ� ���� ������ ��ġ�� ���Ѵ�.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//3. ��Ʈ���� �ܾ������ ���������� �̵��Ѵ�.
	Long currentLetterIndex = this->notepadForm->note->NextWord();
	//4. �̵��� �Ŀ� ���� ��ġ�� ���Ѵ�.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//5. ���� ���� �̵��� ���� �ٷ� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	//6. �ٿ��� ĳ���� �ܾ������ �̵��Ѵ�.
	this->notepadForm->current->Move(currentLetterIndex);
	//7. �ؽ�Ʈ�� ���������Ѵ�.
	SelectText selectText(this->notepadForm);
	selectText.Undo();
}

//�Ҹ���
CtrlRightArrowKeyAction::~CtrlRightArrowKeyAction()
{

}