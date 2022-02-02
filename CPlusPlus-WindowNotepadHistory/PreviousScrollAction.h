#ifndef _PREVIOUSSCROLLACTION_H
#define _PREVIOUSSCROLLACTION_H

#include "ScrollAction.h"

class PreviousScrollAction :public ScrollAction
{
public:
	PreviousScrollAction(NotepadForm* notepadForm = 0);//디폴트생성자
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//연산(전략패턴)
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//연산(전략패턴)
	~PreviousScrollAction();
};

#endif // !_PREVIOUSSCROLLACTION_H
