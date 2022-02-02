#include "FirstScrollAction.h"
#include "ScrollController.h"
#include "Scroll.h"

//����Ʈ������
FirstScrollAction::FirstScrollAction(NotepadForm* notepadForm)
	:ScrollAction(notepadForm)
{

}

void FirstScrollAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. ����ũ���� ó������ �̵��Ѵ�.
	this->notepadForm->scrollController->scroll[0]->First();	
	//2. ����ũ�ѹ��� ����ũ���� ó������ �̵���Ų��.
	this->notepadForm->SetScrollPos(SB_HORZ, 
		this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
}

void FirstScrollAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. ������ũ���� ó������ �̵��Ѵ�.
	this->notepadForm->scrollController->scroll[1]->First();
	//2. ������ũ�ѹ��� ������ũ���� ó������ �̵���Ų��.
	this->notepadForm->SetScrollPos(SB_VERT,
		this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
}

//�Ҹ���
FirstScrollAction::~FirstScrollAction()
{

}