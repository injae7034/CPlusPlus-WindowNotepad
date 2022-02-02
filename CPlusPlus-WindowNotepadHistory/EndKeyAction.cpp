#include "EndKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"

//����Ʈ������
EndKeyAction::EndKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void EndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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
	Long currentRowIndex = 0;//�������� �̵��� �Ŀ� ���� ���� ��ġ

	//1. ���� ���� �� ���������� ĳ���� ���� ��ġ�� �̵���Ų��.
	this->notepadForm->current->Last();

	//4.4 ĳ���� ���� ���� ��ġ�� ���Ѵ�.
	currentRowIndex = this->notepadForm->note->GetCurrent();
	//4.5 caret �̵� �Ŀ� ĳ���� ���� ����(���۰� ��)�� ���Ѵ�.(beginCaretYPos, endCaretYPos)
	beginCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
	endCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);
	//4.6 ������ũ���� �̵��ϱ� ���� ���� ������ũ���� VScrollPos(��ũ���� ���� ��ġ)�� ���Ѵ�.
	VScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
	//4.7 ������ũ���� ������ġ(VScrollPos)�� ����ȭ���� ���α����� ���� ���Ѵ�.
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
		//4.9.2 �Ʒ��� �̵��� �� ���� ���ڰ� ���߸����� �ϱ� ���ؼ� ������ ���Ѵ�.
		Long pageSize = this->notepadForm->scrollController->scroll[1]->GetPageSize();
		Long letterHeight = this->notepadForm->textExtent->GetHeight();
		Long blank = pageSize - (pageSize / letterHeight * letterHeight);
		//4.9.3 ������ũ���� ������ġ�� distance + ����(blank)���� �̵���Ų��.
		this->notepadForm->scrollController->scroll[1]->Move(distance + blank);
		//4.9.3 ������ũ�ѹ��� Thumb�� ������ũ���� ���� ��ġ�� �̵���Ų��.
		this->notepadForm->SetScrollPos(SB_VERT,
			this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
	}

	//2. ���� ���� ĳ���� ���� ��ġ�� ���Ѵ�.
	caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
		current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
	//3. ����ũ���� �̵��ϱ����� ����ũ���� ������ġ(HScrollPos)�� ���Ѵ�.
	HScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
	//4. ����ũ���� ������ġ(HScrollPos)�� ����ȭ���� ���α����� ���� ���Ѵ�.
	HorizontalSum = HScrollPos + this->notepadForm->
		scrollController->scroll[0]->GetPageSize();
	//5. ĳ���� ���� ������ġ(caretXPos)�� ����ũ���� ������ġ(HScrollPos)����
		//�޸��忡�� �տ� ������
	if (caretXPos < HScrollPos)
	{
		//3.13.1 ����ũ���� �̵��� ��ġ(distance)�� ���Ѵ�.
		distance = caretXPos - (this->notepadForm->
			scrollController->scroll[0]->GetPageSize() / 5);
		//3.13.2 distance�� �����̸�
		if (distance < 0)
		{
			//3.13.2.1 distance�� 0���� �ٲ��ش�.
			distance = 0;
		}
		//3.13.3 ����ũ���� ������ġ�� distance�� �̵���Ų��.
		this->notepadForm->scrollController->scroll[0]->Move(distance);
		//3.13.4 ����ũ�ѹ��� Thumb�� ����ũ���� ���� ��ġ�� �̵���Ų��.
		this->notepadForm->SetScrollPos(SB_HORZ,
			this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
	}
	//3.14 ĳ���� ���� ������ġ(caretXPos)�� ����ũ���� ������ġ(HScrollPos)��
	//����ȭ���� ���α����� ��(HorizontalSum)���� �޸��忡�� �ڿ� ������
	else if (caretXPos > HorizontalSum)
	{
		//3.14.1 ����ũ���� �̵��� ��ġ(distance)�� ���Ѵ�.
		distance = caretXPos - (this->notepadForm->
			scrollController->scroll[0]->GetPageSize() / 5 * 4);
		//3.14.2 ����ũ�ѹ��� Thumb�� �ִ� �̵���(maxScrollPos)�� ���Ѵ�.
		maxScrollPos = this->notepadForm->scrollController->scroll[0]->GetMax()
			- this->notepadForm->scrollController->scroll[0]->GetPageSize();
		//3.14.3 distance�� maxScrollPos���� ũ��
		if (distance > maxScrollPos)
		{
			//3.14.3.1 distance�� maxScrollPos�� �����Ѵ�.
			distance = maxScrollPos;
		}
		//3.14.4 ����ũ���� ������ġ�� distance�� �̵���Ų��.
		this->notepadForm->scrollController->scroll[0]->Move(distance);
		//3.14.5 ����ũ�ѹ��� Thumb�� ����ũ���� ���� ��ġ�� �̵���Ų��.
		this->notepadForm->SetScrollPos(SB_HORZ,
			this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
	}
}

//�Ҹ���
EndKeyAction::~EndKeyAction()
{

}