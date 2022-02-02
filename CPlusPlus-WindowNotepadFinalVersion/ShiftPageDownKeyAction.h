#ifndef _SHIFTPAGEDOWNKEYACTION_H
#define _SHIFTPAGEDOWNKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class ShiftPageDownKeyAction : public KeyAction
{
public:
	ShiftPageDownKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~ShiftPageDownKeyAction();//¼Ò¸êÀÚ
};

#endif // !_SHIFTPAGEDOWNKEYACTION_H