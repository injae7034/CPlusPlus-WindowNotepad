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
	Long caretXPos = 0;
	Long caretYPos = 0;
	Long HScrollPos = 0;
	Long YScrollPos = 0;
	Long partScrollWidth = 0;
	Long partScrollHeight = 0;
	Long distance = 0;
	//1. ���� ȭ���� ũ�⸦ ���Ѵ�.
	CRect rect;
	this->notepadForm->GetClientRect(&rect);
	Long pageWidth = rect.Width();
	Long pageHeight = rect.Height();
	//2. �̵��ϱ� ���� ���� ���� ��ġ�� ���Ѵ�.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//3. Note�� NextWord�� ȣ���Ѵ�.
	Long currentCaretindex = this->notepadForm->note->NextWord();
	//4. �̵��� �Ŀ� ���� ���� ��ġ�� ���Ѵ�.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//5. �������� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	//6. �������� ĳ���� ���� ��ġ�� �����Ѵ�.
	Long i = this->notepadForm->current->First();
	while (i < currentCaretindex)
	{
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
		//7.3 ���� ȭ���� ���α��̿� ����ũ���� ���� ��ġ���� ���� ���Ѵ�.
		partScrollWidth = pageWidth + HScrollPos;
		//7.4 ���� ĳ���� ������ġ�� ����ȭ���� ���α��̿� ����ũ���� ������ġ�� �պ��� ũ��
		if (caretXPos > partScrollWidth)
		{
			//7.4.1 ����ũ���� �̵��� ������ ���Ѵ�.
			//�Ʒ� �Ĵ�� �ϸ� ������ ���� distance���� �����ϰ� ������ Move������ ���������� 
			//�ƴ϶� �ش���ġ�� �̵��ϴ� �����̴� �׷��� distance�� ���� ���� �� ���ʿ� ������ �� ����
			//���������. ������ ���� ���ϱ� ���ؼ��� HScrollPos���� ���� �ʰ� �״�� �־�
			//HScrollPos���� careXPos�� pageWidth�� �� ���� ������ �Ǿ� distance�� �����
			//HScrollPos���� ���ָ� HScrollPos�� 0�϶��� ������ ������ HScrollPos�� 0�� �ƴ� ���
			//distance���� ������ ���� Move�� ��� ������ ��ġ�� �ӹ��� �ȴ�.
			//distance = caretXPos - (pageWidth + HScrollPos) + (pageWidth / 5);

			distance = caretXPos - pageWidth + (pageWidth / 5);
			//distance = caretXPos + (pageWidth / 5);
			//7.4.2 ����ũ���� �̵������ش�.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//7.4.3 ����ũ�ѹ��� ����ũ���� �̵���Ų��.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
	}
	//8. �̵��� �ڿ� ���� ĳ���� ���� ��ġ�� �ٲ������(���� �ٲ�. ���� �ٿ��� ���� �ٷ� �̵���.)
	//�ٿ��� �ٷ� �̵��ϹǷ� ������ũ���� �Ű����ϸ� �ٹٲ�� �����̵��� �ֱ� ������ ����ũ�ѵ� �Ű�����)
	else
	{
		//8.1 ĳ���� ���� ��ġ�� ���Ѵ�.(���ڳ��� X �������� ��ġ)
		caretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);
		//8.2 ������ũ���� ���� ��ġ�� ���Ѵ�.
		YScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
		//8.3 ����ȭ���� ���α��̿� ������ũ���� ������ġ�� ���� ���Ѵ�.
		partScrollHeight = pageHeight + YScrollPos;
		//8.4 ���� ĳ���� ���� ��ġ�� ���� ȭ���� ���α��̿� ������ũ���� ������ġ���� ũ��
		if (caretYPos > partScrollHeight)//partScrollHeight���� Ŀ�� ��Ȯ�ϰ� ������ũ���� �̵�������
		{
			//8.4.1 ������ũ���� �̵��� ������ ���Ѵ�.
			//distance = caretYPos - partScrollHeight;// �̷����ϸ� ������ ��� ���ڸ�������
			
			//���� ���⿡ ����������!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			//distance = caretYPos - pageHeight;//�̷����ؾ� YScrollPos�� ��� ������ ���� ����.
			distance = caretYPos - pageHeight;
			//8.4.2 ������ũ���� �̵���Ų��.
			this->notepadForm->scrollController->scroll[1]->Move(distance);
			//8.4.3 ������ũ�ѹ��� ������ũ���� �̵���Ų��.
			this->notepadForm->SetScrollPos(SB_VERT,
				this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
		}
		//8.5 ����ũ���� ó������ �̵���Ų��.(���� �ٲ���� ������)
		this->notepadForm->scrollController->scroll[0]->First();
		//8.6 ����ũ�ѹ��� ����ũ���� �̵���Ų��.
		this->notepadForm->SetScrollPos(SB_HORZ,
			this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		//8.7 ���� ĳ���� ���� ��ġ�� ���Ѵ�.
		caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
			current->GetPartOfContent(currentCaretindex));
		//8.8 ���� ĳ���� ���� ��ġ�� ���� ȭ���� ���� ���̺��� ũ��
		if (caretXPos > pageWidth)
		{
			//8.8.1 ����ũ���� �̵��� ������ ���Ѵ�.
			distance = caretXPos - pageWidth + (pageWidth / 5);
			//distance = caretXPos + (pageWidth / 5);
			//8.8.2 ����ũ���� �̵������ش�.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//8.8.3 ����ũ�ѹ��� ����ũ���� �̵���Ų��.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
	}

}

//�Ҹ���
CtrlRightArrowKeyAction::~CtrlRightArrowKeyAction()
{

}