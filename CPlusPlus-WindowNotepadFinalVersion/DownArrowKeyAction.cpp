#include "DownArrowKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "TextExtent.h"

//����Ʈ������
DownArrowKeyAction::DownArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void DownArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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
	//2. �������� �̵��ϱ� ���� ���� ��ġ�� �����Ѵ�.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//3. �������� �̵��ϱ� ���� ������ ���� ��ġ�� �����Ѵ�.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//4. ���� �ٷ� �̵��ϰ�, ���� ���� ��ġ�� ������ġ�� �����Ѵ�.
	Long currentRowIndex = this->notepadForm->note->Next();
	Long currentLetterIndex= this->notepadForm->current->GetCurrent();
	//5. �������� �̵��ϱ� �� ���� ��ġ�� �̵��� �� ���� ��ġ�� ���� �ٸ���(���������� �̵��� ������)
	if (previousRowIndex != currentRowIndex)
	{
		//5.1 ���� ���� �̵��� ���� �ٷ� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
		//5.2 �������� �̵��ϱ� �� ĳ���� ���� ��ġ�� 0�� �ƴϰ� �̵��� �� �������� ���ڰ����� 0�� �ƴϸ�
		//(�������� �̵��ϱ� ���� ĳ���� ��ġ�� 0�̸� �������� �̵��� ���� ���� ĳ���� ��ġ�� ������ 0�̰�,
		//�̵��� �� ���� ���� ���ڰ����� 0�̾ ���� ĳ���� ���� ��ġ�� ������ 0�̱� ������
		//�� �̿��� ����� ���� if�ȿ��� ó���ϰ� else���� ���� 2��츦 First�� ó���ϸ� �ȴ�!)
		if (previousLetterIndex != 0 && this->notepadForm->current->GetLength() != 0)
		{
			//5.2.1 �̵��ϱ� �� ���� ���� ĳ���� ��ġ���� �ؽ�Ʈ ���� ���Ѵ�.
			Long previousRowTextWidth = this->notepadForm->textExtent->
				GetTextWidth(this->notepadForm->note->GetAt(previousRowIndex)
					->GetPartOfContent(previousLetterIndex));
			//5.2.2 ĳ���� ���� ��ġ�� ó������ �̵���Ų��.
			this->notepadForm->current->First();//ĳ���� ��ġ�� �� ó��(0)���� ����
			//5.2.3 ù��° ���ڸ� �б� ���� ĳ���� ��ó������ �������� �̵���Ų��.
			Long i = this->notepadForm->current->Next();
			//5.2.4 ĳ���� ���� ��ġ���� �ؽ�Ʈ�� ���� ���Ѵ�.
			Long currentRowTextWidth = this->notepadForm->textExtent->
				GetTextWidth(this->notepadForm->current->GetPartOfContent(i).c_str());
			//5.2.5 ĳ���� ���� ��ġ(i)�� ���� ���� ���ڰ���(length)���� �۰�
			//ĳ���� ���� ��ġ���� �ؽ�Ʈ ũ�Ⱑ �̵��ϱ� �� ���� �ؽ�Ʈ ũ�⺸�� �������� �ݺ��Ѵ�.
			while (i < this->notepadForm->current->GetLength()
				&& currentRowTextWidth < previousRowTextWidth)
			{
				//5.2.5.1 ĳ���� ��ġ�� ���� ĭ���� �̵���Ų��.
				i = this->notepadForm->current->Next();
				//5.2.5.2 ĳ���� ��ġ������ �ؽ�Ʈ ���� ���Ѵ�.
				currentRowTextWidth = this->notepadForm->textExtent->
					GetTextWidth(this->notepadForm->current->GetPartOfContent(i).c_str());
			}
			//5.2.6 ĳ���� ��ġ������ �ؽ�Ʈ ������ ���� ���� �ؽ�Ʈ ���� ����.
			Long difference = currentRowTextWidth - previousRowTextWidth;
			//5.2.7 ���̰� 0���� ũ��(�������� �ؽ�Ʈ�� �� ���� ���� ���)
			if (difference > 0)
			{
				//5.2.7.1 ĳ���� ���� ������ġ ������ ���� ���� ���Ѵ�.
				Long letterWidth = this->notepadForm->textExtent->
					GetTextWidth(this->notepadForm->current->GetAt(i - 1)->GetContent());
				Long halfLetterSize = letterWidth / 2;
				//5.2.7.2 ���̰� ���� ����ũ���� ���ݺ��� ���ų� ũ��
				if (difference >= halfLetterSize)
				{
					//5.2.7.2.1 ĳ���� ���� ���� ��ġ�� �������� �̵��Ѵ�.
					currentLetterIndex= this->notepadForm->current->Previous();
				}
			}
		}
		//5.3 ĳ���� ���� ���� ��ġ�� 0 �Ǵ� ĳ���� ���� ���� ���ڰ����� 0���̸�
		else
		{
			//5.3.1 ���� ĳ���� ���� ��ġ�� 0���� �̵���Ų��.
			currentLetterIndex = this->notepadForm->current->First();
		}
	}
}

//�Ҹ���
DownArrowKeyAction::~DownArrowKeyAction()
{

}