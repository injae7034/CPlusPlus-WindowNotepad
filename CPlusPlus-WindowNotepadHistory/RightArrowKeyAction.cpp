#include "RightArrowKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"

//����Ʈ������
RightArrowKeyAction::RightArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void RightArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ������ ����ǰ� �ִ� ���̾�����
	if (this->notepadForm->isSelecting == true)
	{
		//1.1. ���õ� �ؽ�Ʈ�� ���������Ѵ�.(������ ������.)
		this->notepadForm->selectingTexts->Undo();
		//1.2 ������ ���� ���·� �ٲ۴�.
		this->notepadForm->isSelecting = false;
		//1.3 ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
		this->notepadForm->selectedStartXPos = 0;
		//1.4 ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
		this->notepadForm->selectedStartYPos = 0;
	}
	//2. �������� �̵��ϱ� ���� ������ ��ġ�� �����Ѵ�.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//3. �������� �̵��ϱ� ���� ���� ��ġ�� �����Ѵ�.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//4. �������� �̵��ϰ� ���� ������ ��ġ�� ��ȯ�޴´�.
	Long currentLetterIndex = this->notepadForm->current->Next();
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//5. ĳ���� �̵��� ��������(ĳ���� ���� �������� ��ġ�ϰ� �־��� ������)
	if (previousLetterIndex == currentLetterIndex)
	{
		//5.1 ���� �ٷ� �̵���Ų��.
		currentRowIndex = this->notepadForm->note->Next();
		//5.2 ���� �̵��� �־�����(�̵��ϱ� ���� ���� ��Ʈ�� ������ ���� �ƴϸ�)
		if (previousRowIndex != currentRowIndex)
		{
			//5.2.1 ���� ���� �������� �̵��� �ٷ� �����Ѵ�.
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
			//5.2.2 ĳ���� ���� ��ġ�� ó������ ������.(���� ���� ���� �ٷ� �Ű����� ������
			//ĳ���� ���� ���� ������ ��ġ���� ���� ���� ó�� ��ġ�� �̵��Ѵ�.)
			currentLetterIndex = this->notepadForm->current->First();
		}
	}
}

//�Ҹ���
RightArrowKeyAction::~RightArrowKeyAction()
{

}

