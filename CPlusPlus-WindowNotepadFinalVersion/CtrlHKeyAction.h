#ifndef _CTRLHKEYACTION_H
#define _CTRLHKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class CtrlHKeyAction : public KeyAction
{
public:
	CtrlHKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~CtrlHKeyAction();//�Ҹ���
};

#endif // !_CTRLHKEYACTION_H