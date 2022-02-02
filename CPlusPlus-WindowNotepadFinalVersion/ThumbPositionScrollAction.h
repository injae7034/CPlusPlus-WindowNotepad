#ifndef _THUMBPOSITIONSCROLLACTION_H
#define _THUMBPOSITIONSCROLLACTION_H

#include "ScrollAction.h"

class ThumbPositionScrollAction :public ScrollAction
{
public:
	ThumbPositionScrollAction(NotepadForm* notepadForm = 0);//디폴트생성자
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//연산(전략패턴)
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//연산(전략패턴)
	~ThumbPositionScrollAction();
};

#endif // !_THUMBPOSITIONSCROLLACTION_H
