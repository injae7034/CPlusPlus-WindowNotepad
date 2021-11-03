#ifndef _CTRLOKEYACTION_H
#define _CTRLOKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class CtrlOKeyAction : public KeyAction
{
public:
	CtrlOKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~CtrlOKeyAction();//¼Ò¸êÀÚ
};

#endif // !_CTRLOKEYACTION_H