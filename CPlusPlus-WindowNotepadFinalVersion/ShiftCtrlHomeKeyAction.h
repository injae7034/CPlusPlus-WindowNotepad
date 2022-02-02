#ifndef _SHIFTCTRLHOMEKEYACTION_H
#define _SHIFTCTRLHOMEKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class ShiftCtrlHomeKeyAction : public KeyAction
{
public:
	ShiftCtrlHomeKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~ShiftCtrlHomeKeyAction();//¼Ò¸êÀÚ
};

#endif // !_SHIFTCTRLHOMEKEYACTION_H