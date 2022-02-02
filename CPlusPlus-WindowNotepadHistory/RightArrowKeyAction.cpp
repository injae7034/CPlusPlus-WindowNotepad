#include "RightArrowKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"

//����Ʈ������
RightArrowKeyAction::RightArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void RightArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ���� ȭ���� ���� ���̸� ���Ѵ�.
	CRect rect;
	this->notepadForm->GetClientRect(&rect);
	Long pageWidth = rect.Width();
	//2. �������� �̵��ϱ� ���� ĳ���� ���� ������ġ�� �����Ѵ�.
	Long previousIndex = this->notepadForm->current->GetCurrent();
	//3. �������� �̵��ϰ� ���� ĳ���� ������ġ�� ��ȯ�޴´�.
	Long currentIndex = this->notepadForm->current->Next();
	//4. ĳ���� ���� ������ġ�� ĳ���� ���� ������ġ�� ������(ĳ���� �̵��� ��������)
	//�̵��� �����ٴ� ���� ĳ���� ���� ���� ������ ���ڵڿ� �־ ���̻� �̵��� ���� ���ٴ� ���̴�.
	if (previousIndex == currentIndex)
	{
		//3.1 �������� �̵��ϱ� ���� ĳ���� ���� ������ġ(���� ���� ��ġ)�� �����Ѵ�.
		Long previousRowIndex = this->notepadForm->note->GetCurrent();
		//3.2 ĳ���� ���� ���� ��ġ(���� ���� ��ġ)�� ���� �ٷ� �̵���Ų��.
		Long currentRowIndex = this->notepadForm->note->Next();
		//3.3 ĳ���� ���� ���� ��ġ�� ĳ���� ���� ���� ��ġ�� ���� �ٸ���(���� ���� �̵��� �־�����)
		//���� �̵��� �־��ٴ� ���� �̵��ϱ� ���� ���� ������ ���� �ƴϱ� ������ �̵��� ������.
		//���� ���� ������ ���̾����� ���� �ٷ� �̵� �Ŀ��� ���� �ٰ� ��ġ�� ���� ������ �̵��ѰԾƴ�.
		if (previousRowIndex != currentRowIndex)
		{
			//3.3.1 ĳ���� ���� ���� �����Ѵ�.
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
			//3.3.2 ĳ���� ���� ���� ��ġ�� �����Ѵ�.(���� ���� ���� �ٷ� �Ű����� ������
			//ĳ���� ���� ���� ������ ��ġ���� ���� ���� ó�� ��ġ�� �̵��Ѵ�.)
			this->notepadForm->current->First();

			//3.3.3 ����ũ���� ���� ó������ �̵���Ų��.
			//this->notepadForm->scrollController->scroll[0]->First();
			//3.3.4 ������ũ���� �� �� �������� �̵���Ų��.
			//this->notepadForm->scrollController->scroll[1]->LineNext();
			//3.3.5 ����ũ�ѹ��� ����ũ���� �̵���Ų��.
			//this->notepadForm->SetScrollPos(SB_HORZ,
			//	this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
			//2. ������ũ�ѹ��� ������ũ���� �̵���Ų��.
			//this->notepadForm->SetScrollPos(SB_VERT,
			//	this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
		}
	}
}

//�Ҹ���
RightArrowKeyAction::~RightArrowKeyAction()
{

}

