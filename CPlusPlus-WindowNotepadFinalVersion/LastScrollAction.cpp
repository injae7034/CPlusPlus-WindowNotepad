#include "LastScrollAction.h"
#include "ScrollController.h"
#include "Scroll.h"

//����Ʈ������
LastScrollAction::LastScrollAction(NotepadForm* notepadForm)
	:ScrollAction(notepadForm)
{

}

//OnHScroll
void LastScrollAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. ����ũ���� ���������� �̵��Ѵ�.
	this->notepadForm->scrollController->scroll[0]->Last();
	//2. ����ũ�ѹ��� ����ũ���� ���������� �̵���Ų��.
	this->notepadForm->SetScrollPos(SB_HORZ, 
		this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
}

//OnVScroll
void LastScrollAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. ������ũ���� ���������� �̵��Ѵ�.
	this->notepadForm->scrollController->scroll[1]->Last();
	//2. ������ũ�ѹ��� ������ũ���� ���������� �̵���Ų��.
	this->notepadForm->SetScrollPos(SB_VERT, 
		this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
}

//�Ҹ���
LastScrollAction::~LastScrollAction()
{

}