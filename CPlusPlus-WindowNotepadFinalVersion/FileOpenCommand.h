#ifndef _FILEOPENCOMMAND_H
#define _FILEOPENCOMMAND_H

#include "Command.h"

class NotepadForm;

class FileOpenCommand : public Command 
{
public:
	FileOpenCommand(NotepadForm* notepadForm = 0);
	void Execute();
	~FileOpenCommand();//�Ҹ���
};

#endif // !_FILEOPENCOMMAND_H

