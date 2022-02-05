#include "RightArrowKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "DummyRow.h"

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
	//5. ���� ���� ������ ���� �ƴϸ�
	Long lastRowIndex = this->notepadForm->note->GetLength() - 1;
	if (currentRowIndex < lastRowIndex)
	{
		//5.1 ���� ���� ���Ѵ�.
		Glyph* nextRow = this->notepadForm->note->GetAt(currentRowIndex + 1);
		//5.2 ���� ���� ��¥ ���̸�
		if (dynamic_cast<DummyRow*>(nextRow))
		{
			//5.2.1 ���� ������ġ�� ������ �������� ������
			Long letterIndex = this->notepadForm->current->GetLength();
			if (currentLetterIndex == letterIndex)
			{
				//5.2.1 ���� �ٷ� �̵���Ų��.
				currentRowIndex = this->notepadForm->note->Next();
				//5.2.2 ���� ���� �������� �̵��� �ٷ� �����Ѵ�.
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
				//5.2.3 ĳ���� ���� ��ġ�� ó������ ������.(���� ���� ���� �ٷ� �Ű����� ������
				//ĳ���� ���� ���� ������ ��ġ���� ���� ���� ó�� ��ġ�� �̵��Ѵ�.)
				currentLetterIndex = this->notepadForm->current->First();
			}
		}
		//5.3 ���� ���� ��¥ ���̸�
		else
		{
			//5.3.1 ĳ���� �̵��� ��������(ĳ���� ���� �������� ��ġ�ϰ� �־��� ������)
			if (previousLetterIndex == currentLetterIndex)
			{
				//5.3.1.1 ���� �ٷ� �̵���Ų��.
				currentRowIndex = this->notepadForm->note->Next();
				//5.3.1.2 ���� �̵��� �־�����(�̵��ϱ� ���� ���� ��Ʈ�� ������ ���� �ƴϸ�)
				if (previousRowIndex != currentRowIndex)
				{
					//5.3.1.2.1 ���� ���� �������� �̵��� �ٷ� �����Ѵ�.
					this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
					//5.3.1.2.2 ĳ���� ���� ��ġ�� ó������ ������.(���� ���� ���� �ٷ� �Ű����� ������
					//ĳ���� ���� ���� ������ ��ġ���� ���� ���� ó�� ��ġ�� �̵��Ѵ�.)
					currentLetterIndex = this->notepadForm->current->First();
				}
			}
		}
	}
	
}

//�Ҹ���
RightArrowKeyAction::~RightArrowKeyAction()
{

}

