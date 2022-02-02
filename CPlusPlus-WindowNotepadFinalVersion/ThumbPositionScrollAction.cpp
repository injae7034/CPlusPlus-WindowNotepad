#include "ThumbPositionScrollAction.h"
#include "ScrollController.h"
#include "Scroll.h"

//����Ʈ������
ThumbPositionScrollAction::ThumbPositionScrollAction(NotepadForm* notepadForm)
	:ScrollAction(notepadForm)
{

}

void ThumbPositionScrollAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

	//1. ����ũ���� �� ������ �������� �̵��Ѵ�.
	this->notepadForm->scrollController->scroll[0]->Move(nPos);
	//2. ����ũ�ѹ��� ����ũ���� �� ĭ �������� �̵���Ų��.
	this->notepadForm->SetScrollPos(SB_HORZ,
		this->notepadForm->scrollController->scroll[0]->GetCurrentPos());

}

void ThumbPositionScrollAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. ������ũ���� �� �� �������� �̵��Ѵ�.
	this->notepadForm->scrollController->scroll[1]->Move(nPos);
	//2. ������ũ�ѹ��� ������ũ���� �� �� �������� �̵���Ų��.
	this->notepadForm->SetScrollPos(SB_VERT,
		this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
}

//�Ҹ���
ThumbPositionScrollAction::~ThumbPositionScrollAction()
{

}