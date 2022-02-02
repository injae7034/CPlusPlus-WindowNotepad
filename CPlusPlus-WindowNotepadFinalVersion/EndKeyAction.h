#ifndef _ENDKEYACTION_H
#define _ENDKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class EndKeyAction : public KeyAction
{
public:
	EndKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~EndKeyAction();//¼Ò¸êÀÚ
};

#endif // !_ENDKEYACTION_H