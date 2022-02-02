#ifndef _HOMEKEYACTION_H
#define _HOMEKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class HomeKeyAction : public KeyAction
{
public:
	HomeKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~HomeKeyAction();//¼Ò¸êÀÚ
};

#endif // !_HOMEKEYACTION_H