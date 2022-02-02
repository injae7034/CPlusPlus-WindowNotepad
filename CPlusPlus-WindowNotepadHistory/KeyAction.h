#ifndef _KEYACTION_H
#define _KEYACTION_H

#include "NotepadForm.h"

class KeyAction
{
public:
	KeyAction(NotepadForm* notepadForm);//디폴트생성자
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) = 0;//Strategy Patter
	virtual ~KeyAction() = 0;//추상클래스 소멸자
	//인라인함수정의
	NotepadForm* GetNotepadForm() const;
protected:
	NotepadForm* notepadForm;
};

//인라인함수 정의
inline NotepadForm* KeyAction::GetNotepadForm() const
{
	return this->notepadForm;
}

#endif // !_KEYACTION_H
