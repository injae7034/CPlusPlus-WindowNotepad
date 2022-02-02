#ifndef _PAGEPREVIOUSSCROLLACTION_H
#define _PAGEPREVIOUSSCROLLACTION_H

#include "ScrollAction.h"

class PagePreviousScrollAction :public ScrollAction
{
public:
	PagePreviousScrollAction(NotepadForm* notepadForm = 0);//디폴트생성자
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//연산(전략패턴)
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//연산(전략패턴)
	~PagePreviousScrollAction();
};

#endif // !_PAGEPREVIOUSSCROLLACTION_H
