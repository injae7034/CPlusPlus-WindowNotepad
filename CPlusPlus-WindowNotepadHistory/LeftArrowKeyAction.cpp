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
	Long beginCaretYPos = 0;
	Long endCaretYPos = 0;
	Long caretXPos = 0;
	Long sum = 0;
	Long distance = 0;
	Long previousRowIndex = 0;
	Long currentRowIndex = 0;
	//2. �������� �̵��ϱ� ���� ĳ���� ���� ������ġ�� �����Ѵ�.
	Long previousIndex = this->notepadForm->current->GetCurrent();
	//3. �������� �̵��ϰ� ���� ĳ���� ������ġ�� ��ȯ�޴´�.
	Long currentIndex = this->notepadForm->current->Previous();
	//4. ĳ���� ���� ������ġ�� ĳ���� ���� ������ġ�� ������(ĳ���� �̵��� ��������)
	//�̵��� �����ٴ� ���� ĳ���� ���� ���� ���� ó���� ��ġ�ؼ� ���̻� �̵��� ���� ���ٴ� ���̴�.
	if (previousIndex == currentIndex)
	{
		//4.1 �������� �̵��ϱ� ���� ĳ���� ���� ���� ��ġ�� �����Ѵ�.
		previousRowIndex = this->notepadForm->note->GetCurrent();
		//4.2 ĳ���� ���� ���� ��ġ�� ���� �ٷ� �̵���Ų��.
		currentRowIndex = this->notepadForm->note->Previous();
		//4.3 ĳ���� ���� ���� ��ġ�� ĳ���� ���� ���� ��ġ�� ���� �ٸ���
		if (previousRowIndex != currentRowIndex)
		{
			//4.3.1 ĳ���� ���� ���� �����Ѵ�.
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
			//4.3.2 ĳ���� ���� ���� ��ġ�� �����Ѵ�.
			this->notepadForm->current->Last();
			//4.3.3. ĳ���� �̵� �Ŀ� caretYPos(ĳ���� ���� ��ġ)�� ���Ѵ�.
			//currentRowIndex�� 0�� �����̶� ��ճ��̿� ���� ������ ���ؾ� �ϹǷ� +1�� ����� �Ѵ�. 
			endCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);
			//4.3.4 ������ũ���� �̵��ϱ� ���� ���� ������ũ���� VScrollPos(��ũ���� ���� ��ġ)�� ���Ѵ�.
			VScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
			//4.3.5 caretYPos�� VScrollPos���� ũ��
			if (endCaretYPos > VScrollPos)
			{
				//4.3.5.1 ����ȭ���� ���� ���̿� ������ũ���� ���� ��ġ�� ���� ���Ѵ�.
				sum = pageHeight + VScrollPos;
				//4.3.5.2 caretYPos�� sum���� ũ��
				if (endCaretYPos > sum)
				{
					//4.3.5.2.1 ������ũ���� �̵��� ����(distance)�� ���Ѵ�.
					distance = endCaretYPos - pageHeight;
					//4.3.5.2.2 distance��ŭ ���� ������ũ���� �̵���Ų��.
					this->notepadForm->scrollController->scroll[1]->Move(distance);
					//4.3.5.2.3 ������ũ�ѹ��� ������ũ���� �̵���Ų��.
					this->notepadForm->SetScrollPos(SB_VERT,
						this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
				}
			}
			//4.3.6 caretYPos�� VScrollPos���� ������
			else if (endCaretYPos < VScrollPos)
			{
				//4.3.6.1 ������ũ���� �̵��� ����(distance)�� ���Ѵ�.
				//���� ĳ���� YPos���� ���� �ٱ����� ���̸� ���Ѵ�.
				distance = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
				//4.3.6.2 distance��ŭ ���� ������ũ���� �̵���Ų��.
				this->notepadForm->scrollController->scroll[1]->Move(distance);
				//4.3.6.3 ������ũ�ѹ��� ������ũ���� �̵���Ų��.
				this->notepadForm->SetScrollPos(SB_VERT,
					this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
			}
			//4.3.7 ĳ���� �̵��� �Ŀ� ĳ���� ���� ������ġ(caretXPos)�� ���Ѵ�.
			caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
				current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
			//4.3.8 ��ũ���� �̵��ϱ����� ����ũ���� ������ġ(HScrollPos)�� ���Ѵ�.
			HScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
			//4.3.9 caretXPos�� HScrollPos���� ũ��
			if (caretXPos > HScrollPos)
			{
				//4.3.9.1 ����ȭ���� ���� ���̿� ����ũ���� ���� ��ġ�� ���� ���Ѵ�.
				sum = pageWidth + HScrollPos;
				//4.3.9.2 caretXPos�� sum���� ũ��
				if (caretXPos > sum)
				{
					//4.3.9.2.1 ����ũ���� �̵��� ����(distance)�� ���Ѵ�.
					distance = caretXPos - pageWidth;
					//4.3.9.2.2 distance��ŭ ����ũ���� �̵���Ų��.
					this->notepadForm->scrollController->scroll[0]->Move(distance + 2);
					//4.3.9.2.3 ����ũ�ѹ��� ����ũ���� �̵���Ų��.
					this->notepadForm->SetScrollPos(SB_HORZ,
						this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
				}
			}
			//4.3.10 caretXPos�� HScrollPos���� ������
			else if (caretXPos < HScrollPos)
			{
				//4.3.10.1 ����ũ���� �̵��� ����(distance)�� ���Ѵ�.
				distance = caretXPos - pageWidth;
				//4.3.10.2 distance�� �����̸�
				if (distance < 0)
				{
					//4.3.10.2.1 distance�� 0�� �������ش�.
					distance = 0;
				}
				//4.3.10.3 distance��ŭ ����ũ���� �̵���Ų��.
				this->notepadForm->scrollController->scroll[0]->Move(distance + 2);
				//4.3.10.4 ����ũ�ѹ��� ����ũ���� �̵���Ų��.
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
		//5.2 caret �̵� �Ŀ� ĳ���� ���� ��ġ(caretYPos)�� ���Ѵ�.
		//currentRowIndex�� 0�� �����̶� ��ճ��̿� ���� ������ ���ؾ� �ϹǷ� +1�� ����� �Ѵ�. 
		beginCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
		endCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);
		//5.3 ������ũ���� �̵��ϱ� ���� ���� ������ũ���� VScrollPos(��ũ���� ���� ��ġ)�� ���Ѵ�.
		VScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
		//5.4 caretYPos�� VScrollPos���� ũ��
		if (beginCaretYPos < VScrollPos )
		{
			//5.4.1 ����ȭ���� ���� ���̿� ������ũ���� ���� ��ġ�� ���� ���Ѵ�.
			sum = pageHeight + VScrollPos;
			//5.4.2 caretYPos�� sum���� ũ��
			if (beginCaretYPos > sum)
			{
				//5.4.2.1 ������ũ���� �̵��� ����(distance)�� ���Ѵ�.
				distance = beginCaretYPos - pageHeight;
				//5.4.2.2 distance��ŭ ���� ������ũ���� �̵���Ų��.
				this->notepadForm->scrollController->scroll[1]->Move(distance);
				//5.4.2.3 ������ũ�ѹ��� ������ũ���� �̵���Ų��.
				this->notepadForm->SetScrollPos(SB_VERT,
					this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
			}
		}
		//5.5 caretYPos�� VScrollPos���� ������
		else if (endCaretYPos > VScrollPos + pageHeight)
		{
			//5.5.1 ������ũ���� �̵��� ����(distance)�� ���Ѵ�.
			//���� ĳ���� YPos���� ���� �ٱ����� ���̸� ���Ѵ�.
			distance = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
			//5.5.2 distance��ŭ ���� ������ũ���� �̵���Ų��.
			this->notepadForm->scrollController->scroll[1]->Move(distance);
			//5.5.3 ������ũ�ѹ��� ������ũ���� �̵���Ų��.
			this->notepadForm->SetScrollPos(SB_VERT,
				this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
		}
		//5.6 caret�� �̵��ϱ� ���� ĳ���� ���� ��ġ(caretXPos)�� ���Ѵ�.
		//caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
		//	current->GetPartOfContent(this->notepadForm->current->GetCurrent() + 1));
		//4.3.7 ĳ���� �̵��� �Ŀ� ĳ���� ���� ������ġ(caretXPos)�� ���Ѵ�.
		caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
			current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
		//5.6 ��ũ���� �̵��ϱ����� ����ũ���� ������ġ(HScrollPos)�� ���Ѵ�.
		HScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
		//5.7 caretXPos�� HScrollPos���� ũ��
		if (caretXPos > HScrollPos)
		{
			//5.7.1 ����ȭ���� ���� ���̿� ����ũ���� ���� ��ġ�� ���� ���Ѵ�.
			sum = pageWidth + HScrollPos;
			//5.7.2 caretXPos�� sum���� ũ��
			if (caretXPos > sum)
			{
				//5.7.2.1 ����ũ���� �̵��� ����(distance)�� ���Ѵ�.
				distance = caretXPos - (pageWidth / 5);
				//distance = HScrollPos - (pageWidth / 5);
				//5.7.2.2 distance��ŭ ����ũ���� �̵���Ų��.
				this->notepadForm->scrollController->scroll[0]->Move(distance);
				//5.7.2.3 ����ũ�ѹ��� ����ũ���� �̵���Ų��.
				this->notepadForm->SetScrollPos(SB_HORZ,
					this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
			}
		}
		//5.8 caretXPos�� HScrollPos���� ������
		else if (caretXPos < HScrollPos)
		{
			//5.8.1 ����ũ���� �̵��� ����(distance)�� ���Ѵ�.
			distance = caretXPos - (pageWidth / 5);
			//5.8.2 distance�� �����̸�
			if (distance < 0)
			{
				//5.8.2.1 distance�� 0�� �������ش�.
				distance = 0;
			}
			//5.8.3 distance��ŭ ����ũ���� �̵���Ų��.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//5.8.4 ����ũ�ѹ��� ����ũ���� �̵���Ų��.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
	}
}

//�Ҹ���
LeftArrowKeyAction::~LeftArrowKeyAction()
{

}