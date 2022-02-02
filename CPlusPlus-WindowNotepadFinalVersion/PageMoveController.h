#ifndef _PAGEMOVECONTROLLER_H
#define _PAGEMOVECONTROLLER_H

#include "Observer.h"

class NotepadForm;

class PageMoveController :public Observer
{
public:
	NotepadForm* notepadForm;
public:
	PageMoveController(NotepadForm* notepadForm = 0);//디폴트생성자
	virtual void Update();
	~PageMoveController();//소멸자
};

#endif // !_PAGEMOVECONTROLLER_H
