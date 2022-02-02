#include "PageNextScrollAction.h"
#include "ScrollController.h"
#include "Scroll.h"

//����Ʈ������
PageNextScrollAction::PageNextScrollAction(NotepadForm* notepadForm)
	:ScrollAction(notepadForm)
{

}

void PageNextScrollAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

	//1. ����ũ���� �� ������ �������� �̵��Ѵ�.
	this->notepadForm->scrollController->scroll[0]->PageNext();
	//2. ����ũ�ѹ��� ����ũ���� �� ĭ �������� �̵���Ų��.
	this->notepadForm->SetScrollPos(SB_HORZ,
		this->notepadForm->scrollController->scroll[0]->GetCurrentPos());

}

void PageNextScrollAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. ������ũ���� �� �� �������� �̵��Ѵ�.
	this->notepadForm->scrollController->scroll[1]->PageNext();
	//2. ������ũ�ѹ��� ������ũ���� �� �� �������� �̵���Ų��.
	this->notepadForm->SetScrollPos(SB_VERT,
		this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
}

//�Ҹ���
PageNextScrollAction::~PageNextScrollAction()
{

}