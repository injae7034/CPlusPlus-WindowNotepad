#ifndef _LEFTARROWKEYACTION_H
#define _LEFTARROWKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class LeftArrowKeyAction : public KeyAction
{
public:
	LeftArrowKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~LeftArrowKeyAction();//¼Ò¸êÀÚ
};

#endif // !_LEFTARROWKEYACTION_H

