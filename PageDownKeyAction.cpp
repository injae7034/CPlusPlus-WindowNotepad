#include "PageDownKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"
#include "SelectingTexts.h"

//����Ʈ������
PageDownKeyAction::PageDownKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void PageDownKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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
	//2. ĳ���� �̵���Ű�� ���� ���� ���� ĳ���� �ִ� ������ ��ũ���� �̵���Ų��.
	this->notepadForm->Notify();
	//��ũ���� ĳ���� �ִ� ������ �̵���Ų ĳ���� �ִ� ���� ���ػ�� �ٽ� ��ũ���� ������������ �̵���Ų��.
	//3. ������ ������ �̵��ϱ� ���� ������ũ���� ���� ��ġ�� �����Ѵ�.
	Long previousVPos = this->notepadForm->scrollController->scroll[1]->GetCurrentPos();
	//4. ������ũ���� ���� �������� �̵���Ű�� �̵��� ������ũ���� ���� ��ġ�� ��ȯ�޴´�.
	Long currentVPos = this->notepadForm->scrollController->scroll[1]->PageNext();
	//5. ������ũ�ѿ� �̵��� ������(previousVPos�� currentVPos�� �ٸ��� �̵��� �־��ٴ� ����)
	if (previousVPos != currentVPos)
	{
		//5.1 ������ũ�ѹ��� ������ũ���� ������ũ���� ���� ��ġ�� �̵���Ų��.
		this->notepadForm->SetScrollPos(SB_VERT,
			this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
		//5.2 �޸��忡�� ĳ���� �̵��� �Ÿ��� ���Ѵ�.
		Long distance = currentVPos - previousVPos;
		//5.3 ĳ���� �̵��� ���� ������ ���Ѵ�.
		Long MovingRowCount = distance / this->notepadForm->textExtent->GetHeight();
		//5.4 �̵��ϱ� ���� ���� ���� ��ġ�� ���Ѵ�.
		Long previousRowIndex = this->notepadForm->note->GetCurrent();
		//5.5 �̵��ϱ� ���� ĳ���� ���� ��ġ�� ���Ѵ�.
		Long previousCaretIndex = this->notepadForm->current->GetCurrent();
		//5.6 ĳ���� ���� ��ġ�� �̵���Ų��.
		Long currentRowIndex = previousRowIndex + MovingRowCount;
		currentRowIndex = this->notepadForm->note->Move(currentRowIndex);
		//5.7 �̵��� ���� �������� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
		//5.8 �̵��ϱ� �� ĳ���� ���� ��ġ�� 0�� �ƴϰ� �̵��� �� �������� ���ڰ����� 0�� �ƴϸ�
		if (previousCaretIndex != 0 && this->notepadForm->current->GetLength() != 0)
		{
			//5.8.1 �̵��ϱ� �� ���� �ؽ�Ʈ ���� ���Ѵ�.
			Long previousRowTextWidth = this->notepadForm->textExtent->
				GetTextWidth(this->notepadForm->note->GetAt(previousRowIndex)
					->GetPartOfContent(previousCaretIndex));
			//5.8.2 ĳ���� ���� ���� ��ġ�� ó������ �̵���Ų��.
			this->notepadForm->current->First();//ĳ���� ���� ��ġ�� �� ó��(0)���� ����
			//5.8.3 ù��° ���ڸ� �б� ���� ĳ���� 1 ������Ų��.
			Long i = this->notepadForm->current->Next();
			//5.8.4 ���� ���� �ý�Ʈ�� ���� ���Ѵ�.
			Long currentRowTextWidth = this->notepadForm->textExtent->
				GetTextWidth(this->notepadForm->current->GetPartOfContent(i).c_str());
			//5.8.5 i(ĳ���� ���� ������ġ)�� length(�������� ���ڰ���)���� �۰�
			//���� ���� �ؽ�Ʈ ũ�Ⱑ �̵��ϱ� �� ���� �ؽ�Ʈ ũ�⺸�� �������� �ݺ��Ѵ�.
			while (i < this->notepadForm->current->GetLength()
				&& currentRowTextWidth < previousRowTextWidth)
			{
				//5.8.5.1 i(ĳ���� ���� ������ġ)�� ������Ų��
				//(ĳ���� ���� ��ġ�� ���� ĭ���� �̵���Ų��.).
				i = this->notepadForm->current->Next();
				//5.8.5.2 ���� ���� ĳ���� ���� ��ġ������ �ý�Ʈ ���� ���Ѵ�.
				currentRowTextWidth = this->notepadForm->textExtent->
					GetTextWidth(this->notepadForm->current->GetPartOfContent(i).c_str());
			}
			//5.8.6 ���� ���� �ؽ�Ʈ ������ ���� ���� �ؽ�Ʈ ���� ����.
			Long difference = currentRowTextWidth - previousRowTextWidth;
			//5.8.7 ���̰� 0���� ũ��(�������� �ؽ�Ʈ�� �� ���� ���� ���)
			if (difference > 0)
			{
				//5.8.7.1 ĳ���� ���� ������ġ ������ ���� ���� ���Ѵ�.
				Long letterWidth = this->notepadForm->textExtent->
					GetTextWidth(this->notepadForm->current->GetAt(i - 1)->GetContent());
				Long halfLetterSize = letterWidth / 2;
				//5.8.7.2 ���̰� ���� ����ũ���� ���ݺ��� ���ų� ũ��
				if (difference >= halfLetterSize)
				{
					//5.8.7.2.1 ĳ���� ���� ���� ��ġ�� �������� �̵��Ѵ�.
					this->notepadForm->current->Previous();
				}
			}
		}
		//5.9 ĳ���� ���� ���� ��ġ�� 0 �Ǵ� ĳ���� ���� ���� ���ڰ����� 0���̸�
		else
		{
			//5.9.1 ���� ĳ���� ���� ��ġ�� 0���� �̵���Ų��.
			this->notepadForm->current->First();
		}
	}
}

//�Ҹ���
PageDownKeyAction::~PageDownKeyAction()
{

}