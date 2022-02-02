#ifndef _CTRLENDKEYACTION_H
#define _CTRLENDKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class CtrlEndKeyAction : public KeyAction
{
public:
	CtrlEndKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~CtrlEndKeyAction();//¼Ò¸êÀÚ
};

#endif // !_CTRLENDKEYACTION_H