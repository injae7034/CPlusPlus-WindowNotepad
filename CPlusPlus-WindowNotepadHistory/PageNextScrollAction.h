#ifndef _PAGENEXTSCROLLACTION_H
#define _PAGENEXTSCROLLACTION_H

#include "ScrollAction.h"

class PageNextScrollAction :public ScrollAction
{
public:
	PageNextScrollAction(NotepadForm* notepadForm = 0);//디폴트생성자
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//연산(전략패턴)
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//연산(전략패턴)
	~PageNextScrollAction();
};

#endif // !_PAGENEXTSCROLLACTION_H
