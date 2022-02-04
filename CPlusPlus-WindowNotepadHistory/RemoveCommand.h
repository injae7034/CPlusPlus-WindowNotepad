#ifndef _REMOVECOMMAND_H
#define _REMOVECOMMAND_H

#include "Command.h"

class NotepadForm;

class RemoveCommand :public Command
{
public:
	RemoveCommand(NotepadForm* notepadForm = 0);
	void Execute();
	~RemoveCommand();//¼Ò¸êÀÚ
};

#endif // !_REMOVECOMMAND_H