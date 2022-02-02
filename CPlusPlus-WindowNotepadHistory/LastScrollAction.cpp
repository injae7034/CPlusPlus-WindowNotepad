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
	//1. ������ ����Ʈ �迭���� �ش���ġ�� �������� ���Ѵ�.
	Long i = 0;
	Observer* observer = this->notepadForm->GetAt(i);
	//2. i�� length���� ���� ���� �������� ĳ���Ŵ����� �ƴѵ��� �ݺ��Ѵ�.
	while (i < this->notepadForm->GetLength()
		&& dynamic_cast<ScrollController*>(observer) != observer)
	{
		//2.1 ������ ����Ʈ���� �������� ���Ѵ�.
		observer = this->notepadForm->GetAt(i);
		//2.2 i�� ������Ų��.
		i++;
	}
	//3. ScrollController�̸�
	if (dynamic_cast<ScrollController*>(observer))
	{
		//3.1 ��ũ���� ���������� �̵��Ѵ�.
		dynamic_cast<ScrollController*>(observer)->scroll[0]->Last();
		SCROLLINFO scrollInfo;
		scrollInfo.nMin = dynamic_cast<ScrollController*>(observer)->scroll[0]->GetMin();
		scrollInfo.nMax = dynamic_cast<ScrollController*>(observer)->scroll[0]->GetMax();
		scrollInfo.nPage = dynamic_cast<ScrollController*>(observer)->scroll[0]->GetPageSize();
		scrollInfo.nPos = dynamic_cast<ScrollController*>(observer)->scroll[0]->GetCurrentPos();
		dynamic_cast<ScrollController*>(observer)->GetHorizontalScrollBar()->
			SetScrollInfo(&scrollInfo);
	}
}

//OnVScroll
void LastScrollAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. ������ ����Ʈ �迭���� �ش���ġ�� �������� ���Ѵ�.
	Long i = 0;
	Observer* observer = this->notepadForm->GetAt(i);
	//2. i�� length���� ���� ���� �������� ĳ���Ŵ����� �ƴѵ��� �ݺ��Ѵ�.
	while (i < this->notepadForm->GetLength()
		&& dynamic_cast<ScrollController*>(observer) != observer)
	{
		//2.1 ������ ����Ʈ���� �������� ���Ѵ�.
		observer = this->notepadForm->GetAt(i);
		//2.2 i�� ������Ų��.
		i++;
	}
	//3. ScrollController�̸�
	if (dynamic_cast<ScrollController*>(observer))
	{
		//3.1 ��ũ���� ���� �Ʒ��� �̵��Ѵ�.
		dynamic_cast<ScrollController*>(observer)->scroll[1]->Last();
		SCROLLINFO scrollInfo;
		scrollInfo.cbSize = sizeof(SCROLLINFO);
		scrollInfo.fMask = SIF_ALL;
		scrollInfo.nMin = dynamic_cast<ScrollController*>(observer)->scroll[1]->GetMin();
		scrollInfo.nMax = dynamic_cast<ScrollController*>(observer)->scroll[1]->GetMax();
		scrollInfo.nPage = dynamic_cast<ScrollController*>(observer)->scroll[1]->GetPageSize();
		scrollInfo.nPos = dynamic_cast<ScrollController*>(observer)->scroll[1]->GetCurrentPos();
		scrollInfo.nTrackPos = 2;
		dynamic_cast<ScrollController*>(observer)->GetVerticalScrollBar()->
			SetScrollInfo(&scrollInfo);
		//dynamic_cast<ScrollController*>(observer)->GetVerticalScrollBar()->
		//		SetScrollPos(scrollInfo.nPos);
	}
}

//�Ҹ���
LastScrollAction::~LastScrollAction()
{

}