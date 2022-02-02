#ifndef _CTRLDELETEKEYACTION_H
#define _CTRLDELETEKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class CtrlDeleteKeyAction : public KeyAction
{
public:
	CtrlDeleteKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~CtrlDeleteKeyAction();//¼Ò¸êÀÚ
};

#endif // !_CTRLDELETEKEYACTION_H