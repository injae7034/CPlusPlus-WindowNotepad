#ifndef _NEXTSCROLLACTION_H
#define _NEXTSCROLLACTION_H

#include "ScrollAction.h"

class NextScrollAction :public ScrollAction
{
public:
	NextScrollAction(NotepadForm* notepadForm = 0);//����Ʈ������
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//����(��������)
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//����(��������)
	~NextScrollAction();
};

#endif // !_NEXTSCROLLACTION_H
