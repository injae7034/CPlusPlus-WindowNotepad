#ifndef _PAGENEXTSCROLLACTION_H
#define _PAGENEXTSCROLLACTION_H

#include "ScrollAction.h"

class PageNextScrollAction :public ScrollAction
{
public:
	PageNextScrollAction(NotepadForm* notepadForm = 0);//����Ʈ������
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//����(��������)
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//����(��������)
	~PageNextScrollAction();
};

#endif // !_PAGENEXTSCROLLACTION_H
