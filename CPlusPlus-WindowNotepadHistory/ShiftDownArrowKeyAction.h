#ifndef _SHIFTDOWNARROWKEYACTION_H
#define _SHIFTDOWNARROWKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class ShiftDownArrowKeyAction : public KeyAction
{
public:
	ShiftDownArrowKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~ShiftDownArrowKeyAction();//¼Ò¸êÀÚ
};

#endif // !_SHIFTDOWNARROWKEYACTION_H
