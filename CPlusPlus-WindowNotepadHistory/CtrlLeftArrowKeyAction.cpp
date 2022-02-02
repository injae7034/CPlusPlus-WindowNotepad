#include "CtrlLeftArrowKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"

//����Ʈ������
CtrlLeftArrowKeyAction::CtrlLeftArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlLeftArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	Long caretXPos = 0;
	Long caretYPos = 0;
	Long HScrollPos = 0;
	Long YScrollPos = 0;
	Long partScrollHeight = 0;
	Long distance = 0;
	//1. ���� ȭ���� ũ�⸦ ���Ѵ�.
	CRect rect;
	this->notepadForm->GetClientRect(&rect);
	Long pageWidth = rect.Width();
	Long pageHeight = rect.Height();
	//2. �̵��ϱ� ���� ���� ���� ��ġ�� ���Ѵ�.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//3. Note�� PreviousWord�� ȣ���Ѵ�.
	Long currentCaretindex = this->notepadForm->note->PreviousWord();
	//4. �̵��� �Ŀ� ���� ���� ��ġ�� ���Ѵ�.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//5. �������� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(this->notepadForm->note->GetCurrent());
	//6. �������� ĳ���� ���� ��ġ�� �����Ѵ�.
	Long i = this->notepadForm->current->First();
	while (i < currentCaretindex)
	{
		//6.1 ĳ���� �������� �̵���Ų��.
		i = this->notepadForm->current->Next();
	}
	//7. �̵��� �ڿ� ���� ĳ���� ���� ��ġ�� �״���̸�(���� �ٲ���X, �� �� ������ �̵���)
	//���� �ٲ��� �ʾұ� ������ ������ũ���� �Ű澵 �ʿ䰡 ����, ����ũ���� �̵��� �Ű澲�� ��)
	if (previousRowIndex == currentRowIndex)
	{
		//7.1 ���� ĳ���� ���� ��ġ�� ���Ѵ�.
		caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
			current->GetPartOfContent(currentCaretindex));
		//7.2 ����ũ���� ���� ��ġ�� ���Ѵ�.
		HScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
		//7.3 ���� ĳ���� ���� ��ġ�� ����ũ���� ������ġ���� ������
		if (caretXPos < HScrollPos)
		{
			//5.3.1 ����ũ���� �̵��� ������ ���Ѵ�.
			//<-��ư�� ���� ������ ���ν�ũ���� ���� ��ġ���� ������ ���� ȭ���� 5���� 1 ũ�⸸ŭ �̵��Ѵ�. 
			//distance = caretXPos - pageWidth + (pageWidth / 5);

			//�׸��� ���� ���� ��ũ���� ĳ���� ���� ���� ������ �̵���Ű�� ������ ��ũ���� ĳ����
			//�ִ� ȭ������ �̵��� �ؾ��ϴµ� �̵��� ��� �װ͵� �ݿ��ؾ��� ��� Ű�׼ǿ���
			//�̰� ��� �ؼ� ���������� ���ư����� Ȯ���غ���
			//distance���� ������ �Ǹ� Move�� �Ű����� �ڷ����� UINT�� �Է��ϴ� ���� �̻��� ����
			//���� �ǰ� �� �̻��� ���� scroll�� currentPos�� ���� �Ǹ� �ٽ� ������ ����ȴ�
			//�ֳ��ϸ� scroll�� currentPos�� singned�̱� ������ ������ ��ũ�ѿ��� �ּ� �̵��� 0�̰�
			//������ �̵��� �Ұ����ϱ� ������ �ᱹ �������� �Ѱ��ָ� ���߿� ScrollController���� 
			//GetScrollPos�� �ϸ� �������� ��°� �Ұ����ϱ⶧���� �����ؼ� 0�� ��ȯ�ϰ� �ǰ� �׷���
			//�ᱹ�� ��ũ���� ���� ��ġ�� 0���� �̵��ϰ� �ȴ�. �׷��� ������ ���͵� ���ư����ϴµ�
			//���ʿ� distance�� ������ �ȵ��� �ϴ°� �����Ű��Ƽ� if�� ������ �Ǵ� ��츦 ����
			//����ũ���� �������� �̵��� ������ ���Ѵ�.
			distance = caretXPos - (pageWidth / 5);
			//distance�� 0���� ������
			if (distance < 0)
			{
				//distance�� ��ũ���� �̵������̱⶧���� ������ �� �� ���� �ּҰ��� 0�̹Ƿ�
				//0�� �������ش�.
				distance = 0;
			}
			

			//distance = HScrollPos - pageWidth - (pageWidth / 5);
			//distance = HScrollPos - caretXPos - (pageWidth / 5);
			//5.3.2 ĳ���� �̵��� ������ �ʺ�ŭ ����ũ���� �̵������ش�.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//5.3.3 ����ũ�ѹ��� ����ũ���� �̵���Ų��.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
	}
	//8. �̵��� �ڿ� ���� ĳ���� ���� ��ġ�� �ٲ������(���� �ٲ�. ���� �ٿ��� ���� �ٷ� �̵���.)
	//�ٿ��� �ٷ� �̵��ϹǷ� ������ũ���� �Ű����ϸ� �ٹٲ�� �����̵��� �ֱ� ������ ����ũ�ѵ� �Ű�����)
	else
	{
		//8.1 ���� �ٱ����� �� ���� ���̸� ���Ѵ�.(ĳ���� y��ǥ �������� ���Ѵ�.)
		caretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
		//8.2 ������ũ���� ���� ��ġ�� ���Ѵ�.
		YScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
		//8.3 ���� �ٱ����� �� ���� ���̰� ������ũ���� ���� ��ġ���� �۰ų� ������
		if (caretYPos < YScrollPos)
		{
			//8.3.1 ������ũ���� caretYPos�� �̵���Ų��.
			this->notepadForm->scrollController->scroll[1]->Move(caretYPos);
			//8.3.2 ������ũ�ѹ��� ������ũ���� �̵���Ų��.
			this->notepadForm->SetScrollPos(SB_VERT,
				this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
		}
		//8.4 ���� ĳ���� ���� ��ġ(x��ǥ)�� ���Ѵ�.
		caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
			current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
		//8.5 ����ũ���� ���� ��ġ�� ���Ѵ�.
		HScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
		//8.6 ����ȭ���� ���α��̿� ����ũ���� ������ġ�� ���� ���Ѵ�.
		partScrollHeight = pageHeight + HScrollPos;
		//8.6 ���� ĳ���� ���� ��ġ��  ����ȭ���� ���α��̿� ����ũ���� ������ġ�� �պ��� ũ��
		if (caretXPos > partScrollHeight)
		{
			//8.6.1 ĳ���� �̵��� ������ �ʺ�ŭ ����ũ���� �̵������ش�.
			caretXPos -= pageWidth;//����ũ���� ������ġ�� �����Ǿ�����!
			this->notepadForm->scrollController->scroll[0]->Move(caretXPos + 2);
			//8.6.2 ����ũ�ѹ��� ����ũ���� �̵���Ų��.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
	}

}

//�Ҹ���
CtrlLeftArrowKeyAction::~CtrlLeftArrowKeyAction()
{

}