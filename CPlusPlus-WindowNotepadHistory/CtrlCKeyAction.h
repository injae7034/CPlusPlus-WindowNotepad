#ifndef _CTRLCKEYACTION_H
#define _CTRLCKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class CtrlCKeyAction : public KeyAction
{
public:
	CtrlCKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~CtrlCKeyAction();//�Ҹ���
};

#endif // !_CTRLCKEYACTION_H