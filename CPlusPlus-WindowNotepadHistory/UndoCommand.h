#ifndef _UNDOCOMMAND_H
#define _UNDOCOMMAND_H

#include "Command.h"

class NotepadForm;

class UndoCommand :public Command
{
public:
	UndoCommand(NotepadForm* notepadForm = 0);//����Ʈ������
	virtual void Execute();
	virtual ~UndoCommand();//�Ҹ���
};

#endif // !_UNDOCOMMAND_H