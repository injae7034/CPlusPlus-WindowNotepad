#ifndef _RIGHTARROWKEYACTION_H
#define _RIGHTARROWKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class RightArrowKeyAction : public KeyAction
{
public:
	RightArrowKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~RightArrowKeyAction();//¼Ò¸êÀÚ
};

#endif // !_RIGHTARROWKEYACTION_H

