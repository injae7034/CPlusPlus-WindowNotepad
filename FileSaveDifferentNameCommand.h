#ifndef _FILESAVEDIFFERENTNAMECOMMAND_H
#define _FILESAVEDIFFERENTNAMECOMMAND_H

#include "Command.h"

class NotepadForm;

class FileSaveDifferentNameCommand : public Command
{
public:
	FileSaveDifferentNameCommand(NotepadForm* notepadForm = 0);
	void Execute();
	~FileSaveDifferentNameCommand();//¼Ò¸êÀÚ
};

#endif // !_FILESAVEDIFFERENTNAMECOMMAND_H
