#include "PageMoveController.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "TextExtent.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "CaretController.h"

//����Ʈ������
PageMoveController::PageMoveController(NotepadForm* notepadForm)
	: Observer(notepadForm)//PageMoveController�� Subject�� NotepadForm�� ����ȭ��Ŵ.
{
	//1. �Է¹��� notepadForm�� �ּҸ� �����Ѵ�.
	this->notepadForm = notepadForm;
	//2. CaretContoroller�� ����������Ʈ���� ã�´�.
	Long i = this->subject->Search(this->notepadForm->caretController);
	//3. ����������Ʈ�� CaretController�� ������
	if (i >= 0)
	{
		//3.1 CaretController �տ� PageMoveController�� �����ִ´�.
		this->subject->Insert(i, this);
	}
	//4. ����������Ʈ�� CaretController�� ������
	else
	{
		//4.1 PageMoveController�� �߰��Ѵ�.
		this->subject->Attach(this);
	}
}

//Update
void PageMoveController::Update()
{
	Long HScrollPos = 0;//����ũ���� ������ġ
	Long VScrollPos = 0;//������ũ���� ������ġ
	//�޸��忡�� ĳ���� ������ġ �������� ������ ���̿��� ������ ��� ���̸�ŭ�� ���̰� �����Ѵ�.
	Long beginCaretYPos = 0;//�޸��忡�� ĳ���� ������ġ ��������
	Long endCaretYPos = 0;//�޸��忡�� ĳ���� ������ġ ��������
	Long caretXPos = 0;//�޸��忡�� ĳ���� ������ġ
	Long VerticalSum = 0;//������ũ���� ������ġ + ����ȭ���� ���α���
	Long HorizontalSum = 0;//���ν�ũ���� ������ġ + ����ȭ���� ���α���
	Long distance = 0;//��ũ���� �̵��� ��ġ
	//��ũ���� Thumb�� ��ũ���� Max���� �̵��� �� ����. �ֳ��ϸ� Thumb�� ���̴� ���� ȭ���� �����̱�
	//������ ��ũ���� Thumb�� �ּ� 0���� �ִ� Max-pageSize���� �̵��� �� �ִ�.
	Long maxScrollPos = 0;//��ũ�ѹ��� Thumb�� �̵��� �� �ִ� ��ġ
	Long previousRowIndex = 0;//�̵��ϱ� ���� ���� ��ġ
	Long currentRowIndex = 0;//�̵��� �Ŀ� ���� ���� ��ġ

	//1. ���� ���� ��ġ�� ���Ѵ�.
	currentRowIndex = this->notepadForm->note->GetCurrent();
	//2. �޸��忡�� ĳ���� ���� ����(���۰� ��)�� ���Ѵ�.(beginCaretYPos, endCaretYPos)
	beginCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
	endCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);
	//3. ������ũ���� �̵��ϱ� ���� ���� ������ũ���� VScrollPos(��ũ���� ���� ��ġ)�� ���Ѵ�.
	VScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
	//4. ���ν�ũ���� ������ġ(VScrollPos)�� ����ȭ���� ���α����� ���� ���Ѵ�.
	VerticalSum = VScrollPos + this->notepadForm->
		scrollController->scroll[1]->GetPageSize();
	//5. ĳ���� ���� ��ġ ������(beginCaretYPos)�� 
	//������ũ���� ������ġ(VScrollPos)���� �޸��忡�� ���� ������
	//ĳ���� ȭ�麸�� ���� ������
	if (beginCaretYPos < VScrollPos)
	{
		//5.1 ������ũ���� �̵��� ��ġ(distance)�� ĳ���� ���� ��ġ ���������� ���Ѵ�.
		distance = beginCaretYPos;
		//5.2 ������ũ�� ���� ��ġ�� distance�� �̵���Ų��.
		this->notepadForm->scrollController->scroll[1]->Move(distance);
		//5.3 ������ũ�ѹ��� Thumb�� ������ũ���� ������ġ�� �̵���Ų��.
		this->notepadForm->SetScrollPos(SB_VERT,
			this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
	}
	//6. ĳ���� ������ġ ������(endCaretYPos)�� ������ũ���� ���� ��ġ(VScrollPos)�� 
	//����ȭ���� ���α����� ��(VerticalSum)���� �޸��忡�� �Ʒ��� ������
	//ĳ���� ȭ�麸�� �Ʒ��� ������
	else if (endCaretYPos > VerticalSum)
	{
		//6.1 ������ũ���� �̵��� ����(distance)�� ���Ѵ�.
		//ĳ���� ���� ��ġ ������ �������� ȭ���� ���α����� ���� ���Ѵ�.
		distance = endCaretYPos - this->notepadForm->
			scrollController->scroll[1]->GetPageSize();
		//6.2 ������ũ���� ������ġ�� distance�� �̵���Ų��.
		this->notepadForm->scrollController->scroll[1]->Move(distance);
		//6.3 ������ũ�ѹ��� Thumb�� ������ũ���� ���� ��ġ�� �̵���Ų��.
		this->notepadForm->SetScrollPos(SB_VERT,
			this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
	}
	//7. �޸��忡�� ĳ���� ������ġ(caretXPos)�� ���Ѵ�.
	caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
		current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
	//8. ����ũ���� �̵��ϱ����� ����ũ���� ������ġ(HScrollPos)�� ���Ѵ�.
	HScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
	//9. ����ũ���� ������ġ(HScrollPos)�� ����ȭ���� ���α����� ���� ���Ѵ�.
	HorizontalSum = HScrollPos + this->notepadForm->
		scrollController->scroll[0]->GetPageSize();
	//10. ĳ���� ���� ������ġ(caretXPos)�� ����ũ���� ������ġ(HScrollPos)���� ������
	//�޸��忡�� ĳ���� ȭ���� ��� ���ʿ� ������
	if (caretXPos < HScrollPos)
	{
		//10.1 ����ũ���� �̵��� ��ġ(distance)�� ���Ѵ�.
		distance = caretXPos - (this->notepadForm->
			scrollController->scroll[0]->GetPageSize() / 5);
		//10.2 distance�� �����̸�
		if (distance < 0)
		{
			//10.2.1 distance�� 0���� �ٲ��ش�.
			distance = 0;
		}
		//10.3 ����ũ���� ������ġ�� distance�� �̵���Ų��.
		this->notepadForm->scrollController->scroll[0]->Move(distance);
		//10.4 ����ũ�ѹ��� Thumb�� ����ũ���� ���� ��ġ�� �̵���Ų��.
		this->notepadForm->SetScrollPos(SB_HORZ,
			this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
	}
	//11. ĳ���� ���� ������ġ(caretXPos)�� ����ũ���� ������ġ(HScrollPos)��
	//����ȭ���� ���α����� ��(HorizontalSum)���� ũ��
	//�޸��忡�� ĳ���� ȭ���� ��� �����ʿ� ������
	else if (caretXPos > HorizontalSum)
	{
		//11.1 ����ũ���� �̵��� ��ġ(distance)�� ���Ѵ�.
		distance = caretXPos - (this->notepadForm->
			scrollController->scroll[0]->GetPageSize() / 5 * 4);
		//11.2 ����ũ�ѹ��� Thumb�� �ִ� �̵���(maxScrollPos)�� ���Ѵ�.
		maxScrollPos = this->notepadForm->scrollController->scroll[0]->GetMax()
			- this->notepadForm->scrollController->scroll[0]->GetPageSize();
		//11.3 distance�� maxScrollPos���� ũ��
		if (distance > maxScrollPos)
		{
			//11.3.1 distance�� maxScrollPos�� �����Ѵ�.
			distance = maxScrollPos;
		}
		//11.4 ����ũ���� ������ġ�� distance�� �̵���Ų��.
		this->notepadForm->scrollController->scroll[0]->Move(distance);
		//11.5 ����ũ�ѹ��� Thumb�� ����ũ���� ���� ��ġ�� �̵���Ų��.
		this->notepadForm->SetScrollPos(SB_HORZ,
			this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
	}
}

//�Ҹ���
PageMoveController::~PageMoveController()
{

}