#ifndef _CTRLXKEYACTION_H
#define _CTRLXKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class CtrlXKeyAction : public KeyAction
{
public:
	CtrlXKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~CtrlXKeyAction();//�Ҹ���
};

#endif // !_CTRLXKEYACTION_H