#ifndef _CTRLYKEYACTION_H
#define _CTRLYKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class CtrlYKeyAction : public KeyAction
{
public:
	CtrlYKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~CtrlYKeyAction();//�Ҹ���
};

#endif // !_CTRLYKEYACTION_H