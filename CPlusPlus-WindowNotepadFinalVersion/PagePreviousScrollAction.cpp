#include "PagePreviousScrollAction.h"
#include "ScrollController.h"
#include "Scroll.h"

//����Ʈ������
PagePreviousScrollAction::PagePreviousScrollAction(NotepadForm* notepadForm)
	:ScrollAction(notepadForm)
{

}

void PagePreviousScrollAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

	//1. ����ũ���� �� ������ �������� �̵��Ѵ�.
	this->notepadForm->scrollController->scroll[0]->PagePrevious();
	//2. ����ũ�ѹ��� ����ũ���� �� ĭ �������� �̵���Ų��.
	this->notepadForm->SetScrollPos(SB_HORZ,
		this->notepadForm->scrollController->scroll[0]->GetCurrentPos());

}

void PagePreviousScrollAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. ������ũ���� �� �� �������� �̵��Ѵ�.
	this->notepadForm->scrollController->scroll[1]->PagePrevious();
	//2. ������ũ�ѹ��� ������ũ���� �� �� �������� �̵���Ų��.
	this->notepadForm->SetScrollPos(SB_VERT,
		this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
}

//�Ҹ���
PagePreviousScrollAction::~PagePreviousScrollAction()
{

}