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
	//1. �̵��ϱ� ���� ���� ��ġ�� ���Ѵ�.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//2. ��Ʈ���� �ܾ������ ���������� �̵��Ѵ�.
	Long currentCaretindex = this->notepadForm->note->NextWord();
	//3. �̵��� �Ŀ� ���� ��ġ�� ���Ѵ�.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//4. ���� ���� �̵��� ���� �ٷ� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	//5. �ٿ��� ĳ���� �ܾ������ �̵��Ѵ�.
	Long i = this->notepadForm->current->First();
	while (i < currentCaretindex)
	{
		//5.1 ĳ���� �������� �̵���Ų��.
		i = this->notepadForm->current->Next();
	}
}

//�Ҹ���
CtrlRightArrowKeyAction::~CtrlRightArrowKeyAction()
{

}