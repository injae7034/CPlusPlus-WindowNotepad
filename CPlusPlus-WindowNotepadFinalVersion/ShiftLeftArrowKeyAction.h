#ifndef _SHIFTLEFTARROWKEYACTION_H
#define _SHIFTLEFTARROWKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class ShiftLeftArrowKeyAction : public KeyAction
{
public:
	ShiftLeftArrowKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~ShiftLeftArrowKeyAction();//�Ҹ���
};

#endif // !_SHIFTLEFTARROWKEYACTION_H
