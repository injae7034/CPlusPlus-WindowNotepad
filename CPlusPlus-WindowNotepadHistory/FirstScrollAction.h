#ifndef _FIRSTSCROLLACTION_H
#define _FIRSTSCROLLACTION_H

#include "ScrollAction.h"

class FirstScrollAction :public ScrollAction
{
public:
	FirstScrollAction(NotepadForm* notepadForm = 0);//����Ʈ������
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//����(��������)
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//����(��������)
	~FirstScrollAction();
};

#endif // _FIRSTSCROLLACTION_H
