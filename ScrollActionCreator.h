#ifndef _SCROLLACTIONCREATOR_H
#define _SCROLLACTIONCREATOR_H

#include "NotepadForm.h"

class ScrollAction;

class ScrollActionCreator
{
public:
	NotepadForm* notepadForm;//연관화
public:
	ScrollActionCreator(NotepadForm* notepadForm = 0);//디폴트생성자
	ScrollAction* Create(UINT nSBCode);//팩토리메소드패턴
	~ScrollActionCreator();//소멸자
};

#endif // !_SCROLLACTIONCREATOR_H
