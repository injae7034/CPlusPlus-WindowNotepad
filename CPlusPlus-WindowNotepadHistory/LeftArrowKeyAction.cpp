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
	//1. �������� �̵��ϱ� ���� ĳ���� ��ġ�� �����Ѵ�.
	Long previousCaretIndex = this->notepadForm->current->GetCurrent();
	//2. �������� �̵��ϰ� ĳ���� ��ġ�� ��ȯ�޴´�.
	Long currentCaretIndex = this->notepadForm->current->Previous();
	//3. �������� �̵��ϱ� �� ĳ���� ��ġ�� �������� �̵��� ���� ĳ���� ��ġ�� ������(ĳ���� �̵��� ��������)
	//�̵��� �����ٴ� ���� ĳ���� ���� ���� ó���� �־ ���̻� �̵��� ���� ���ٴ� ���̴�.
	if (previousCaretIndex == currentCaretIndex)
	{
		//3.1 �������� �̵��ϱ� ���� ���� ��ġ�� �����Ѵ�.
		Long previousRowIndex = this->notepadForm->note->GetCurrent();
		//3.2 ���� �ٷ� �̵���Ų��.
		Long currentRowIndex = this->notepadForm->note->Previous();
		//3.3 �������� �̵��ϱ� ���� ���� ��ġ�� �������� �̵��� �Ŀ� ���� ��ġ�� ���� �ٸ���
		//(���� ���� �̵��� �־�����)
		//���� �̵��� �־��ٴ� ���� �̵��ϱ� ���� ���� �޸��忡�� ���� ó�� ���� �ƴϱ� ������ �̵��� ������.
		//���� ���� ó�� ���̾����� ���� �ٷ� �̵� �Ŀ��� ���� �ٰ� ��ġ�� ���� ������ �̵��ѰԾƴ�.
		//���� ó������ ���� ���� ���������� �̵�
		if (previousRowIndex != currentRowIndex)
		{
			//3.3.1 ���� ���� �������� �̵��� �ٷ� �����Ѵ�.
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
			//3.3.2 ���� �������� �̵��߱� ������ ĳ���� ���� ��ġ�� ���������� �����Ѵ�.
			this->notepadForm->current->Last();
		}
	}
}

//�Ҹ���
LeftArrowKeyAction::~LeftArrowKeyAction()
{

}