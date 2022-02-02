#include "LeftArrowKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"

//����Ʈ������
LeftArrowKeyAction::LeftArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void LeftArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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

	//1. �������� �̵��ϱ� ���� ĳ���� ���� ������ġ�� �����Ѵ�.
	Long previousIndex = this->notepadForm->current->GetCurrent();
	//2. �������� �̵��ϰ� ���� ĳ���� ������ġ�� ��ȯ�޴´�.
	Long currentIndex = this->notepadForm->current->Previous();
	//3. ĳ���� ���� ������ġ�� ĳ���� ���� ������ġ�� ������(ĳ���� �̵��� ��������)
	//�̵��� �����ٴ� ���� ĳ���� ���� ���� ���� ó���� ��ġ�ؼ� ���̻� �̵��� ���� ���ٴ� ���̴�.
	if (previousIndex == currentIndex)
	{
		//3.1 �������� �̵��ϱ� ���� ĳ���� ���� ���� ��ġ�� �����Ѵ�.
		previousRowIndex = this->notepadForm->note->GetCurrent();
		//3.2 ���� ���� ��ġ�� ���� �ٷ� �̵���Ų��.
		currentRowIndex = this->notepadForm->note->Previous();
		//3.3 ���� ���� ��ġ�� ĳ���� ���� ���� ��ġ�� ���� �ٸ���
		if (previousRowIndex != currentRowIndex)
		{
			//3.3.1 notepadForm�� ���� ���� �����Ѵ�.
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
			//3.3.2 ���� ���� �ٲ���� ������ ĳ���� ���� ���� ��ġ�� ���������� �����Ѵ�.
			this->notepadForm->current->Last();
			//3.3.3. ĳ���� �̵��� �Ŀ� caretYPos(ĳ���� ���� ��ġ)�� ����(���۰� ��)�� ���Ѵ�.
			beginCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
			endCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);
			//3.3.4 ������ũ���� �̵��ϱ� ���� ���� ������ũ���� VScrollPos(��ũ���� ���� ��ġ)�� ���Ѵ�.
			VScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
			//3.3.5 ���ν�ũ���� ������ġ(VScrollPos)�� ����ȭ���� ���α����� ���� ���Ѵ�.
			VerticalSum = VScrollPos + this->notepadForm->
				scrollController->scroll[1]->GetPageSize();
			//3.3.6 ĳ���� ���� ��ġ ������(beginCaretYPos)�� 
			//������ũ���� ������ġ(VScrollPos)���� �޸��忡�� ���� ������
			if (beginCaretYPos < VScrollPos)
			{
				//3.3.6.1 ������ũ���� �̵��� ��ġ(distance)�� ĳ���� ���� ��ġ ���������� ���Ѵ�.
				distance = beginCaretYPos;
				//3.3.6.2 ������ũ�� ���� ��ġ�� distance�� �̵���Ų��.
				this->notepadForm->scrollController->scroll[1]->Move(distance);
				//3.3.6.3 ������ũ�ѹ��� Thumb�� ������ũ���� ������ġ�� �̵���Ų��.
				this->notepadForm->SetScrollPos(SB_VERT,
					this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
			}
			//3.3.7 ĳ���� ������ġ ������(endCaretYPos)�� ������ũ���� ���� ��ġ(VScrollPos)�� 
			//����ȭ���� ���α����� ��(VerticalSum)���� �޸��忡�� �Ʒ��� ������
			else if (endCaretYPos > VerticalSum)
			{
				//3.3.7.1 ������ũ���� �̵��� ����(distance)�� ���Ѵ�.
				//ĳ���� ���� ��ġ ������ �������� ȭ���� ���α����� ���� ���Ѵ�.
				distance = endCaretYPos - this->notepadForm->
					scrollController->scroll[1]->GetPageSize();
				//3.3.7.2 ������ũ���� ������ġ�� distance�� �̵���Ų��.
				this->notepadForm->scrollController->scroll[1]->Move(distance);
				//3.3.7.3 ������ũ�ѹ��� Thumb�� ������ũ���� ���� ��ġ�� �̵���Ų��.
				this->notepadForm->SetScrollPos(SB_VERT,
					this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
			}
			//3.3.8 ĳ���� �̵��� �Ŀ� ĳ���� ���� ������ġ(caretXPos)�� ���Ѵ�.
			caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
				current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
			//3.3.9 ����ũ���� �̵��ϱ����� ����ũ���� ������ġ(HScrollPos)�� ���Ѵ�.
			HScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
			//3.3.10 ����ũ���� ������ġ(HScrollPos)�� ����ȭ���� ���α����� ���� ���Ѵ�.
			HorizontalSum = HScrollPos + this->notepadForm->
				scrollController->scroll[0]->GetPageSize();
			//3.3.11 ĳ���� ���� ������ġ(caretXPos)�� ����ũ���� ������ġ(HScrollPos)����
			//�޸��忡�� �տ� ������
			if (caretXPos < HScrollPos)
			{
				//3.3.11.1 ����ũ���� �̵��� ��ġ(distance)�� ���Ѵ�.
				distance = caretXPos - (this->notepadForm->
					scrollController->scroll[0]->GetPageSize() / 5);
				//3.3.11.2 distance�� �����̸�
				if (distance < 0)
				{
					//3.3.11.2.1 distance�� 0���� �ٲ��ش�.
					distance = 0;
				}
				//3.3.11.3 ����ũ���� ������ġ�� distance�� �̵���Ų��.
				this->notepadForm->scrollController->scroll[0]->Move(distance);
				//3.3.11.4 ����ũ�ѹ��� Thumb�� ����ũ���� ���� ��ġ�� �̵���Ų��.
				this->notepadForm->SetScrollPos(SB_HORZ,
					this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
			}
			//3.3.12 ĳ���� ���� ������ġ(caretXPos)�� ����ũ���� ������ġ(HScrollPos)��
			//����ȭ���� ���α����� ��(HorizontalSum)���� �޸��忡�� �ڿ� ������
			else if (caretXPos > HorizontalSum)
			{
				//3.3.12.1 ����ũ���� �̵��� ��ġ(distance)�� ���Ѵ�.
				distance = caretXPos - (this->notepadForm->
					scrollController->scroll[0]->GetPageSize() / 5 * 4);
				//3.3.12.2 ����ũ�ѹ��� Thumb�� �ִ� �̵���(maxScrollPos)�� ���Ѵ�.
				maxScrollPos = this->notepadForm->scrollController->scroll[0]->GetMax()
					- this->notepadForm->scrollController->scroll[0]->GetPageSize();
				//3.3.12.3 distance�� maxScrollPos���� ũ��
				if (distance > maxScrollPos)
				{
					//3.3.12.3.1 distance�� maxScrollPos�� �����Ѵ�.
					distance = maxScrollPos;
				}
				//3.3.12.4 ����ũ���� ������ġ�� distance�� �̵���Ų��.
				this->notepadForm->scrollController->scroll[0]->Move(distance);
				//3.3.12.5 ����ũ�ѹ��� Thumb�� ����ũ���� ���� ��ġ�� �̵���Ų��.
				this->notepadForm->SetScrollPos(SB_HORZ,
					this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
			}
		}
	}
	//5. ĳ���� ���� ������ġ�� ĳ���� ���� ������ġ�� �ٸ���(ĳ���� �� �ٳ����� �̵�������)
	//ĳ���� �̵��ߴٴ� ���� ĳ���� �̵��ϱ� ���� ���� ó���� ���� �ʾҴٴ� ����
	else
	{
		//5.1 ĳ���� ���� ���� ��ġ�� ���Ѵ�.
		currentRowIndex = this->notepadForm->note->GetCurrent();
		//5.2 caret �̵� �Ŀ� ĳ���� ���� ����(���۰� ��)�� ���Ѵ�.(beginCaretYPos, endCaretYPos)
		beginCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
		endCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);
		//5.3 ������ũ���� �̵��ϱ� ���� ���� ������ũ���� VScrollPos(��ũ���� ���� ��ġ)�� ���Ѵ�.
		VScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
		//5.4 ���ν�ũ���� ������ġ(VScrollPos)�� ����ȭ���� ���α����� ���� ���Ѵ�.
		VerticalSum = VScrollPos + this->notepadForm->
			scrollController->scroll[1]->GetPageSize();
		//5.5 ĳ���� ���� ��ġ ������(beginCaretYPos)�� 
		//������ũ���� ������ġ(VScrollPos)���� �޸��忡�� ���� ������
		if (beginCaretYPos < VScrollPos)
		{
			//5.5.1 ������ũ���� �̵��� ��ġ(distance)�� ĳ���� ���� ��ġ ���������� ���Ѵ�.
			distance = beginCaretYPos;
			//5.5.2 ������ũ�� ���� ��ġ�� distance�� �̵���Ų��.
			this->notepadForm->scrollController->scroll[1]->Move(distance);
			//5.5.3 ������ũ�ѹ��� Thumb�� ������ũ���� ������ġ�� �̵���Ų��.
			this->notepadForm->SetScrollPos(SB_VERT,
				this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
		}
		//5.6 ĳ���� ������ġ ������(endCaretYPos)�� ������ũ���� ���� ��ġ(VScrollPos)�� 
		//����ȭ���� ���α����� ��(VerticalSum)���� �޸��忡�� �Ʒ��� ������
		else if (endCaretYPos > VerticalSum)
		{
			//5.6.1 ������ũ���� �̵��� ����(distance)�� ���Ѵ�.
			//ĳ���� ���� ��ġ ������ �������� ȭ���� ���α����� ���� ���Ѵ�.
			distance = endCaretYPos - this->notepadForm->
				scrollController->scroll[1]->GetPageSize();
			//5.6.2 ������ũ���� ������ġ�� distance�� �̵���Ų��.
			this->notepadForm->scrollController->scroll[1]->Move(distance);
			//5.6.3 ������ũ�ѹ��� Thumb�� ������ũ���� ���� ��ġ�� �̵���Ų��.
			this->notepadForm->SetScrollPos(SB_VERT,
				this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
		}
		//5.7 ĳ���� �̵��� �Ŀ� ĳ���� ���� ������ġ(caretXPos)�� ���Ѵ�.
		caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
			current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
		//5.8 ����ũ���� �̵��ϱ����� ����ũ���� ������ġ(HScrollPos)�� ���Ѵ�.
		HScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
		//5.9 ����ũ���� ������ġ(HScrollPos)�� ����ȭ���� ���α����� ���� ���Ѵ�.
		HorizontalSum = HScrollPos + this->notepadForm->
			scrollController->scroll[0]->GetPageSize();
		//5.10 ĳ���� ���� ������ġ(caretXPos)�� ����ũ���� ������ġ(HScrollPos)����
			//�޸��忡�� �տ� ������
		if (caretXPos < HScrollPos)
		{
			//5.10.1 ����ũ���� �̵��� ��ġ(distance)�� ���Ѵ�.
			distance = caretXPos - (this->notepadForm->
				scrollController->scroll[0]->GetPageSize() / 5);
			//5.10.2 distance�� �����̸�
			if (distance < 0)
			{
				//5.10.2.1 distance�� 0���� �ٲ��ش�.
				distance = 0;
			}
			//5.10.3 ����ũ���� ������ġ�� distance�� �̵���Ų��.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//5.10.4 ����ũ�ѹ��� Thumb�� ����ũ���� ���� ��ġ�� �̵���Ų��.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
		//5.11 ĳ���� ���� ������ġ(caretXPos)�� ����ũ���� ������ġ(HScrollPos)��
		//����ȭ���� ���α����� ��(HorizontalSum)���� �޸��忡�� �ڿ� ������
		else if (caretXPos > HorizontalSum)
		{
			//5.11.1 ����ũ���� �̵��� ��ġ(distance)�� ���Ѵ�.
			distance = caretXPos - (this->notepadForm->
				scrollController->scroll[0]->GetPageSize() / 5 * 4);
			//5.11.2 ����ũ�ѹ��� Thumb�� �ִ� �̵���(maxScrollPos)�� ���Ѵ�.
			maxScrollPos = this->notepadForm->scrollController->scroll[0]->GetMax()
				- this->notepadForm->scrollController->scroll[0]->GetPageSize();
			//5.11.3 distance�� maxScrollPos���� ũ��
			if (distance > maxScrollPos)
			{
				//5.11.3.1 distance�� maxScrollPos�� �����Ѵ�.
				distance = maxScrollPos;
			}
			//5.11.4 ����ũ���� ������ġ�� distance�� �̵���Ų��.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//5.11.5 ����ũ�ѹ��� Thumb�� ����ũ���� ���� ��ġ�� �̵���Ų��.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
	}
}

//�Ҹ���
LeftArrowKeyAction::~LeftArrowKeyAction()
{

}