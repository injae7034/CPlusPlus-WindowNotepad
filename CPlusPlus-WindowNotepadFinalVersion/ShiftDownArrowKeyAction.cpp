#include "ShiftDownArrowKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "TextExtent.h"

//����Ʈ������
ShiftDownArrowKeyAction::ShiftDownArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void ShiftDownArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. �������� �̵��ϱ� ���� ���� ��ġ�� �����Ѵ�.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//2. �������� �̵��ϱ� ���� ĳ���� ���� ��ġ�� �����Ѵ�.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//3. ���� �ٷ� �̵��ϰ�, ���� ���� ��ġ�� ������ġ�� �����Ѵ�.
	Long currentRowIndex = this->notepadForm->note->Next();
	Long currentLetterIndex = this->notepadForm->current->GetCurrent();
	//4. �������� �̵��ϱ� �� ���� ��ġ�� �̵��� �� ���� ��ġ�� ���� �ٸ���(���������� �̵��� ������)
	if (previousRowIndex < currentRowIndex)
	{
		//4.1 ���� ���� �̵��� ���� �ٷ� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
		//4.2 �������� �̵��ϱ� �� ĳ���� ���� ��ġ�� 0�� �ƴϰ� �̵��� �� �������� ���ڰ����� 0�� �ƴϸ�
		//(�������� �̵��ϱ� ���� ĳ���� ��ġ�� 0�̸� �������� �̵��� ���� ���� ĳ���� ��ġ�� ������ 0�̰�,
		//�̵��� �� ���� ���� ���ڰ����� 0�̾ ���� ĳ���� ���� ��ġ�� ������ 0�̱� ������
		//�� �̿��� ����� ���� if�ȿ��� ó���ϰ� else���� ���� 2��츦 First�� ó���ϸ� �ȴ�!)
		if (previousLetterIndex != 0 && this->notepadForm->current->GetLength() != 0)
		{
			//4.2.1 �̵��ϱ� �� ���� ���� ĳ���� ��ġ���� �ؽ�Ʈ ���� ���Ѵ�.
			Long previousRowTextWidth = this->notepadForm->textExtent->
				GetTextWidth(this->notepadForm->note->GetAt(previousRowIndex)
					->GetPartOfContent(previousLetterIndex));
			//4.2.2 ĳ���� ���� ��ġ�� ó������ �̵���Ų��.
			this->notepadForm->current->First();//ĳ���� ��ġ�� �� ó��(0)���� ����
			//4.2.3 ù��° ���ڸ� �б� ���� ĳ���� ��ó������ �������� �̵���Ų��.
			currentLetterIndex = this->notepadForm->current->Next();
			//4.2.4 ĳ���� ���� ��ġ���� �ؽ�Ʈ�� ���� ���Ѵ�.
			Long currentRowTextWidth = this->notepadForm->textExtent->
				GetTextWidth(this->notepadForm->current->GetPartOfContent
				(currentLetterIndex).c_str());
			//4.2.5 ĳ���� ���� ��ġ(i)�� ���� ���� ���ڰ���(length)���� �۰�
			//ĳ���� ���� ��ġ���� �ؽ�Ʈ ũ�Ⱑ �̵��ϱ� �� ���� �ؽ�Ʈ ũ�⺸�� �������� �ݺ��Ѵ�.
			while (currentLetterIndex < this->notepadForm->current->GetLength()
				&& currentRowTextWidth < previousRowTextWidth)
			{
				//4.2.5.1 ĳ���� ��ġ�� ���� ĭ���� �̵���Ų��.
				currentLetterIndex = this->notepadForm->current->Next();
				//4.2.5.2 ĳ���� ��ġ������ �ؽ�Ʈ ���� ���Ѵ�.
				currentRowTextWidth = this->notepadForm->textExtent->
					GetTextWidth(this->notepadForm->current->GetPartOfContent
					(currentLetterIndex).c_str());
			}
			//4.2.6 ĳ���� ��ġ������ �ؽ�Ʈ ������ ���� ���� �ؽ�Ʈ ���� ����.
			Long difference = currentRowTextWidth - previousRowTextWidth;
			//4.2.7 ���̰� 0���� ũ��(�������� �ؽ�Ʈ�� �� ���� ���� ���)
			if (difference > 0)
			{
				//4.2.7.1 ĳ���� ���� ������ġ ������ ���� ���� ���Ѵ�.
				Long letterWidth = this->notepadForm->textExtent->
					GetTextWidth(this->notepadForm->current->
						GetAt(currentLetterIndex - 1)->GetContent());
				Long halfLetterSize = letterWidth / 2;
				//4.2.7.2 ���̰� ���� ����ũ���� ���ݺ��� ���ų� ũ��
				if (difference >= halfLetterSize)
				{
					//4.2.7.2.1 ĳ���� ���� ���� ��ġ�� �������� �̵��Ѵ�.
					currentLetterIndex = this->notepadForm->current->Previous();
				}
			}
		}
		//4.3 ĳ���� ���� ���� ��ġ�� 0 �Ǵ� ĳ���� ���� ���� ���ڰ����� 0���̸�
		else
		{
			//4.3.1 ���� ĳ���� ���� ��ġ�� 0���� �̵���Ų��.
			currentLetterIndex = this->notepadForm->current->First();
		}
		//4.4 ������ ó�� ���۵Ǹ�
		if (this->notepadForm->isSelecting == false)
		{
			//4.4.1 ������ ����ǰ� �ִ� ������ ���¸� �ٲ۴�.
			this->notepadForm->isSelecting = true;
			//4.4.2 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
			this->notepadForm->selectedStartXPos = previousLetterIndex;
			//4.4.3 ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
			this->notepadForm->selectedStartYPos = previousRowIndex;
		}
	}
	//5. ������ ���� ���̸�
	if (this->notepadForm->isSelecting == true)
	{
		//5.1 �ؽ�Ʈ�� �����Ѵ�.
		this->notepadForm->selectingTexts->DoNext(previousRowIndex, previousLetterIndex,
			currentRowIndex, currentLetterIndex);
	}
}

//�Ҹ���
ShiftDownArrowKeyAction::~ShiftDownArrowKeyAction()
{

}