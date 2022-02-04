#ifndef _CUTCOMMAND_H
#define _CUTCOMMAND_H

#include "Command.h"

class NotepadForm;

class CutCommand :public Command
{
public:
	CutCommand(NotepadForm* notepadForm = 0);
	void Execute();
	~CutCommand();//¼Ò¸êÀÚ
};

#endif // !_CUTCOMMAND_H