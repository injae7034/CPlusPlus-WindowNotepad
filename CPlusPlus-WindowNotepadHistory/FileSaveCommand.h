#ifndef _FILESAVECOMMAND_H
#define _FILESAVECOMMAND_H

#include "Command.h"

class NotepadForm;

class FileSaveCommand : public Command
{
public:
	FileSaveCommand(NotepadForm* notepadForm = 0);
	void Execute();
	~FileSaveCommand();//¼Ò¸êÀÚ
};

#endif // !_FILESAVECOMMAND_H


