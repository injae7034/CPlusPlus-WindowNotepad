#ifndef _THUMBPOSITIONSCROLLACTION_H
#define _THUMBPOSITIONSCROLLACTION_H

#include "ScrollAction.h"

class ThumbPositionScrollAction :public ScrollAction
{
public:
	ThumbPositionScrollAction(NotepadForm* notepadForm = 0);//����Ʈ������
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//����(��������)
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//����(��������)
	~ThumbPositionScrollAction();
};

#endif // !_THUMBPOSITIONSCROLLACTION_H
