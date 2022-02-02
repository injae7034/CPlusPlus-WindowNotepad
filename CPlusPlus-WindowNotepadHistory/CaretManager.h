#ifndef _CARETMANAGER_H
#define _CARETMANAGER_H

#include "Observer.h"

class NotepadForm;
class Caret;

class CaretManager :public Observer
{
public:
	NotepadForm* notepadForm;
	Caret* caret;
public:
	CaretManager(NotepadForm* notepadForm = 0);//디폴트생성자
	void Update();
	~CaretManager();//소멸자
};

#endif // !_CARETMANAGER_H

