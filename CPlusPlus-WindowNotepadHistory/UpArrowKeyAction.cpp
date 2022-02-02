#include "UpArrowKeyAction.h"
#include "Glyph.h"
#include "TextExtent.h"
#include "ScrollController.h"
#include "Scroll.h"

//����Ʈ������
UpArrowKeyAction::UpArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void UpArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	Long HScrollPos = 0;//���ν�ũ���� ������ġ
	Long VScrollPos = 0;//���ν�ũ���� ������ġ
	Long beginCaretYPos = 0;//ĳ���� ������ġ ��������
	Long endCaretYPos = 0;//ĳ���� ������ġ ��������
	Long caretXPos = 0;//ĳ���� ������ġ
	Long VerticalSum = 0;//���ν�ũ���� ������ġ + ����ȭ���� ���α���
	Long HorizontalSum = 0;//���ν�ũ���� ������ġ + ����ȭ���� ���α���
	Long distance = 0;//��ũ���� �̵��� ��ġ
	Long maxScrollPos = 0;//��ũ�ѹ��� Thumb�� �̵��� �� �ִ� ��ġ
	Long previousRowIndex = 0;//�������� �̵��ϱ� ���� ���� ���� ��ġ
	Long currentRowIndex = 0;//�������� �̵��� �Ŀ� ���� ���� ��ġ

	//1. �������� �̵��ϱ� ���� ĳ���� ���� ���� ��ġ�� �����Ѵ�.
	previousRowIndex = this->notepadForm->note->GetCurrent();
	//2. �������� �̵��ϱ� ���� ĳ���� ���� ���� ��ġ�� �����Ѵ�.
	Long previousCaretIndex = this->notepadForm->current->GetCurrent();
	//3. ĳ���� ���� ���� ��ġ�� ���� �ٷ� �̵���Ų��.
	currentRowIndex = this->notepadForm->note->Previous();
	//4. �̵��ϱ� �� ĳ���� ���� ��ġ�� �̵��� �� ĳ���� ���� ��ġ�� ���� �ٸ���(���������� �̵��� ������)
	if (previousRowIndex != currentRowIndex)
	{
		//4.1 ���� ���� �̵��� �� ĳ���� ���� ��ġ�� �ִ� �ٷ� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
		//4.2 �̵��ϱ� �� ĳ���� ���� ��ġ�� 0�� �ƴϰ� �̵��� �� �������� ���ڰ����� 0�� �ƴϸ�
		//(�̵��ϱ� �� ĳ���� ���� ��ġ�� 0�̸� �̵��� ���� ���� ĳ���� ���� ��ġ�� ������ 0�̰�,
		//�̵��� �� �������� ���ڰ����� 0�̾ ���� ĳ���� ���� ��ġ�� ������ 0��,
		//���� �� �̿��� ����� ���� if�ȿ��� ó���ϰ� else���� ���� 2��츦 first�� ó����!)
		if (previousCaretIndex != 0 && this->notepadForm->current->GetLength() != 0)
		{
			//4.2.1 �̵��ϱ� �� ���� �ؽ�Ʈ ���� ���Ѵ�.
			Long previousRowTextWidth = this->notepadForm->textExtent->
				GetTextWidth(this->notepadForm->note->GetAt(previousRowIndex)
				->GetPartOfContent(previousCaretIndex));
			//4.2.2 ĳ���� ���� ���� ��ġ�� ó������ �̵���Ų��.(���� ĳ���� ���� ��ġ�� ����� �𸣱⶧����)
			//i�� 0�� �Ǹ� GetPartOfContent���� �д� �ؽ�Ʈ�� ���� ������ i�� �ּҰ��� 1�� �Ǿ����.
			//GetPartOfContent�� ĳ���� ��ġ���� �ִ� ���ڵ��� �д°���!
			//�� if ���ǽĿ��� �̹� ���� ���� ���ڰ� �ϳ��� ���� ���� �ɷ����� ���Ա� ������
			//�������� ���ڼ��� �ּ� 1���̻��� �ִ� ��쿡 ���ؼ� ó����.
			this->notepadForm->current->First();//ĳ���� ���� ��ġ�� �� ó��(0)���� ����(����ġ)
			//4.2.3 ù��° ���ڸ� �б� ���� ĳ���� ��ĭ�̵���Ų��.
			Long i = this->notepadForm->current->Next();
			//4.2.4 ���� ���� �ý�Ʈ�� ���� ���Ѵ�.
			Long currentRowTextWidth = this->notepadForm->textExtent->
				GetTextWidth(this->notepadForm->current->GetPartOfContent(i).c_str());
			//4.2.5 i(ĳ���� ���� ������ġ)�� length(�������� ���ڰ���)���� �۰�
			//���� ���� �ؽ�Ʈ ũ�Ⱑ �̵��ϱ� �� ���� �ؽ�Ʈ ũ�⺸�� �������� �ݺ��Ѵ�.
			while (i < this->notepadForm->current->GetLength()
				&& currentRowTextWidth < previousRowTextWidth)
			{
				//4.2.5.1 i(ĳ���� ���� ������ġ)�� ������Ų��
				//(ĳ���� ���� ��ġ�� ���� ĭ���� �̵���Ų��.).
				i = this->notepadForm->current->Next();
				//4.2.5.2 ���� ���� ĳ���� ���� ��ġ������ �ý�Ʈ ���� ���Ѵ�.
				currentRowTextWidth = this->notepadForm->textExtent->
				GetTextWidth(this->notepadForm->current->GetPartOfContent(i).c_str());
			}
			//4.2.6 ���� ���� �ؽ�Ʈ ������ ���� ���� �ؽ�Ʈ ���� ����.
			Long difference = currentRowTextWidth - previousRowTextWidth;
			//(�����ٰ� �������� �ؽ�Ʈũ�� ���̰� 0�̸� �ؽ�Ʈ ũ�Ⱑ ���ٴ� �ǹ��̱⶧����
			//���� ĳ���� ���� ��ġ�� ���� ��ġ �״�ο� ������ ��)
			//(�����ٰ� �������� �ؽ�Ʈũ�� ���̰� �����̸� 
			//currentRowTextSize.cx < previousRowTextSize.cx�̶�� ���ε� �׷��� ����
			//�ݺ��� ���ǿ��� ������������ �ᱹ i�� �������� ���ڰ���(length)�� ���Ƽ� �������Դٴ� 
			//�ǹ��̰� �׷��ٸ� i�� ���� ���� ������ ĳ�� ��ġ��� ���̰� 
			//�̵��ϱ��� ���� �ؽ�Ʈ�� ���� ���� ��ü �ؽ�Ʈ���� �� ū ���� �������� ĳ��������ġ��
			//������ �������� �������� �־�� �ϰ� ���� ������ ��ġ�� �ֱ� ������ �״�� �θ� �ȴ�.
			//4.2.7 ���̰� 0���� ũ��(�������� �ؽ�Ʈ�� �� ���� ���� ���)
			//�������� ��ü �ؽ�Ʈ�� �������� ĳ�� ���� ��ġ���� ���� �ؽ�Ʈ���� ū ���
			if (difference > 0)
			{
				//4.2.7.1 ĳ���� ���� ������ġ ������ ���� ���� ���Ѵ�.
				Long letterWidth = this->notepadForm->textExtent->
					GetTextWidth(this->notepadForm->current->GetAt(i - 1)->GetContent());
				Long halfLetterSize = letterWidth / 2;
				//4.2.7.2 ���̰� ���� ����ũ���� ���ݺ��� ���ų� ũ��
				if (difference >= halfLetterSize)
				{
					//4.2.7.2.1 ĳ���� ���� ���� ��ġ�� �������� �̵��Ѵ�.
					this->notepadForm->current->Previous();
				}
			}
		}
		//4.3 ĳ���� ���� ���� ��ġ�� 0 �Ǵ� ĳ���� ���� ���� ���ڰ����� 0���̸�
		else
		{
			//4.3.1 ���� ĳ���� ���� ��ġ�� 0���� �̵���Ų��.
			this->notepadForm->current->First();
		}

		//ĳ�� �̵� ���� ��Ų �Ŀ� ��ũ�� �̵�
		//4.4 ĳ���� ���� ���� ��ġ�� ���Ѵ�.
		currentRowIndex = this->notepadForm->note->GetCurrent();
		//4.5 caret �̵� �Ŀ� ĳ���� ���� ����(���۰� ��)�� ���Ѵ�.(beginCaretYPos, endCaretYPos)
		beginCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
		endCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);
		//4.6 ������ũ���� �̵��ϱ� ���� ���� ������ũ���� VScrollPos(��ũ���� ���� ��ġ)�� ���Ѵ�.
		VScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
		//4.7 ���ν�ũ���� ������ġ(VScrollPos)�� ����ȭ���� ���α����� ���� ���Ѵ�.
		VerticalSum = VScrollPos + this->notepadForm->
			scrollController->scroll[1]->GetPageSize();
		//4.8 ĳ���� ���� ��ġ ������(beginCaretYPos)�� 
		//������ũ���� ������ġ(VScrollPos)���� �޸��忡�� ���� ������
		if (beginCaretYPos < VScrollPos)
		{
			//4.8.1 ������ũ���� �̵��� ��ġ(distance)�� ĳ���� ���� ��ġ ���������� ���Ѵ�.
			distance = beginCaretYPos;
			//4.8.2 ������ũ�� ���� ��ġ�� distance�� �̵���Ų��.
			this->notepadForm->scrollController->scroll[1]->Move(distance);
			//4.8.3 ������ũ�ѹ��� Thumb�� ������ũ���� ������ġ�� �̵���Ų��.
			this->notepadForm->SetScrollPos(SB_VERT,
				this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
		}
		//4.9 ĳ���� ������ġ ������(endCaretYPos)�� ������ũ���� ���� ��ġ(VScrollPos)�� 
		//����ȭ���� ���α����� ��(VerticalSum)���� �޸��忡�� �Ʒ��� ������
		else if (endCaretYPos > VerticalSum)
		{
			//4.9.1 ������ũ���� �̵��� ����(distance)�� ���Ѵ�.
			//ĳ���� ���� ��ġ ������ �������� ȭ���� ���α����� ���� ���Ѵ�.
			distance = endCaretYPos - this->notepadForm->
				scrollController->scroll[1]->GetPageSize();
			//4.9.2 ������ũ���� ������ġ�� distance�� �̵���Ų��.
			this->notepadForm->scrollController->scroll[1]->Move(distance);
			//4.9.3 ������ũ�ѹ��� Thumb�� ������ũ���� ���� ��ġ�� �̵���Ų��.
			this->notepadForm->SetScrollPos(SB_VERT,
				this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
		}
		//4.10 ĳ���� �̵��� �Ŀ� ĳ���� ���� ������ġ(caretXPos)�� ���Ѵ�.
		caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
			current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
		//4.11 ����ũ���� �̵��ϱ����� ����ũ���� ������ġ(HScrollPos)�� ���Ѵ�.
		HScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
		//4.12 ����ũ���� ������ġ(HScrollPos)�� ����ȭ���� ���α����� ���� ���Ѵ�.
		HorizontalSum = HScrollPos + this->notepadForm->
			scrollController->scroll[0]->GetPageSize();
		//4.13 ĳ���� ���� ������ġ(caretXPos)�� ����ũ���� ������ġ(HScrollPos)����
			//�޸��忡�� �տ� ������
		if (caretXPos < HScrollPos)
		{
			//4.13.1 ����ũ���� �̵��� ��ġ(distance)�� ���Ѵ�.
			distance = caretXPos - (this->notepadForm->
				scrollController->scroll[0]->GetPageSize() / 5);
			//4.13.2 distance�� �����̸�
			if (distance < 0)
			{
				//4.13.2.1 distance�� 0���� �ٲ��ش�.
				distance = 0;
			}
			//4.13.3 ����ũ���� ������ġ�� distance�� �̵���Ų��.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//4.13.4 ����ũ�ѹ��� Thumb�� ����ũ���� ���� ��ġ�� �̵���Ų��.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
		//4.14 ĳ���� ���� ������ġ(caretXPos)�� ����ũ���� ������ġ(HScrollPos)��
		//����ȭ���� ���α����� ��(HorizontalSum)���� �޸��忡�� �ڿ� ������
		else if (caretXPos > HorizontalSum)
		{
			//4.14.1 ����ũ���� �̵��� ��ġ(distance)�� ���Ѵ�.
			distance = caretXPos - (this->notepadForm->
				scrollController->scroll[0]->GetPageSize() / 5 * 4);
			//4.14.2 ����ũ�ѹ��� Thumb�� �ִ� �̵���(maxScrollPos)�� ���Ѵ�.
			maxScrollPos = this->notepadForm->scrollController->scroll[0]->GetMax()
				- this->notepadForm->scrollController->scroll[0]->GetPageSize();
			//4.14.3 distance�� maxScrollPos���� ũ��
			if (distance > maxScrollPos)
			{
				//4.14.3.1 distance�� maxScrollPos�� �����Ѵ�.
				distance = maxScrollPos;
			}
			//4.14.4 ����ũ���� ������ġ�� distance�� �̵���Ų��.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//4.14.5 ����ũ�ѹ��� Thumb�� ����ũ���� ���� ��ġ�� �̵���Ų��.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
	}
}

//�Ҹ���
UpArrowKeyAction::~UpArrowKeyAction()
{

}