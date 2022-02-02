#ifndef _CTRLRIGHTARROWKEYACTION_H
#define _CTRLRIGHTARROWKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class CtrlRightArrowKeyAction : public KeyAction
{
public:
	CtrlRightArrowKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~CtrlRightArrowKeyAction();//¼Ò¸êÀÚ
};

#endif // !_CTRLRIGHTARROWKEYACTION_H

