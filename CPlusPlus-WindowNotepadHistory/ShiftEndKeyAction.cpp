#include "ShiftEndKeyAction.h"
#include "Glyph.h"
#include "SelectText.h"

//����Ʈ������
ShiftEndKeyAction::ShiftEndKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void ShiftEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ������ġ�� ���������� �̵��ϱ� ���� ���� ��ġ�� �����Ѵ�.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//2. ������ġ�� ���������� �̵��ϱ� ���� ������ ���� ��ġ�� �����Ѵ�.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//3. ���� ���� �� ���������� ������ ��ġ�� �̵���Ų��.
	Long currentLetterIndex = this->notepadForm->current->Last();
	//4. �̵� �� ���� ��ġ�� �����Ѵ�.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//5. ���ڸ� �����Ѵ�.
	SelectText selectText(this->notepadForm);
	selectText.DoNext(previousRowIndex, previousLetterIndex, currentRowIndex,
		currentLetterIndex);
}

//�Ҹ���
ShiftEndKeyAction::~ShiftEndKeyAction()
{

}