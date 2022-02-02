#ifndef _SHIFTRIGHTARROWKEYACTION_H
#define _SHIFTRIGHTARROWKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class ShiftRightArrowKeyAction : public KeyAction
{
public:
	ShiftRightArrowKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~ShiftRightArrowKeyAction();//¼Ò¸êÀÚ
};

#endif // !_SHIFTRIGHTARROWKEYACTION_H

