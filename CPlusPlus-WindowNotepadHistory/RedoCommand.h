#ifndef _REDOCOMMAND_H
#define _REDOCOMMAND_H

#include "Command.h"

class NotepadForm;

class RedoCommand :public Command
{
public:
	RedoCommand(NotepadForm* notepadForm = 0);//디폴트생성자
	void Execute();
	~RedoCommand();//소멸자
};

#endif // !_REDOCOMMAND_H#pragma once
