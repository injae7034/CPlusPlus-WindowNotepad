#include "ShiftEndKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"

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
	//3. ������ ó�� ���۵Ǹ�
	if (this->notepadForm->isSelecting == false)
	{
		//3.1 ������ ����ǰ� �ִ� ������ ���¸� �ٲ۴�.
		this->notepadForm->isSelecting = true;
		//3.2 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
		this->notepadForm->selectedStartXPos = previousLetterIndex;
		//3.3 ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
		this->notepadForm->selectedStartYPos = previousRowIndex;
	}
	//4. ���� ���� �� ���������� ������ ��ġ�� �̵���Ų��.
	Long currentLetterIndex = this->notepadForm->current->Last();
	//5. �̵� �� ���� ��ġ�� �����Ѵ�.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//6. ���ڸ� �����Ѵ�.
	this->notepadForm->selectingTexts->DoNext(previousRowIndex, previousLetterIndex,
		currentRowIndex, currentLetterIndex);
}

//�Ҹ���
ShiftEndKeyAction::~ShiftEndKeyAction()
{

}