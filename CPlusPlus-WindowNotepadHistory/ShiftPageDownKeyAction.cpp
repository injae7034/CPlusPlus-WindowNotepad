#include "ShiftPageDownKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"
#include "SelectText.h"

//����Ʈ������
ShiftPageDownKeyAction::ShiftPageDownKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void ShiftPageDownKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ĳ���� �̵���Ű�� ���� ���� ���� ĳ���� �ִ� ������ ��ũ���� �̵���Ų��.
	this->notepadForm->Notify();

	//��ũ���� ĳ���� �ִ� ������ �̵���Ų ĳ���� �ִ� ���� ���ػ�� �ٽ� ��ũ���� ������������ �̵���Ų��.
	//2. ������ ������ �̵��ϱ� ���� ������ũ���� ���� ��ġ�� �����Ѵ�.
	Long previousVPos = this->notepadForm->scrollController->scroll[1]->GetCurrentPos();
	//3. ������ũ���� ���� �������� �̵���Ű�� �̵��� ������ũ���� ���� ��ġ�� ��ȯ�޴´�.
	Long currentVPos = this->notepadForm->scrollController->scroll[1]->PageNext();
	//4. ������ũ�ѿ� �̵��� ������(previousVPos�� currentVPos�� �ٸ��� �̵��� �־��ٴ� ����)
	if (previousVPos != currentVPos)
	{
		//4.1 ������ũ�ѹ��� ������ũ���� ������ũ���� ���� ��ġ�� �̵���Ų��.
		this->notepadForm->SetScrollPos(SB_VERT,
			this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
		//4.2 �޸��忡�� ĳ���� �̵��� �Ÿ��� ���Ѵ�.
		Long distance = currentVPos - previousVPos;
		//4.3 ĳ���� �̵��� ���� ������ ���Ѵ�.
		Long MovingRowCount = distance / this->notepadForm->textExtent->GetHeight();
		//4.4 �̵��ϱ� ���� ���� ��ġ�� ���Ѵ�.
		Long previousRowIndex = this->notepadForm->note->GetCurrent();
		//4.5 �̵��ϱ� ���� ���� ��ġ�� ���Ѵ�.
		Long previousLetterIndex = this->notepadForm->current->GetCurrent();
		//4.6 ���� ���� ��ġ�� �̵���Ų��.
		Long currentRowIndex = previousRowIndex + MovingRowCount;
		currentRowIndex = this->notepadForm->note->Move(currentRowIndex);
		//4.7 �̵��� ���� �������� �����ϰ� ���� ���� ��ġ�� ���Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
		Long currentLetterIndex = this->notepadForm->current->GetCurrent();
		//4.8 �̵��ϱ� �� ���� ��ġ�� 0�� �ƴϰ� �̵��� �� �������� ���ڰ����� 0�� �ƴϸ�
		if (previousLetterIndex != 0 && this->notepadForm->current->GetLength() != 0)
		{
			//4.8.1 �̵��ϱ� �� ���� �ؽ�Ʈ ���� ���Ѵ�.
			Long previousRowTextWidth = this->notepadForm->textExtent->
				GetTextWidth(this->notepadForm->note->GetAt(previousRowIndex)
					->GetPartOfContent(previousLetterIndex));
			//4.8.2 ĳ���� ���� ���� ��ġ�� ó������ �̵���Ų��.
			this->notepadForm->current->First();//ĳ���� ���� ��ġ�� �� ó��(0)���� ����
			//4.8.3 ù��° ���ڸ� �б� ���� ĳ���� 1 ������Ų��.
			currentLetterIndex = this->notepadForm->current->Next();
			//4.8.4 ���� ���� �ý�Ʈ�� ���� ���Ѵ�.
			Long currentRowTextWidth = this->notepadForm->textExtent->
				GetTextWidth(this->notepadForm->current->
					GetPartOfContent(currentLetterIndex).c_str());
			//4.8.5 i(ĳ���� ���� ������ġ)�� length(�������� ���ڰ���)���� �۰�
			//���� ���� �ؽ�Ʈ ũ�Ⱑ �̵��ϱ� �� ���� �ؽ�Ʈ ũ�⺸�� �������� �ݺ��Ѵ�.
			while (currentLetterIndex < this->notepadForm->current->GetLength()
				&& currentRowTextWidth < previousRowTextWidth)
			{
				//4.8.5.1 i(ĳ���� ���� ������ġ)�� ������Ų��
				//(ĳ���� ���� ��ġ�� ���� ĭ���� �̵���Ų��.).
				currentLetterIndex = this->notepadForm->current->Next();
				//4.8.5.2 ���� ���� ĳ���� ���� ��ġ������ �ý�Ʈ ���� ���Ѵ�.
				currentRowTextWidth = this->notepadForm->textExtent->
					GetTextWidth(this->notepadForm->current->
						GetPartOfContent(currentLetterIndex).c_str());
			}
			//4.8.6 ���� ���� �ؽ�Ʈ ������ ���� ���� �ؽ�Ʈ ���� ����.
			Long difference = currentRowTextWidth - previousRowTextWidth;
			//4.8.7 ���̰� 0���� ũ��(�������� �ؽ�Ʈ�� �� ���� ���� ���)
			if (difference > 0)
			{
				//4.8.7.1 ĳ���� ���� ������ġ ������ ���� ���� ���Ѵ�.
				Long letterWidth = this->notepadForm->textExtent->
					GetTextWidth(this->notepadForm->current->
						GetAt(currentLetterIndex - 1)->GetContent());
				Long halfLetterSize = letterWidth / 2;
				//4.8.7.2 ���̰� ���� ����ũ���� ���ݺ��� ���ų� ũ��
				if (difference >= halfLetterSize)
				{
					//4.8.7.2.1 ĳ���� ���� ���� ��ġ�� �������� �̵��Ѵ�.
					currentLetterIndex = this->notepadForm->current->Previous();
				}
			}
		}
		//4.9 ĳ���� ���� ���� ��ġ�� 0 �Ǵ� ĳ���� ���� ���� ���ڰ����� 0���̸�
		else
		{
			//4.9.1 ���� ĳ���� ���� ��ġ�� 0���� �̵���Ų��.
			currentLetterIndex = this->notepadForm->current->First();
		}
		//4.10 ���ڸ� �����Ѵ�.
		SelectText selectText(this->notepadForm);
		selectText.DoNext(previousRowIndex, previousLetterIndex, currentRowIndex,
			currentLetterIndex);
	}
}

//�Ҹ���
ShiftPageDownKeyAction::~ShiftPageDownKeyAction()
{

}