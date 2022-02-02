#ifndef _COMMANDCREATOR_H
#define _COMMANDCREATOR_H

#include "Command.h"
#include "NotepadForm.h"

class CommandCreator
{
public:
	CommandCreator(NotepadForm* notepadForm = 0);//디폴트생성자
	Command* Create(UINT nId);//팩토리 메소드 패턴
	~CommandCreator();//소멸자
public://notepadForm과 연관화
	NotepadForm* notepadForm;
};

#endif // !_COMMANDCREATOR_H

