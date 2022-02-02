#ifndef _CTRLHOMEKEYACTION_H
#define _CTRLHOMEKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class CtrlHomeKeyAction : public KeyAction
{
public:
	CtrlHomeKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~CtrlHomeKeyAction();//¼Ò¸êÀÚ
};

#endif // !_CTRLHOMEKEYACTION_H