#include "CtrlHomeKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"

//����Ʈ������
CtrlHomeKeyAction::CtrlHomeKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. �޸����� ���� ó������ ĳ���� ���� ��ġ�� �̵���Ų��.
	Long firstIndex = this->notepadForm->note->First();
	//2. �޸����� ���� ���� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(firstIndex);
	//3. ���� ���� �� ó������ ĳ���� ���� ��ġ�� �̵���Ų��.
	Long HScrollcurrentPos =this->notepadForm->current->First();
	//4. ����ũ���� ó������ �̵������ش�.
	this->notepadForm->scrollController->scroll[0]->Move(HScrollcurrentPos);
	//5. ����ũ�ѹ��� ����ũ���� �̵���Ų��.
	this->notepadForm->SetScrollPos(SB_HORZ,
		this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
	//6. ������ũ���� ó������ �̵������ش�.
	this->notepadForm->scrollController->scroll[1]->Move(firstIndex);
	//7. ����ũ�ѹ��� ����ũ���� �̵���Ų��.
	this->notepadForm->SetScrollPos(SB_VERT,
		this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
}

//�Ҹ���
CtrlHomeKeyAction::~CtrlHomeKeyAction()
{

}