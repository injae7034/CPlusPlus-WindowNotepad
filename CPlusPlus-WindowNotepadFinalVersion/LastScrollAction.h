#ifndef _LASTSCROLLACTION_H
#define _LASTSCROLLACTION_H

#include "ScrollAction.h"

class LastScrollAction :public ScrollAction
{
public:
	LastScrollAction(NotepadForm* notepadForm = 0);//����Ʈ������
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//����(��������)
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//����(��������)
	~LastScrollAction();
};

#endif // !_LASTSCROLLACTION_H
