#ifndef _PREVIOUSSCROLLACTION_H
#define _PREVIOUSSCROLLACTION_H

#include "ScrollAction.h"

class PreviousScrollAction :public ScrollAction
{
public:
	PreviousScrollAction(NotepadForm* notepadForm = 0);//����Ʈ������
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//����(��������)
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//����(��������)
	~PreviousScrollAction();
};

#endif // !_PREVIOUSSCROLLACTION_H
