#ifndef _SCROLLACTION_H
#define _SCROLLACTION_H

#include "NotepadForm.h"

class ScrollAction
{
public:
	NotepadForm* notepadForm;
public:
	ScrollAction(NotepadForm* notepadForm = 0);//디폴트생성자
	virtual void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) = 0;//연산(전략패턴)
	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) = 0;//연산(전략패턴)
	virtual ~ScrollAction() = 0;
};

#endif // !_SCROLLACTION_H
