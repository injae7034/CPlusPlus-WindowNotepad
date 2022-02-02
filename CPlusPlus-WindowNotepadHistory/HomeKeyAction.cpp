#include "HomeKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"

//����Ʈ������
HomeKeyAction::HomeKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void HomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	Long VScrollPos = 0;//���ν�ũ���� ������ġ
	Long beginCaretYPos = 0;//ĳ���� ������ġ ��������
	Long endCaretYPos = 0;//ĳ���� ������ġ ��������
	Long VerticalSum = 0;//���ν�ũ���� ������ġ + ����ȭ���� ���α���
	Long distance = 0;//��ũ���� �̵��� ��ġ
	Long currentRowIndex = 0;//�������� �̵��� �Ŀ� ���� ���� ��ġ

	//1. ���� ���� �� ó������ ĳ���� ���� ��ġ�� �̵���Ų��.
	Long currentScrollPos = this->notepadForm->current->First();
	//2. ����ũ���� ó������ �̵������ش�.
	this->notepadForm->scrollController->scroll[0]->Move(currentScrollPos);
	//3. ����ũ�ѹ��� ����ũ���� �̵���Ų��.
	this->notepadForm->SetScrollPos(SB_HORZ,
		this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
	//4. ĳ���� ���� ���� ��ġ�� ���Ѵ�.
	currentRowIndex = this->notepadForm->note->GetCurrent();
	//5. caret �̵� �Ŀ� ĳ���� ���� ����(���۰� ��)�� ���Ѵ�.(beginCaretYPos, endCaretYPos)
	beginCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
	endCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);
	//6. ������ũ���� �̵��ϱ� ���� ���� ������ũ���� VScrollPos(��ũ���� ���� ��ġ)�� ���Ѵ�.
	VScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
	//7. ������ũ���� ������ġ(VScrollPos)�� ����ȭ���� ���α����� ���� ���Ѵ�.
	VerticalSum = VScrollPos + this->notepadForm->
		scrollController->scroll[1]->GetPageSize();
	//8. ĳ���� ���� ��ġ ������(beginCaretYPos)�� 
	//������ũ���� ������ġ(VScrollPos)���� �޸��忡�� ���� ������
	if (beginCaretYPos < VScrollPos)
	{
		//8.1 ������ũ���� �̵��� ��ġ(distance)�� ĳ���� ���� ��ġ ���������� ���Ѵ�.
		distance = beginCaretYPos;
		//8.2 ������ũ�� ���� ��ġ�� distance�� �̵���Ų��.
		this->notepadForm->scrollController->scroll[1]->Move(distance);
		//8.3 ������ũ�ѹ��� Thumb�� ������ũ���� ������ġ�� �̵���Ų��.
		this->notepadForm->SetScrollPos(SB_VERT,
			this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
	}
	//9. ĳ���� ������ġ ������(endCaretYPos)�� ������ũ���� ���� ��ġ(VScrollPos)�� 
	//����ȭ���� ���α����� ��(VerticalSum)���� �޸��忡�� �Ʒ��� ������
	else if (endCaretYPos > VerticalSum)
	{
		//9.1 ������ũ���� �̵��� ����(distance)�� ���Ѵ�.
		//ĳ���� ���� ��ġ ������ �������� ȭ���� ���α����� ���� ���Ѵ�.
		distance = endCaretYPos - this->notepadForm->
			scrollController->scroll[1]->GetPageSize();
		//9.2 �Ʒ��� �̵��� �� ���� ���ڰ� ���߸����� �ϱ� ���ؼ� ������ ���Ѵ�.
		Long pageSize = this->notepadForm->scrollController->scroll[1]->GetPageSize();
		Long letterHeight = this->notepadForm->textExtent->GetHeight();
		Long blank = pageSize - (pageSize / letterHeight * letterHeight);
		//9.3 ������ũ���� ������ġ�� distance + ����(blank)���� �̵���Ų��.
		this->notepadForm->scrollController->scroll[1]->Move(distance + blank);
		//9.3 ������ũ�ѹ��� Thumb�� ������ũ���� ���� ��ġ�� �̵���Ų��.
		this->notepadForm->SetScrollPos(SB_VERT,
			this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
	}
}

//�Ҹ���
HomeKeyAction::~HomeKeyAction()
{

}