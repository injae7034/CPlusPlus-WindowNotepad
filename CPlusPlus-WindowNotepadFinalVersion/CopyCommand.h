#ifndef _COPYCOMMAND_H
#define _COPYCOMMAND_H

#include "Command.h"

class NotepadForm;

class CopyCommand :public Command
{
public:
	CopyCommand(NotepadForm* notepadForm = 0);
	virtual void Execute();
	virtual ~CopyCommand();//¼Ò¸êÀÚ
};

#endif // !_COPYCOMMAND_H
