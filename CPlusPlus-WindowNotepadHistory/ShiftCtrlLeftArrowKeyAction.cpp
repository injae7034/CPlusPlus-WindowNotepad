#include "ShiftCtrlLeftArrowKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"

//����Ʈ������
ShiftCtrlLeftArrowKeyAction::ShiftCtrlLeftArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void ShiftCtrlLeftArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. �̵��ϱ� ���� ���� ��ġ�� ���Ѵ�.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//2. �̵��ϱ� ���� ������ ��ġ�� ���Ѵ�.
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
	//4. ��Ʈ���� �ܾ������ �������� �̵��Ѵ�.
	Long currentLetterIndex = this->notepadForm->note->PreviousWord();
	//5. �̵��� �Ŀ� ���� ��ġ�� ���Ѵ�.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//6. ���� ���� �̵��� ���� �ٷ� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	//7. �ٿ��� ĳ���� �ܾ������ �̵��Ѵ�.
	this->notepadForm->current->Move(currentLetterIndex);
	//8. �ؽ�Ʈ�� �����Ѵ�.
	this->notepadForm->selectingTexts->DoPrevious(previousRowIndex, previousLetterIndex,
		currentRowIndex, currentLetterIndex);
}

//�Ҹ���
ShiftCtrlLeftArrowKeyAction::~ShiftCtrlLeftArrowKeyAction()
{

}