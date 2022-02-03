#ifndef _SHIFTHOMEKEYACTION_H
#define _SHIFTHOMEKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class ShiftHomeKeyAction : public KeyAction
{
public:
	ShiftHomeKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~ShiftHomeKeyAction();//¼Ò¸êÀÚ
};

#endif // !_SHIFTHOMEKEYACTION_H
