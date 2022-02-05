#include "LeftArrowKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "DummyRow.h"

//����Ʈ������
LeftArrowKeyAction::LeftArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void LeftArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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
	//4. �������� �̵��ϰ� ĳ���� ��ġ�� ��ȯ�޴´�.
	Long currentLetterIndex = this->notepadForm->current->Previous();
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//5. ĳ���� �̵��� ��������(ĳ���� ���� ���� ó���� �־ ���̻� �̵��� ���� ����.)
	if (previousLetterIndex == currentLetterIndex)
	{
		//5.1 ���� �ٷ� �̵���Ų��.
		currentRowIndex = this->notepadForm->note->Previous();
		//5.2 ���� �̵��� �־�����(�̵��ϱ� ���� ���� ��Ʈ�� ó�� ���� �ƴϸ�)
		if (previousRowIndex > currentRowIndex)
		{
			//5.2.1 ���� ���� �������� �̵��� �ٷ� �����Ѵ�.
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
			//5.2.2 ���� ���� ���Ѵ�.
			Glyph* currentRow = this->notepadForm->current;
			//5.2.3 ���� ���� ���Ѵ�.
			Glyph* nextRow = this->notepadForm->note->GetAt(currentRowIndex + 1);
			//5.2.4 ���� ���� ��¥ ���̰� ���� ���� ��¥ ���̸�
			if (!dynamic_cast<DummyRow*>(nextRow) && dynamic_cast<DummyRow*>(currentRow))
			{
				//5.2.4.1 ���� �������� �̵��߱� ������ ĳ���� ���� ��ġ�� ���������� �����Ѵ�.
				currentLetterIndex = this->notepadForm->current->Last();
			}
			//5.2.5 ���� ���� ��¥ ���̰� ���� ���� ��¥ ���̸�
			else if (!dynamic_cast<DummyRow*>(nextRow) && !dynamic_cast<DummyRow*>(currentRow))
			{
				//5.2.5.1 ���� �������� �̵��߱� ������ ĳ���� ���� ��ġ�� ���������� �����Ѵ�.
				currentLetterIndex = this->notepadForm->current->Last();
			}
			//5.2.6 �� �̿ܿ���
			else
			{
				//5.2.6.1 ���� �������� �̵��߱� ������ ĳ���� ���� ��ġ�� ���������� �����Ѵ�.
				currentLetterIndex = this->notepadForm->current->Last();
				//5.2.6.2 �������� �̵��� ���� ��¥ ���̱� ������ ������ ������ġ �������� �̵��Ѵ�.
				currentLetterIndex = this->notepadForm->current->Previous();
			}
		}
	}
}

//�Ҹ���
LeftArrowKeyAction::~LeftArrowKeyAction()
{

}