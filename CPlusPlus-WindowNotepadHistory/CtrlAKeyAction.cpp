#include "CtrlAKeyAction.h"
#include "Glyph.h"
#include "SelectText.h"

//����Ʈ������
CtrlAKeyAction::CtrlAKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//��������
void CtrlAKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ������ �ִ� ���� ������ �����Ѵ�.
	SelectText selectText(this->notepadForm);
	selectText.Undo();
	//2. ������ �ٷ� �̵��ϱ� ���� ���� ��ġ�� �����Ѵ�.
	Long previousRowIndex = 0;
	//3. ���������� ���ڷ� �̵��ϱ� ���� ������ ��ġ�� �����Ѵ�.
	Long previousLetterIndex = 0;
	//4. �޸����� ���� ������ �ٷ� �̵��Ѵ�.
	Long currentRowIndex = this->notepadForm->note->Last();
	//5. �޸����� ���� ���� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	//6. ���� ���� ������ ���ڷ� �̵��Ѵ�.
	Long currentLetterIndex = this->notepadForm->current->Last();
	//7. ��ü ���ڸ� �����Ѵ�.
	selectText.DoNext(previousRowIndex, previousLetterIndex, currentRowIndex,
		currentLetterIndex);
	
}

//�Ҹ���
CtrlAKeyAction::~CtrlAKeyAction()
{

}