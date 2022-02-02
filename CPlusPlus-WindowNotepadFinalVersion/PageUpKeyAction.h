#ifndef _PAGEUPKEYACTION_H
#define _PAGEUPKEYACTION_H

#include "KeyAction.h"
#include "NotepadForm.h"

class PageUpKeyAction : public KeyAction
{
public:
	PageUpKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~PageUpKeyAction();//¼Ò¸êÀÚ
};

#endif // !_PAGEUPKEYACTION_H
