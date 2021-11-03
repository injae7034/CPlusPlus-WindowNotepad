#ifndef _DELETEKEYACTION_H
#define _DELETEKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class DeleteKeyAction : public KeyAction
{
public:
	DeleteKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~DeleteKeyAction();//¼Ò¸êÀÚ
};

#endif // !_DELETEKEYACTION_H
