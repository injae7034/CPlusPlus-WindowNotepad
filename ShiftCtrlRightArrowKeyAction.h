#ifndef _SHIFTCTRLRIGHTARROWKEYACTION_H
#define _SHIFTCTRLRIGHTARROWKEYACTION_H


#include "KeyAction.h"
#include "NotepadForm.h"

class ShiftCtrlRightArrowKeyAction : public KeyAction
{
public:
	ShiftCtrlRightArrowKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~ShiftCtrlRightArrowKeyAction();//�Ҹ���
};

#endif // !_SHIFTCTRLRIGHTARROWKEYACTION_H
