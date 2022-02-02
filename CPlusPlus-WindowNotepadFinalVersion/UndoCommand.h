#ifndef _UNDOCOMMAND_H
#define _UNDOCOMMAND_H

#include "Command.h"

class NotepadForm;

class UndoCommand :public Command
{
public:
	UndoCommand(NotepadForm* notepadForm = 0);//디폴트생성자
	virtual void Execute();
	virtual ~UndoCommand();//소멸자
};

#endif // !_UNDOCOMMAND_H