#include "ShiftHomeKeyAction.h"
#include "Glyph.h"
#include "SelectText.h"

//����Ʈ������
ShiftHomeKeyAction::ShiftHomeKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void ShiftHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ������ġ�� ���������� �̵��ϱ� ���� ���� ��ġ�� �����Ѵ�.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//2. ������ġ�� ���������� �̵��ϱ� ���� ������ ���� ��ġ�� �����Ѵ�.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//1. ���� ���� �� ó������ ������ ��ġ�� �̵���Ų��.
	Long currentLetterIndex = this->notepadForm->current->First();
	//4. �̵� �� ���� ��ġ�� �����Ѵ�.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//5. ���ڸ� �����Ѵ�.
	SelectText selectText(this->notepadForm);
	selectText.DoPrevious(previousRowIndex, previousLetterIndex, currentRowIndex,
		currentLetterIndex);
}

//�Ҹ���
ShiftHomeKeyAction::~ShiftHomeKeyAction()
{

}