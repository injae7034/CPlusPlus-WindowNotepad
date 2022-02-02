#ifndef _REPLACECOMMAND_H
#define _REPLACECOMMAND_H

#include "Command.h"

class NotepadForm;

class ReplaceCommand : public Command
{
public:
	ReplaceCommand(NotepadForm* notepadForm = 0);
	void Execute();
	~ReplaceCommand();//¼Ò¸êÀÚ
};

#endif // !_REPLACECOMMAND_H