#ifndef _SHIFTCTRLLEFTARROWKEYACTION_H
#define _SHIFTCTRLLEFTARROWKEYACTION_H


#include "KeyAction.h"
#include "NotepadForm.h"

class ShiftCtrlLeftArrowKeyAction : public KeyAction
{
public:
	ShiftCtrlLeftArrowKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~ShiftCtrlLeftArrowKeyAction();//¼Ò¸êÀÚ
};

#endif // !_SHIFTCTRLLEFTARROWKEYACTION_H