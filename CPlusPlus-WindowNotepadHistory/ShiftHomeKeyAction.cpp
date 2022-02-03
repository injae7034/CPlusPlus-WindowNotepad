#include "ShiftHomeKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"

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
	//4. ���� ���� �� ó������ ������ ��ġ�� �̵���Ų��.
	Long currentLetterIndex = this->notepadForm->current->First();
	//5. �̵� �� ���� ��ġ�� �����Ѵ�.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//6. ���ڸ� �����Ѵ�.
	this->notepadForm->selectingTexts->DoPrevious(previousRowIndex, previousLetterIndex,
		currentRowIndex, currentLetterIndex);
}

//�Ҹ���
ShiftHomeKeyAction::~ShiftHomeKeyAction()
{

}