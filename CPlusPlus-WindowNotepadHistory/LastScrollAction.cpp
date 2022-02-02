#include "LastScrollAction.h"
#include "ScrollController.h"
#include "Scroll.h"

//디폴트생성자
LastScrollAction::LastScrollAction(NotepadForm* notepadForm)
	:ScrollAction(notepadForm)
{

}

//OnHScroll
void LastScrollAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. 옵저버 리스트 배열에서 해당위치의 옵저버를 구한다.
	Long i = 0;
	Observer* observer = this->notepadForm->GetAt(i);
	//2. i가 length보다 작은 동안 옵저버가 캐럿매니저가 아닌동안 반복한다.
	while (i < this->notepadForm->GetLength()
		&& dynamic_cast<ScrollController*>(observer) != observer)
	{
		//2.1 옵저버 리스트에서 옵저버를 구한다.
		observer = this->notepadForm->GetAt(i);
		//2.2 i를 증가시킨다.
		i++;
	}
	//3. ScrollController이면
	if (dynamic_cast<ScrollController*>(observer))
	{
		//3.1 스크롤을 마지막으로 이동한다.
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
	//1. 옵저버 리스트 배열에서 해당위치의 옵저버를 구한다.
	Long i = 0;
	Observer* observer = this->notepadForm->GetAt(i);
	//2. i가 length보다 작은 동안 옵저버가 캐럿매니저가 아닌동안 반복한다.
	while (i < this->notepadForm->GetLength()
		&& dynamic_cast<ScrollController*>(observer) != observer)
	{
		//2.1 옵저버 리스트에서 옵저버를 구한다.
		observer = this->notepadForm->GetAt(i);
		//2.2 i를 증가시킨다.
		i++;
	}
	//3. ScrollController이면
	if (dynamic_cast<ScrollController*>(observer))
	{
		//3.1 스크롤을 가장 아래로 이동한다.
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

//소멸자
LastScrollAction::~LastScrollAction()
{

}