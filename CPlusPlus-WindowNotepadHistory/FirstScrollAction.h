#ifndef _FIRSTSCROLLACTION_H
#define _FIRSTSCROLLACTION_H

#include "ScrollAction.h"

class FirstScrollAction :public ScrollAction
{
public:
	FirstScrollAction(NotepadForm* notepadForm = 0);//디폴트생성자
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//연산(전략패턴)
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//연산(전략패턴)
	~FirstScrollAction();
};

#endif // _FIRSTSCROLLACTION_H
