#ifndef _SHIFTPAGEUPKEYACTION_H
#define _SHIFTPAGEUPKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class ShiftPageUpKeyAction : public KeyAction
{
public:
	ShiftPageUpKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~ShiftPageUpKeyAction();//¼Ò¸êÀÚ
};

#endif // !_SHIFTPAGEUPKEYACTION_H