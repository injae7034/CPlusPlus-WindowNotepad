#include "CtrlEndKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"

//����Ʈ������
CtrlEndKeyAction::CtrlEndKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	Long distance;
	//1. �޸����� ���� ���������� ĳ���� ���� ��ġ�� �̵���Ų��.
	Long lastIndex = this->notepadForm->note->Last();
	//2. �޸����� ���� ���� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(lastIndex);
	//3. ���� ���� �� ���������� ĳ���� ���� ��ġ�� �̵���Ų��.
	this->notepadForm->current->Last();
	//4. ���� �ٱ��� �� ���� ���̸� ���Ѵ�.(ĳ���� ���� ��ġ�� ���Ѵ�.)
	Long caretYPos = this->notepadForm->textExtent->GetHeight() * (lastIndex + 1);
	//5. ������ũ���� �̵��� �� �ִ� ���������� �̵���Ų��.
	Long maxVScroll = caretYPos - this->notepadForm->scrollController->
		scroll[1]->GetPageSize();
	this->notepadForm->scrollController->scroll[1]->Move(maxVScroll);
	//6. ����ũ�ѹ��� ����ũ���� �̵���Ų��.
	this->notepadForm->SetScrollPos(SB_VERT,
		this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
	//7. ���� ���� ĳ���� ���� ��ġ�� ���Ѵ�.
	Long caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
		current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
	//8. ����ũ���� �̵��ϱ����� ����ũ���� ������ġ(HScrollPos)�� ���Ѵ�.
	Long HScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
	//9. ����ũ���� ������ġ(HScrollPos)�� ����ȭ���� ���α����� ���� ���Ѵ�.
	Long HorizontalSum = HScrollPos + this->notepadForm->
		scrollController->scroll[0]->GetPageSize();
	//10. ĳ���� ���� ������ġ(caretXPos)�� ����ũ���� ������ġ(HScrollPos)����
		//�޸��忡�� �տ� ������
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
	//����ȭ���� ���α����� ��(HorizontalSum)���� �޸��忡�� �ڿ� ������
	else if (caretXPos > HorizontalSum)
	{
		//11.1 ����ũ���� �̵��� ��ġ(distance)�� ���Ѵ�.
		distance = caretXPos - (this->notepadForm->
			scrollController->scroll[0]->GetPageSize() / 5 * 4);
		//11.2 ����ũ�ѹ��� Thumb�� �ִ� �̵���(maxScrollPos)�� ���Ѵ�.
		Long maxScrollPos = this->notepadForm->scrollController->scroll[0]->GetMax()
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
CtrlEndKeyAction::~CtrlEndKeyAction()
{

}