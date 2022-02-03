#ifndef _SHIFTENDKEYACTION_H
#define _SHIFTENDKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class ShiftEndKeyAction : public KeyAction
{
public:
	ShiftEndKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~ShiftEndKeyAction();//¼Ò¸êÀÚ
};

#endif // !_SHIFTENDKEYACTION_H