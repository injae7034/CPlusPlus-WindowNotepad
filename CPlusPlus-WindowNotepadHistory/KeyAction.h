#ifndef _KEYACTION_H
#define _KEYACTION_H

#include "NotepadForm.h"

class KeyAction
{
public:
	NotepadForm* notepadForm;
public:
	KeyAction(NotepadForm* notepadForm = 0);//����Ʈ������
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) = 0;//Strategy Pattern
	virtual ~KeyAction() = 0;//�߻�Ŭ���� �Ҹ���
};

#endif // !_KEYACTION_H
