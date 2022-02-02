#ifndef _FILEEXITCOMMAND_H
#define _FILEEXITCOMMAND_H

#include "Command.h"

class NotepadForm;

class FileExitCommand : public Command
{
public:
	FileExitCommand(NotepadForm* notepadForm = 0);
	void Execute();
	~FileExitCommand();//¼Ò¸êÀÚ
};

#endif // !_FILEEXITCOMMAND_H

