#ifndef _PAGEMOVECONTROLLER_H
#define _PAGEMOVECONTROLLER_H

#include "Observer.h"

class NotepadForm;

class PageMoveController :public Observer
{
public:
	NotepadForm* notepadForm;
public:
	PageMoveController(NotepadForm* notepadForm = 0);//����Ʈ������
	virtual void Update();
	~PageMoveController();//�Ҹ���
};

#endif // !_PAGEMOVECONTROLLER_H
