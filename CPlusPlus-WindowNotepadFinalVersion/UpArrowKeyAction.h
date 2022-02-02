#ifndef _UPARROWKEYACTION_H
#define _UPARROWKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class UpArrowKeyAction : public KeyAction
{
public:
	UpArrowKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~UpArrowKeyAction();//¼Ò¸êÀÚ
};

#endif // !_UPARROWKEYACTION_H