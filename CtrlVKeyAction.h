#ifndef _CTRLVKEYACTION_H
#define _CTRLVKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class CtrlVKeyAction : public KeyAction
{
public:
	CtrlVKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~CtrlVKeyAction();//¼Ò¸êÀÚ
};

#endif // !_CTRLVKEYACTION_H