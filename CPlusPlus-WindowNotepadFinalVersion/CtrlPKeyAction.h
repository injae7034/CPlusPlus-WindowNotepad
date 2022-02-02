#ifndef _CTRLPKEYACTION_H
#define _CTRLPKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class CtrlPKeyAction : public KeyAction
{
public:
	CtrlPKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~CtrlPKeyAction();//¼Ò¸êÀÚ
};

#endif // !_CTRLPKEYACTION_H