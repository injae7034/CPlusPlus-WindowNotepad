#ifndef _CTRLSHIFTSKEYACTION_H
#define _CTRLSHIFTSKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class CtrlShiftSKeyAction : public KeyAction
{
public:
	CtrlShiftSKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~CtrlShiftSKeyAction();//¼Ò¸êÀÚ
};

#endif // !_CTRLSHIFTSKEYACTION_H