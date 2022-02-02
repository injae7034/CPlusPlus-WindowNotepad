#include "CtrlRightArrowKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"

//����Ʈ������
CtrlRightArrowKeyAction::CtrlRightArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlRightArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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

	//1. �̵��ϱ� ���� ���� ���� ��ġ�� ���Ѵ�.
	previousRowIndex = this->notepadForm->note->GetCurrent();
	//2. Note�� NextWord�� ȣ���Ѵ�.
	Long currentCaretindex = this->notepadForm->note->NextWord();
	//3. �̵��� �Ŀ� ���� ���� ��ġ�� ���Ѵ�.
	currentRowIndex = this->notepadForm->note->GetCurrent();
	//4. �������� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	//5. �������� ĳ���� ���� ��ġ�� �����Ѵ�.
	Long i = this->notepadForm->current->First();
	while (i < currentCaretindex)
	{
		//5.1 ĳ���� �������� �̵���Ų��.
		i = this->notepadForm->current->Next();
	}
	//6. �̵��� �ڿ� ���� ĳ���� ���� ��ġ�� �״���̸�(���� �ٲ���X, �� �� ������ �̵���)
	if (previousRowIndex == currentRowIndex)
	{
		//6.1 caret �̵� �Ŀ� ĳ���� ���� ����(���۰� ��)�� ���Ѵ�.(beginCaretYPos, endCaretYPos)
		beginCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
		endCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);
		//6.2 ������ũ���� �̵��ϱ� ���� ���� ������ũ���� VScrollPos(��ũ���� ���� ��ġ)�� ���Ѵ�.
		VScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
		//6.3 ���ν�ũ���� ������ġ(VScrollPos)�� ����ȭ���� ���α����� ���� ���Ѵ�.
		VerticalSum = VScrollPos + this->notepadForm->
			scrollController->scroll[1]->GetPageSize();
		//6.4 ĳ���� ���� ��ġ ������(beginCaretYPos)�� 
		//������ũ���� ������ġ(VScrollPos)���� �޸��忡�� ���� ������
		if (beginCaretYPos < VScrollPos)
		{
			//6.4.1 ������ũ���� �̵��� ��ġ(distance)�� ĳ���� ���� ��ġ ���������� ���Ѵ�.
			distance = beginCaretYPos;
			//6.4.2 ������ũ�� ���� ��ġ�� distance�� �̵���Ų��.
			this->notepadForm->scrollController->scroll[1]->Move(distance);
			//6.4.3 ������ũ�ѹ��� Thumb�� ������ũ���� ������ġ�� �̵���Ų��.
			this->notepadForm->SetScrollPos(SB_VERT,
				this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
		}
		//6.5 ĳ���� ������ġ ������(endCaretYPos)�� ������ũ���� ���� ��ġ(VScrollPos)�� 
		//����ȭ���� ���α����� ��(VerticalSum)���� �޸��忡�� �Ʒ��� ������
		else if (endCaretYPos > VerticalSum)
		{
			//6.5.1 ������ũ���� �̵��� ����(distance)�� ���Ѵ�.
			//ĳ���� ���� ��ġ ������ �������� ȭ���� ���α����� ���� ���Ѵ�.
			distance = endCaretYPos - this->notepadForm->
				scrollController->scroll[1]->GetPageSize();
			//6.5.2 ������ũ���� ������ġ�� distance�� �̵���Ų��.
			this->notepadForm->scrollController->scroll[1]->Move(distance);
			//6.5.3 ������ũ�ѹ��� Thumb�� ������ũ���� ���� ��ġ�� �̵���Ų��.
			this->notepadForm->SetScrollPos(SB_VERT,
				this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
		}
		//6.6 ���� ĳ���� ���� ��ġ�� ���Ѵ�.
		caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
			current->GetPartOfContent(currentCaretindex));
		//6.7 ����ũ���� ���� ��ġ�� ���Ѵ�.
		HScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
		//6.8 ����ũ���� ������ġ(HScrollPos)�� ����ȭ���� ���α����� ���� ���Ѵ�.
		HorizontalSum = HScrollPos + this->notepadForm->
			scrollController->scroll[0]->GetPageSize();
		//6.9 ĳ���� ���� ������ġ(caretXPos)�� ����ũ���� ������ġ(HScrollPos)����
			//�޸��忡�� �տ� ������
		if (caretXPos < HScrollPos)
		{
			//6.9.1 ����ũ���� �̵��� ��ġ(distance)�� ���Ѵ�.
			distance = caretXPos - (this->notepadForm->
				scrollController->scroll[0]->GetPageSize() / 5);
			//6.9.2 distance�� �����̸�
			if (distance < 0)
			{
				//6.9.2.1 distance�� 0���� �ٲ��ش�.
				distance = 0;
			}
			//6.9.3 ����ũ���� ������ġ�� distance�� �̵���Ų��.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//6.9.4 ����ũ�ѹ��� Thumb�� ����ũ���� ���� ��ġ�� �̵���Ų��.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
		//6.10 ĳ���� ���� ������ġ(caretXPos)�� ����ũ���� ������ġ(HScrollPos)��
		//����ȭ���� ���α����� ��(HorizontalSum)���� �޸��忡�� �ڿ� ������
		else if (caretXPos > HorizontalSum)
		{
			//6.10.1 ����ũ���� �̵��� ��ġ(distance)�� ���Ѵ�.
			distance = caretXPos - (this->notepadForm->
				scrollController->scroll[0]->GetPageSize() / 5 * 4);
			//6.10.2 ����ũ�ѹ��� Thumb�� �ִ� �̵���(maxScrollPos)�� ���Ѵ�.
			maxScrollPos = this->notepadForm->scrollController->scroll[0]->GetMax()
				- this->notepadForm->scrollController->scroll[0]->GetPageSize();
			//6.10.3 distance�� maxScrollPos���� ũ��
			if (distance > maxScrollPos)
			{
				//6.10.3.1 distance�� maxScrollPos�� �����Ѵ�.
				distance = maxScrollPos;
			}
			//6.10.4 ����ũ���� ������ġ�� distance�� �̵���Ų��.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//6.10.5 ����ũ�ѹ��� Thumb�� ����ũ���� ���� ��ġ�� �̵���Ų��.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
	}
	//7. �̵��� �ڿ� ���� ĳ���� ���� ��ġ�� �ٲ������(���� �ٲ�. ���� �ٿ��� ���� �ٷ� �̵���.)
	else
	{
		//7.1 ĳ���� �̵��� �Ŀ� caretYPos(ĳ���� ���� ��ġ)�� ����(���۰� ��)�� ���Ѵ�.
		beginCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
		endCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);
		//7.2 ������ũ���� �̵��ϱ� ���� ���� ������ũ���� VScrollPos(��ũ���� ���� ��ġ)�� ���Ѵ�.
		VScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
		//7.3 ���ν�ũ���� ������ġ(VScrollPos)�� ����ȭ���� ���α����� ���� ���Ѵ�.
		VerticalSum = VScrollPos + this->notepadForm->
			scrollController->scroll[1]->GetPageSize();
		//7.4 ĳ���� ���� ��ġ ������(beginCaretYPos)�� 
		//������ũ���� ������ġ(VScrollPos)���� �޸��忡�� ���� ������
		if (beginCaretYPos < VScrollPos)
		{
			//7.4.1 ������ũ���� �̵��� ��ġ(distance)�� ĳ���� ���� ��ġ ���������� ���Ѵ�.
			distance = beginCaretYPos;
			//7.4.2 ������ũ�� ���� ��ġ�� distance�� �̵���Ų��.
			this->notepadForm->scrollController->scroll[1]->Move(distance);
			//7.4.3 ������ũ�ѹ��� Thumb�� ������ũ���� ������ġ�� �̵���Ų��.
			this->notepadForm->SetScrollPos(SB_VERT,
				this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
		}
		//7.5 ĳ���� ������ġ ������(endCaretYPos)�� ������ũ���� ���� ��ġ(VScrollPos)�� 
		//����ȭ���� ���α����� ��(VerticalSum)���� �޸��忡�� �Ʒ��� ������
		else if (endCaretYPos > VerticalSum)
		{
			//7.5.1 ������ũ���� �̵��� ����(distance)�� ���Ѵ�.
			//ĳ���� ���� ��ġ ������ �������� ȭ���� ���α����� ���� ���Ѵ�.
			distance = endCaretYPos - this->notepadForm->
				scrollController->scroll[1]->GetPageSize();
			//7.5.2 ������ũ���� ������ġ�� distance�� �̵���Ų��.
			this->notepadForm->scrollController->scroll[1]->Move(distance);
			//7.5.3 ������ũ�ѹ��� Thumb�� ������ũ���� ���� ��ġ�� �̵���Ų��.
			this->notepadForm->SetScrollPos(SB_VERT,
				this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
		}
		//7.6 ĳ���� �̵��� �Ŀ� ĳ���� ���� ������ġ(caretXPos)�� ���Ѵ�.
		caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
			current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
		//7.7 ����ũ���� �̵��ϱ����� ����ũ���� ������ġ(HScrollPos)�� ���Ѵ�.
		HScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
		//7.8 ����ũ���� ������ġ(HScrollPos)�� ����ȭ���� ���α����� ���� ���Ѵ�.
		HorizontalSum = HScrollPos + this->notepadForm->
			scrollController->scroll[0]->GetPageSize();
		//7.9 ĳ���� ���� ������ġ(caretXPos)�� ����ũ���� ������ġ(HScrollPos)����
		//�޸��忡�� �տ� ������
		if (caretXPos < HScrollPos)
		{
			//7.9.1 ����ũ���� �̵��� ��ġ(distance)�� ���Ѵ�.
			distance = caretXPos - (this->notepadForm->
				scrollController->scroll[0]->GetPageSize() / 5);
			//7.9.2 distance�� �����̸�
			if (distance < 0)
			{
				//7.9.2.1 distance�� 0���� �ٲ��ش�.
				distance = 0;
			}
			//7.9.3 ����ũ���� ������ġ�� distance�� �̵���Ų��.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//7.9.4 ����ũ�ѹ��� Thumb�� ����ũ���� ���� ��ġ�� �̵���Ų��.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
		//7.10 ĳ���� ���� ������ġ(caretXPos)�� ����ũ���� ������ġ(HScrollPos)��
		//����ȭ���� ���α����� ��(HorizontalSum)���� �޸��忡�� �ڿ� ������
		else if (caretXPos > HorizontalSum)
		{
			//7.10.1 ����ũ���� �̵��� ��ġ(distance)�� ���Ѵ�.
			distance = caretXPos - (this->notepadForm->
				scrollController->scroll[0]->GetPageSize() / 5 * 4);
			//7.10.2 ����ũ�ѹ��� Thumb�� �ִ� �̵���(maxScrollPos)�� ���Ѵ�.
			maxScrollPos = this->notepadForm->scrollController->scroll[0]->GetMax()
				- this->notepadForm->scrollController->scroll[0]->GetPageSize();
			//7.10.3 distance�� maxScrollPos���� ũ��
			if (distance > maxScrollPos)
			{
				//7.10.3.1 distance�� maxScrollPos�� �����Ѵ�.
				distance = maxScrollPos;
			}
			//7.10.4 ����ũ���� ������ġ�� distance�� �̵���Ų��.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//7.10.5 ����ũ�ѹ��� Thumb�� ����ũ���� ���� ��ġ�� �̵���Ų��.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
	}

}

//�Ҹ���
CtrlRightArrowKeyAction::~CtrlRightArrowKeyAction()
{

}