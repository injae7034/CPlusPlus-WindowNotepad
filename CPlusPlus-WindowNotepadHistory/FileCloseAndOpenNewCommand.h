#ifndef _FILECLOSEANDOPENNEWCOMMAND_H
#define _FILECLOSEANDOPENNEWCOMMAND_H

#include "Command.h"

class NotepadForm;

class FileCloseAndOpenNewCommand : public Command
{
public:
	FileCloseAndOpenNewCommand(NotepadForm* notepadForm = 0);
	void Execute();
	~FileCloseAndOpenNewCommand();//¼Ò¸êÀÚ
};

#endif // !_FILECLOSEANDOPENNEWCOMMAND_H

