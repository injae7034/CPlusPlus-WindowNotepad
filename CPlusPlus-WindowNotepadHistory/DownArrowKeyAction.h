#ifndef _DOWNARROWKEYACTION_H
#define _DOWNARROWKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class DownArrowKeyAction : public KeyAction
{
public:
	DownArrowKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~DownArrowKeyAction();//¼Ò¸êÀÚ
};

#endif // !_DOWNARROWKEYACTION_H