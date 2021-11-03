#ifndef _CARETCONTROLLER_H
#define _CARETCONTROLLER_H

#include "Observer.h"

class NotepadForm;
class Caret;

class CaretController :public Observer
{
public:
	NotepadForm* notepadForm;
	Caret* caret;
public:
	CaretController(NotepadForm* notepadForm = 0);//����Ʈ������
	virtual void Update();
	~CaretController();//�Ҹ���
};

#endif // !_CARETCONTROLLER_H

