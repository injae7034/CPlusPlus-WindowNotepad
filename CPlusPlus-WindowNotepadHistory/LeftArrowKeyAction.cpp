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
	//1. ���� ȭ���� ũ�⸦ ���Ѵ�.
	CRect rect;
	this->notepadForm->GetClientRect(&rect);
	Long pageWidth = rect.Width();
	Long pageHeight = rect.Height();
	Long HScrollPos = 0;
	Long VScrollPos = 0;
	Long caretYPos = 0;
	Long caretXPos = 0;
	Long sum = 0;
	Long distance = 0;
	//2. �������� �̵��ϱ� ���� ĳ���� ���� ������ġ�� �����Ѵ�.
	Long previousIndex = this->notepadForm->current->GetCurrent();
	//3. �������� �̵��ϰ� ���� ĳ���� ������ġ�� ��ȯ�޴´�.
	Long currentIndex = this->notepadForm->current->Previous();
	//4. ĳ���� ���� ������ġ�� ĳ���� ���� ������ġ�� ������(ĳ���� �̵��� ��������)
	//�̵��� �����ٴ� ���� ĳ���� ���� ���� ���� ó���� ��ġ�ؼ� ���̻� �̵��� ���� ���ٴ� ���̴�.
	if (previousIndex == currentIndex)
	{
		//4.1 �������� �̵��ϱ� ���� ĳ���� ���� ���� ��ġ�� �����Ѵ�.
		Long previousRowIndex = this->notepadForm->note->GetCurrent();
		//4.2 ĳ���� ���� ���� ��ġ�� ���� �ٷ� �̵���Ų��.
		Long currentRowIndex = this->notepadForm->note->Previous();
		//4.3 ĳ���� ���� ���� ��ġ�� ĳ���� ���� ���� ��ġ�� ���� �ٸ���
		if (previousRowIndex != currentRowIndex)
		{
			//4.3.1 ĳ���� ���� ���� �����Ѵ�.
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
			//4.3.2 ĳ���� ���� ���� ��ġ�� �����Ѵ�.
			this->notepadForm->current->Last();

			//4.3.3 ���� �ٱ��� �� ���� ���̸� ���Ѵ�.(ĳ���� ���� ��ġ�� ���Ѵ�.)
			//caretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);

			//4.3.3.���� �ٱ����� �� ���� ���̸� ���Ѵ�.(ĳ���� y��ǥ �������� ���Ѵ�.)
			//������ũ���� ���� ��ġ�� ���ڵ��� ���̴� �ݿ��Ǿ� ���� �ʱ� ������ �����ٱ����� ���̸� ���ؾ�
			//���� ĳ���� y��ǥ�� �������� �� �� �ִ� ���� �ٱ����� �� ���� ���̸� ���ϸ�
			//���ڵ��� ���̰� �ݿ��� �Ǿ ������ũ���� ���� ��ġ�� ���� �� ����!
			caretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
			//4.3.4 ������ũ���� ���� ��ġ�� ���Ѵ�.
			VScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
			//4.3.5 ����ȭ���� ���� ���̿� ������ũ���� ���� ��ġ�� ���� ���Ѵ�.
			sum = pageHeight + VScrollPos;
			//4.3.6 ���� �ٱ����� �� ���� ���̰� ������ũ���� ���� ��ġ���� �۰ų� �Ǵ�
			//���� �ٱ����� �� ���� ���̰� ����ȭ���� ���� ���̿� ������ũ���� ���� ��ġ�� �պ���
			//ũ�ų� ������

			if (caretYPos < VScrollPos || caretYPos > sum)
			{

				//4.3.6.1 ������ũ���� ���� ���� �̵���Ų��.
				//this->notepadForm->scrollController->scroll[1]->LinePrevious();
				//caretYPos += pageHeight;//�̷����ؾ� ��Ȯ�� �� �پ� �̵���.

				//4.3.6.1 ������ũ���� caretYPos�� �̵���Ų��.
				this->notepadForm->scrollController->scroll[1]->Move(caretYPos);
				//4.3.6.2 ������ũ�ѹ��� ������ũ���� �̵���Ų��.
				this->notepadForm->SetScrollPos(SB_VERT,
					this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
			}


			//4.3.7 ���� ĳ���� ���� ��ġ(x��ǥ)�� ���Ѵ�.
			caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
				current->GetPartOfContent(this->notepadForm->current->GetCurrent()));

			//�� ��Ȳ������ ����ũ���� ���� ��ġ�� ������ 0�̱� ������ ���� ���� �ʿ䰡 ����.
			//4.3.7 ����ũ���� ���� ��ġ�� ���Ѵ�.
			//currentScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);

			//4.3.8 ����ȭ��ũ�⺸�� ���� ĳ���� ���� ��ġ(x��ǥ)�� ��ũ��
			if (caretXPos > pageWidth)
			{
				//4.3.8.1 ĳ���� �̵��� ������ �ʺ�ŭ ����ũ���� �̵������ش�.
				caretXPos -= pageWidth;//����ũ���� ������ġ�� �����Ǿ�����!
				this->notepadForm->scrollController->scroll[0]->Move(caretXPos + 2);
				//4.3.8.2 ����ũ�ѹ��� ����ũ���� �̵���Ų��.
				this->notepadForm->SetScrollPos(SB_HORZ,
					this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
			}

		}
	}
	//5. ĳ���� ���� ������ġ�� ĳ���� ���� ������ġ�� �ٸ���(ĳ���� �̵��� �־�����)
	//ĳ���� �̵��ߴٴ� ���� ĳ���� �̵��ϱ� ���� ���� ó���� ���� �ʾҴٴ� ����
	//���� ĳ���� Previous������ ���� ���������� �� ĭ �̵��߱� ������
	//����ũ�Ѹ� �̵����� ���� Ȯ���ϸ� ��
	else
	{
		//5.1 ���� ĳ���� x��ǥ�� ���Ѵ�.(Previous�� �̵��� ���� x��ǥ)
		caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
			current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
		//5.2 ����ũ���� ���� ��ġ�� ���Ѵ�. 
		HScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
		//5.3 ���� ĳ���� ��ġ�� ����ũ���� ���� ��ġ���� ������
		if (caretXPos < HScrollPos)
		{
			//5.3.1 ����ũ���� �̵��� ������ ���Ѵ�.
			//<-��ư�� ���� ������ ���ν�ũ���� ���� ��ġ���� ������ ���� ȭ���� 5���� 1 ũ�⸸ŭ �̵��Ѵ�. 
			distance = HScrollPos - (pageWidth / 5);
			//5.3.2 ĳ���� �̵��� ������ �ʺ�ŭ ����ũ���� �̵������ش�.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//5.3.3 ����ũ�ѹ��� ����ũ���� �̵���Ų��.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
	}
}

//�Ҹ���
LeftArrowKeyAction::~LeftArrowKeyAction()
{

}