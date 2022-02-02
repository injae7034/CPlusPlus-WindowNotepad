#ifndef _LASTSCROLLACTION_H
#define _LASTSCROLLACTION_H

#include "ScrollAction.h"

class LastScrollAction :public ScrollAction
{
public:
	LastScrollAction(NotepadForm* notepadForm = 0);//디폴트생성자
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//연산(전략패턴)
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//연산(전략패턴)
	~LastScrollAction();
};

#endif // !_LASTSCROLLACTION_H
