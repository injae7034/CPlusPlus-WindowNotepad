#ifndef _SCROLLCONTROLLER_H
#define _SCROLLCONTROLLER_H

#include "Observer.h"

class NotepadForm;
class Scroll;

class ScrollController:public Observer
{
public:
	NotepadForm* notepadForm;
	Scroll* scroll[2];//Scroll포인터 배열
public:
	ScrollController(NotepadForm* notepadForm = 0);//디폴트생성자
	virtual void Update();
	~ScrollController();//소멸자
};

#endif // !_SCROLLCONTROLLER_H
