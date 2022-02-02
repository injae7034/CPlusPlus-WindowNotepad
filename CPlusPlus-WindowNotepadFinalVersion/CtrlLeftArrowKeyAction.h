#ifndef _CTRLLEFTARROWKEYACTION_H
#define _CTRLLEFTARROWKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class CtrlLeftArrowKeyAction : public KeyAction
{
public:
	CtrlLeftArrowKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~CtrlLeftArrowKeyAction();//¼Ò¸êÀÚ
};

#endif // !_CTRLLEFTARROWKEYACTION_H