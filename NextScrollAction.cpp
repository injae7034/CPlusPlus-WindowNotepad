#include "NextScrollAction.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"

//����Ʈ������
NextScrollAction::NextScrollAction(NotepadForm* notepadForm)
	:ScrollAction(notepadForm)
{

}

void NextScrollAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{	
	//1. ����ũ���� �� ĭ �������� �̵��Ѵ�.
	this->notepadForm->scrollController->scroll[0]->LineNext();
	//2. ����ũ�ѹ��� ����ũ���� �� ĭ �������� �̵���Ų��.
	this->notepadForm->SetScrollPos(SB_HORZ,
		this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
	
}

void NextScrollAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. ������ũ���� �� �� �������� �̵��Ѵ�.
	this->notepadForm->scrollController->scroll[1]->LineNext();
	//2. ������ũ�ѹ��� ������ũ���� ���������� �̵���Ų��.
	this->notepadForm->SetScrollPos(SB_VERT,
		this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
	
}

//�Ҹ���
NextScrollAction::~NextScrollAction()
{

}