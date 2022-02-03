#ifndef _SHIFTCTRLBACKSPACEKEYACTION_H
#define _SHIFTCTRLBACKSPACEKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class ShiftCtrlBackSpaceKeyAction : public KeyAction
{
public:
	ShiftCtrlBackSpaceKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~ShiftCtrlBackSpaceKeyAction();//¼Ò¸êÀÚ
};

#endif // !_SHIFTCTRLBACKSPACEKEYACTION_H