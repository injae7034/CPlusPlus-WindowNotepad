#ifndef _SHIFTUPARROWKEYACTION_H
#define _SHIFTUPARROWKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class ShiftUpArrowKeyAction : public KeyAction
{
public:
	ShiftUpArrowKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~ShiftUpArrowKeyAction();//�Ҹ���
};

#endif // !_SHIFTUPARROWKEYACTION_H