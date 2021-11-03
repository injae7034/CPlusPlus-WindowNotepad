#ifndef _SHIFTCTRLENDKEYACTION_H
#define _SHIFTCTRLENDKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class ShiftCtrlEndKeyAction : public KeyAction
{
public:
	ShiftCtrlEndKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~ShiftCtrlEndKeyAction();//¼Ò¸êÀÚ
};

#endif // !_SHIFTCTRLENDKEYACTION_H