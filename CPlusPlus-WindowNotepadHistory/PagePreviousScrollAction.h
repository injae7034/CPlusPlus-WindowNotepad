#ifndef _PAGEPREVIOUSSCROLLACTION_H
#define _PAGEPREVIOUSSCROLLACTION_H

#include "ScrollAction.h"

class PagePreviousScrollAction :public ScrollAction
{
public:
	PagePreviousScrollAction(NotepadForm* notepadForm = 0);//����Ʈ������
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//����(��������)
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//����(��������)
	~PagePreviousScrollAction();
};

#endif // !_PAGEPREVIOUSSCROLLACTION_H
