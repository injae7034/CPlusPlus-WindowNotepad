#ifndef _FINDCOMMAND_H
#define _FINDCOMMAND_H

#include "Command.h"

class NotepadForm;

class FindCommand : public Command
{
public:
	FindCommand(NotepadForm* notepadForm = 0);
	void Execute();
	~FindCommand();//¼Ò¸êÀÚ
};

#endif // !_FINDCOMMAND_H