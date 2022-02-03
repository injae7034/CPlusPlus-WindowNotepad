#include "ShiftCtrlHomeKeyAction.h"
#include "Glyph.h"
#include "SelectText.h"

//����Ʈ������
ShiftCtrlHomeKeyAction::ShiftCtrlHomeKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void ShiftCtrlHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ó�� �ٷ� �̵��ϱ� ���� ���� ��ġ�� �����Ѵ�.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//2. ó�� ���ڷ� �̵��ϱ� ���� ������ ��ġ�� �����Ѵ�.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//3. ���� ó�� �ٷ� �̵��Ѵ�.
	Long currentRowIndex = this->notepadForm->note->First();
	//4. ���� ���� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	//5. ���� ó�� ���ڷ� �̵��Ѵ�.
	Long currentLetterIndex = this->notepadForm->current->First();
	//6. ���ڸ� �����Ѵ�.
	SelectText selectText(this->notepadForm);
	selectText.DoPrevious(previousRowIndex, previousLetterIndex, currentRowIndex,
		currentLetterIndex);
}

//�Ҹ���
ShiftCtrlHomeKeyAction::~ShiftCtrlHomeKeyAction()
{

}