#ifndef _CTRLNKEYACTION_H
#define _CTRLNKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class CtrlNKeyAction : public KeyAction
{
public:
	CtrlNKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~CtrlNKeyAction();//¼Ò¸êÀÚ
};

#endif // !_CTRLNKEYACTION_H