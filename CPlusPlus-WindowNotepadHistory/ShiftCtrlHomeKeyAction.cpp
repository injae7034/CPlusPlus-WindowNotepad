#include "ShiftCtrlHomeKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"

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
	//4. ���� ó�� �ٷ� �̵��Ѵ�.
	Long currentRowIndex = this->notepadForm->note->First();
	//5. ���� ���� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	//6. ���� ó�� ���ڷ� �̵��Ѵ�.
	Long currentLetterIndex = this->notepadForm->current->First();
	//7. ���ڸ� �����Ѵ�.
	this->notepadForm->selectingTexts->DoPrevious(previousRowIndex, previousLetterIndex,
		currentRowIndex, currentLetterIndex);
}

//�Ҹ���
ShiftCtrlHomeKeyAction::~ShiftCtrlHomeKeyAction()
{

}