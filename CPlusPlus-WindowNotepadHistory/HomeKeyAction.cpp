#include "HomeKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"

//����Ʈ������
HomeKeyAction::HomeKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void HomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ���� ���� �� ó������ ĳ���� ���� ��ġ�� �̵���Ų��.
	Long currentScrollPos = this->notepadForm->current->First();
	//2. ����ũ���� ó������ �̵������ش�.
	this->notepadForm->scrollController->scroll[0]->Move(currentScrollPos);
	//3. ����ũ�ѹ��� ����ũ���� �̵���Ų��.
	this->notepadForm->SetScrollPos(SB_HORZ,
		this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
}

//�Ҹ���
HomeKeyAction::~HomeKeyAction()
{

}