#ifndef _CTRLFKEYACTION_H
#define _CTRLFKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class CtrlFKeyAction : public KeyAction
{
public:
	CtrlFKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~CtrlFKeyAction();//�Ҹ���
};

#endif // !_CTRLFKEYACTION_H