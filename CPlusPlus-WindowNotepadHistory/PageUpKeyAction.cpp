#include "PageUpKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"

//����Ʈ������
PageUpKeyAction::PageUpKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void PageUpKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ������ũ���� �� �� �������� �̵��Ѵ�.
	this->notepadForm->scrollController->scroll[1]->PagePrevious();
	//2. ������ũ�ѹ��� ������ũ���� �� �� �������� �̵���Ų��.
	this->notepadForm->SetScrollPos(SB_VERT,
		this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
}

//�Ҹ���
PageUpKeyAction::~PageUpKeyAction()
{

}