#ifndef _ONCHARCOMMAND_H
#define _ONCHARCOMMAND_H

#include "Command.h"

class NotepadForm;

class OnCharCommand :public Command
{
public:
	OnCharCommand(NotepadForm* notepadForm = 0);//디폴트생성자
	virtual void Execute();
	virtual void Unexecute();
	virtual ~OnCharCommand();//소멸자
};

#endif // !_ONCHARCOMMAND_H