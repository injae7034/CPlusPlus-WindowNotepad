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
	CaretController(NotepadForm* notepadForm = 0);//디폴트생성자
	virtual void Update();
	~CaretController();//소멸자
};

#endif // !_CARETCONTROLLER_H

