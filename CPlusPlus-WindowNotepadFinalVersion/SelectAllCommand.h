#ifndef _SELECTALLCOMMAND_H
#define _SELECTALLCOMMAND_H

#include "Command.h"

class NotepadForm;

class SelectAllCommand :public Command
{
public:
	SelectAllCommand(NotepadForm* notepadForm = 0);
	void Execute();
	~SelectAllCommand();//¼Ò¸êÀÚ
};

#endif // !_SELECTALLCOMMAND_H