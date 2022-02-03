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
	//1. �������� �̵��ϱ� ���� ĳ���� ��ġ�� �����Ѵ�.
	Long previousCaretIndex = this->notepadForm->current->GetCurrent();
	//2. �������� �̵��ϰ� ���� ĳ���� ��ġ�� ��ȯ�޴´�.
	Long currentCaretIndex = this->notepadForm->current->Next();
	//3. �������� �̵��ϱ� �� ĳ���� ��ġ�� �������� �̵��� ���� ĳ���� ��ġ�� ������(ĳ���� �̵��� ��������)
	//�̵��� �����ٴ� ���� ĳ���� ���� ������ ���� �ڿ� �־ ���̻� �̵��� ���� ���ٴ� ���̴�.
	if (previousCaretIndex == currentCaretIndex)
	{
		//3.1 �������� �̵��ϱ� ���� ���� ��ġ�� �����Ѵ�.
		Long previousRowIndex = this->notepadForm->note->GetCurrent();
		//3.2 ���� ���� ��ġ�� ���� �ٷ� �̵���Ų��.
		Long currentRowIndex = this->notepadForm->note->Next();
		//3.3 �������� �̵��ϱ� ���� ���� ��ġ�� �������� �̵��� �Ŀ� ���� ��ġ�� ���� �ٸ���
		//(���� ���� �̵��� �־�����)
		//���� �̵��� �־��ٴ� ���� �̵��ϱ� ���� ���� ������ ���� �ƴϱ� ������ �̵��� ������.
		//���� ���� ������ ���̾����� ���� �ٷ� �̵� �Ŀ��� ���� �ٰ� ��ġ�� ���� ������ �̵��ѰԾƴ�.
		//���� ���������� ������ ó������ �̵�
		if (previousRowIndex != currentRowIndex)
		{
			//3.3.1 ���� ���� �������� �̵��� �ٷ� �����Ѵ�.
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
			//3.3.2 ĳ���� ���� ��ġ�� ó������ ������.(���� ���� ���� �ٷ� �Ű����� ������
			//ĳ���� ���� ���� ������ ��ġ���� ���� ���� ó�� ��ġ�� �̵��Ѵ�.)
			this->notepadForm->current->First();
		}
	}
}

//�Ҹ���
RightArrowKeyAction::~RightArrowKeyAction()
{

}

