#ifndef _BACKSPACEKEYACTION_H
#define _BACKSPACEKEYACTION_H

#include "../KeyAction.h"
#include "../NotepadForm.h"

class BackSpaceKeyAction : public KeyAction
{
public:
	BackSpaceKeyAction(NotepadForm* notepadForm = 0);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	~BackSpaceKeyAction();//¼Ò¸êÀÚ
};

#endif // !_BACKSPACEKEYACTION_H