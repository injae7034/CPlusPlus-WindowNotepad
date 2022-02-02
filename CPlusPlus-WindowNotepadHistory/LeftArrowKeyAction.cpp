#include "LeftArrowKeyAction.h"
#include "Glyph.h"

//����Ʈ������
LeftArrowKeyAction::LeftArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void LeftArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. �������� �̵��ϱ� ���� ĳ���� ���� ������ġ�� �����Ѵ�.
	Long previousIndex = this->notepadForm->current->GetCurrent();
	//2. �������� �̵��ϰ� ���� ĳ���� ������ġ�� ��ȯ�޴´�.
	Long currentIndex = this->notepadForm->current->Previous();
	//3. ĳ���� ���� ������ġ�� ĳ���� ���� ������ġ�� ������
	if (previousIndex == currentIndex)
	{
		//3.1 �������� �̵��ϱ� ���� ĳ���� ���� ���� ��ġ�� �����Ѵ�.
		Long previousRowIndex = this->notepadForm->note->GetCurrent();
		//3.2 ĳ���� ���� ���� ��ġ�� ���� �ٷ� �̵���Ų��.
		Long currentRowIndex = this->notepadForm->note->Previous();
		//3.3 ĳ���� ���� ���� ��ġ�� ĳ���� ���� ���� ��ġ�� ���� �ٸ���
		if (previousRowIndex != currentRowIndex)
		{
			//3.3.1 ĳ���� ���� ���� �����Ѵ�.
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
			//3.3.2 ĳ���� ���� ���� ��ġ�� �����Ѵ�.
			this->notepadForm->current->Last();
		}
	}
}

//�Ҹ���
LeftArrowKeyAction::~LeftArrowKeyAction()
{

}