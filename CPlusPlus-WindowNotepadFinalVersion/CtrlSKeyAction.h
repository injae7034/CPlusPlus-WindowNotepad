#ifndef _CTRLSKEYACTION_H
#define _CTRLSKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class CtrlSKeyAction : public KeyAction
{
public:
	CtrlSKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~CtrlSKeyAction();//�Ҹ���
};

#endif // !_CTRLSKEYACTION_H