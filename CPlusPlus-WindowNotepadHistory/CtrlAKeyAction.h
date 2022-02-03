#ifndef _CTRLAKEYACTION_H
#define _CTRLAKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class CtrlAKeyAction : public KeyAction
{
public:
	CtrlAKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~CtrlAKeyAction();//¼Ò¸êÀÚ
};

#endif // !_CTRLAKEYACTION_H