#ifndef _FILESETUPCOMMAND_H
#define _FILESETUPCOMMAND_H

#include "Command.h"

class NotepadForm;

class FileSetupCommand : public Command
{
public:
	FileSetupCommand(NotepadForm* notepadForm = 0);
	void Execute();
	~FileSetupCommand();//�Ҹ���
};

#endif // !_FILESETUPCOMMAND_H


