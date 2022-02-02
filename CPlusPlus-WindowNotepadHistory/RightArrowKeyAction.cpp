#include "RightArrowKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"

//����Ʈ������
RightArrowKeyAction::RightArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void RightArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ���� ȭ���� ũ�⸦ ���Ѵ�.
	CRect rect;
	this->notepadForm->GetClientRect(&rect);
	Long pageWidth = rect.Width();
	Long pageHeight = rect.Height();
	Long currentScrollPos = 0;
	Long caretYPos = 0;
	Long caretXPos = 0;
	Long partScrollWidth = 0;
	Long partScrollHeight = 0;
	Long distance = 0;
	//2. �������� �̵��ϱ� ���� ĳ���� ���� ������ġ�� �����Ѵ�.
	Long previousIndex = this->notepadForm->current->GetCurrent();
	//3. �������� �̵��ϰ� ���� ĳ���� ������ġ�� ��ȯ�޴´�.
	Long currentIndex = this->notepadForm->current->Next();
	//4. ĳ���� ���� ������ġ�� ĳ���� ���� ������ġ�� ������(ĳ���� �̵��� ��������)
	//�̵��� �����ٴ� ���� ĳ���� ���� ���� ������ ���ڵڿ� �־ ���̻� �̵��� ���� ���ٴ� ���̴�.
	if (previousIndex == currentIndex)
	{
		//4.1 �������� �̵��ϱ� ���� ĳ���� ���� ������ġ(���� ���� ��ġ)�� �����Ѵ�.
		Long previousRowIndex = this->notepadForm->note->GetCurrent();
		//4.2 ĳ���� ���� ���� ��ġ(���� ���� ��ġ)�� ���� �ٷ� �̵���Ų��.
		Long currentRowIndex = this->notepadForm->note->Next();
		//4.3 ĳ���� ���� ���� ��ġ�� ĳ���� ���� ���� ��ġ�� ���� �ٸ���(���� ���� �̵��� �־�����)
		//���� �̵��� �־��ٴ� ���� �̵��ϱ� ���� ���� ������ ���� �ƴϱ� ������ �̵��� ������.
		//���� ���� ������ ���̾����� ���� �ٷ� �̵� �Ŀ��� ���� �ٰ� ��ġ�� ���� ������ �̵��ѰԾƴ�.
		if (previousRowIndex != currentRowIndex)
		{
			//4.3.1 ĳ���� ���� ���� �����Ѵ�.
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
			//4.3.2 ĳ���� ���� ���� ��ġ�� �����Ѵ�.(���� ���� ���� �ٷ� �Ű����� ������
			//ĳ���� ���� ���� ������ ��ġ���� ���� ���� ó�� ��ġ�� �̵��Ѵ�.)
			this->notepadForm->current->First();
			//4.3.3 ���� �ٱ��� �� ���� ���̸� ���Ѵ�.(ĳ���� ���� ��ġ�� ���Ѵ�.)
			caretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);
			//4.3.4 ������ũ���� ���� ��ġ�� ���Ѵ�.
			currentScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
			//4.3.5 ����ȭ���� ���α��̿� ������ũ���� ������ġ�� ���� ���Ѵ�.
			partScrollHeight = pageHeight + currentScrollPos;
			//4.3.6 ���� �ٱ����� �� ���� ���̰� ����ȭ���� ���α��̿� ������ũ���� ������ġ�� �պ��� ũ��
			if (caretYPos > partScrollHeight)
			{

				//4.3.6.1 ������ũ���� ���� �Ʒ��� �̵���Ų��.
				//this->notepadForm->scrollController->scroll[1]->LineNext();

				//4.3.6.1 ������ũ���� �̵��� ������ ���Ѵ�. 
				//caretYPos -= pageHeight;//�̷����ؾ� ��Ȯ�� �� �پ� �̵���.
				distance = caretYPos - pageHeight;//distance���� ���� �� ���� �� ���� ���ؾ���!
				//distance = caretYPos - partScrollHeight;
				//partScrollHeight�� ����ȭ���� ���α��̿� ���������ũ���� ��ġ�� ���� ���ε�
				//�� ���� ���ָ� distance�� �׻� ������ ���� �Ǿ� ������ ����.
				//Move������ distance��ŭ ���ؼ�(�������Ѽ�) �̵���Ű�°� �ƴ϶�
				//�׳� �Է¹��� distance�� �̵��ϴ� ���̱� ������ ���ʿ� distance���� ���� ��
				//������ ���� ���ؾ���. �׷��� ���� ĳ���� ���� ��ġ���� ���� ȭ��ũ�� ���� ���ָ�
				//���������ũ����������ġ���� �����־ ���� ���̰� ������ distance�� ���� �� �ִ�.
				this->notepadForm->scrollController->scroll[1]->Move(distance);
				//4.3.6.2 ������ũ�ѹ��� ������ũ���� �̵���Ų��.
				this->notepadForm->SetScrollPos(SB_VERT,
					this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
			}
			//4.3.7 ����ũ���� ó������ �̵���Ų��.
			this->notepadForm->scrollController->scroll[0]->First();
			//4.3.8 ����ũ�ѹ��� ����ũ���� �̵���Ų��.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
	}
	//5. ĳ���� ���� ������ġ�� ĳ���� ���� ������ġ�� �ٸ���(ĳ���� �̵��� �־�����)
	//ĳ���� �̵��ߴٴ� ���� ĳ���� �̵��ϱ� ���� ���� ������ ��ġ�� ���� �ʾҴٴ� ����
	//���� ĳ���� Next������ ���� ���������� �� ĭ �̵��߱� ������ ����ũ�Ѹ� �̵����� ���� Ȯ���ϸ� ��
	else
	{
		//5.1 ���� ĳ���� ��ġ�� ���Ѵ�.
		caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
			current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
		//5.2 ����ũ���� ���� ��ġ�� ���Ѵ�. 
		currentScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
		//5.3 ����ȭ���� ���α��̿� ����ũ���� ���� ��ġ�� ���Ѵ�.
		partScrollWidth = pageWidth + currentScrollPos;
		//5.4 ���� ĳ���� ��ġ�� ����ȭ���� ���α��̿� ����ũ���� ���� ��ġ�� ���� ������ ũ��
		if (caretXPos > partScrollWidth)
		{
			
			//5.4.1 ����ũ���� �̵��� ������ ���Ѵ�.
			//caretXPos -= pageWidth;
			//��ũ���� ���� �� ĭ�� �̵��ϸ� �̵��� �ʹ� ª�Ƽ� �̵��� �� ����� ���忡�� �ʹ� �����! 
			
			//5.4.1 ����ũ���� �̵��� ������ ���Ѵ�.
			//->��ư�� ���� ������ ���ν�ũ���� ���� ��ġ���� ������ ���� ȭ���� 5���� 1 ũ�⸸ŭ �̵��Ѵ�. 
			distance = (pageWidth / 5) + currentScrollPos;
			//5.4.2 ĳ���� �̵��� ������ �ʺ�ŭ ����ũ���� �̵������ش�.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//5.4.3 ����ũ�ѹ��� ����ũ���� �̵���Ų��.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
	}
}

//�Ҹ���
RightArrowKeyAction::~RightArrowKeyAction()
{

}

