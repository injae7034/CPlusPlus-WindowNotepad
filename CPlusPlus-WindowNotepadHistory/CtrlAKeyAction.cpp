#include "CtrlAKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"

//����Ʈ������
CtrlAKeyAction::CtrlAKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void CtrlAKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ������ �ִ� ���� ������ �����Ѵ�.
	this->notepadForm->selectingTexts->Undo();
	//2. ������ �ٷ� �̵��ϱ� ���� ���� ��ġ�� �����Ѵ�.
	Long previousRowIndex = 0;
	//3. ���������� ���ڷ� �̵��ϱ� ���� ������ ��ġ�� �����Ѵ�.
	Long previousLetterIndex = 0;
	//4. ������ ����ǰ� �ִ� ������ ���¸� �ٲ۴�.
	this->notepadForm->isSelecting = true;
	//5 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
	this->notepadForm->selectedStartXPos = previousLetterIndex;
	//6. ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
	this->notepadForm->selectedStartYPos = previousRowIndex;
	//7. �޸����� ���� ������ �ٷ� �̵��Ѵ�.
	Long currentRowIndex = this->notepadForm->note->Last();
	//8. �޸����� ���� ���� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	//9. ���� ���� ������ ���ڷ� �̵��Ѵ�.
	Long currentLetterIndex = this->notepadForm->current->Last();
	//10. ��ü ���ڸ� �����Ѵ�.
	this->notepadForm->selectingTexts->DoNext(previousRowIndex, previousLetterIndex,
		currentRowIndex, currentLetterIndex);
}

//�Ҹ���
CtrlAKeyAction::~CtrlAKeyAction()
{

}