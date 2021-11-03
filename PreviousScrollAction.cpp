#include "PreviousScrollAction.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"

//����Ʈ������
PreviousScrollAction::PreviousScrollAction(NotepadForm* notepadForm)
	:ScrollAction(notepadForm)
{

}

void PreviousScrollAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	
	//1. ����ũ���� �� ĭ �������� �̵��Ѵ�.
	this->notepadForm->scrollController->scroll[0]->LinePrevious();
	//2. ����ũ�ѹ��� ����ũ���� �� ĭ �������� �̵���Ų��.
	this->notepadForm->SetScrollPos(SB_HORZ,
		this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
	
}

void PreviousScrollAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. ������ũ���� �� �� �������� �̵��Ѵ�.
	this->notepadForm->scrollController->scroll[1]->LinePrevious();
	//2. ������ũ�ѹ��� ������ũ���� �� �� �������� �̵���Ų��.
	this->notepadForm->SetScrollPos(SB_VERT,
		this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
}

//�Ҹ���
PreviousScrollAction::~PreviousScrollAction()
{

}