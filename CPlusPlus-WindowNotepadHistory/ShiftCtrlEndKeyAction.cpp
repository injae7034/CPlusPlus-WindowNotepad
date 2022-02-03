#include "ShiftCtrlEndKeyAction.h"
#include "Glyph.h"
#include "SelectText.h"

//����Ʈ������
ShiftCtrlEndKeyAction::ShiftCtrlEndKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void ShiftCtrlEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ������ �ٷ� �̵��ϱ� ���� ���� ��ġ�� �����Ѵ�.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//2. ���������� ���ڷ� �̵��ϱ� ���� ������ ��ġ�� �����Ѵ�.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//3. �޸����� ���� ������ �ٷ� �̵��Ѵ�.
	Long currentRowIndex = this->notepadForm->note->Last();
	//4. �޸����� ���� ���� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	//5. ���� ���� ������ ���ڷ� �̵��Ѵ�.
	Long currentLetterIndex = this->notepadForm->current->Last();
	//6. ���ڸ� �����Ѵ�.
	SelectText selectText(this->notepadForm);
	selectText.DoNext(previousRowIndex, previousLetterIndex, currentRowIndex,
		currentLetterIndex);
}

//�Ҹ���
ShiftCtrlEndKeyAction::~ShiftCtrlEndKeyAction()
{

}