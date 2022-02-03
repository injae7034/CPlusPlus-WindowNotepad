#ifndef _CTRLBACKSPACEKEYACTION_H
#define _CTRLBACKSPACEKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class CtrlBackSpaceKeyAction : public KeyAction
{
public:
	CtrlBackSpaceKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~CtrlBackSpaceKeyAction();//¼Ò¸êÀÚ
};

#endif // !_CTRLBACKSPACEKEYACTION_H