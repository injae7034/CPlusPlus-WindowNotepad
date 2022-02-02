#ifndef _SHIFTCTRLDELETEKEYACTION_H
#define _SHIFTCTRLDELETEKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class ShiftCtrlDeleteKeyAction : public KeyAction
{
public:
	ShiftCtrlDeleteKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~ShiftCtrlDeleteKeyAction();//¼Ò¸êÀÚ
};

#endif // !_SHIFTCTRLDELETEKEYACTION_H