#ifndef _KEYACTION_H
#define _KEYACTION_H

#include "NotepadForm.h"

class KeyAction
{
public:
	KeyAction(NotepadForm* notepadForm);//����Ʈ������
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) = 0;//Strategy Patter
	virtual ~KeyAction() = 0;//�߻�Ŭ���� �Ҹ���
	//�ζ����Լ�����
	NotepadForm* GetNotepadForm() const;
protected:
	NotepadForm* notepadForm;
};

//�ζ����Լ� ����
inline NotepadForm* KeyAction::GetNotepadForm() const
{
	return this->notepadForm;
}

#endif // !_KEYACTION_H
