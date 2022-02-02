#ifndef _NEXTSCROLLACTION_H
#define _NEXTSCROLLACTION_H

#include "ScrollAction.h"

class NextScrollAction :public ScrollAction
{
public:
	NextScrollAction(NotepadForm* notepadForm = 0);//디폴트생성자
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//연산(전략패턴)
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//연산(전략패턴)
	~NextScrollAction();
};

#endif // !_NEXTSCROLLACTION_H
