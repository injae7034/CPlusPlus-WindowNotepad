#ifndef _PAGEDOWNKEYACTION_H
#define _PAGEDOWNKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class PageDownKeyAction : public KeyAction
{
public:
	PageDownKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~PageDownKeyAction();//¼Ò¸êÀÚ
};

#endif // !_PAGEDOWNKEYACTION_H
