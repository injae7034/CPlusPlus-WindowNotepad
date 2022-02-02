#ifndef _SCROLLCONTROLLER_H
#define _SCROLLCONTROLLER_H

#include "Observer.h"

class NotepadForm;
class Scroll;

class ScrollController:public Observer
{
public:
	NotepadForm* notepadForm;
	Scroll* scroll[2];//Scroll������ �迭
public:
	ScrollController(NotepadForm* notepadForm = 0);//����Ʈ������
	virtual void Update();
	~ScrollController();//�Ҹ���
};

#endif // !_SCROLLCONTROLLER_H
