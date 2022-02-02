#ifndef _CTRLZKEYACTION_H
#define _CTRLZKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class CtrlZKeyAction : public KeyAction
{
public:
	CtrlZKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~CtrlZKeyAction();//¼Ò¸êÀÚ
};

#endif // !_CTRLZKEYACTION_H