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
	CaretManager(NotepadForm* notepadForm = 0);//����Ʈ������
	void Update();
	~CaretManager();//�Ҹ���
};

#endif // !_CARETMANAGER_H

