#ifndef _FILEPRINTINGCOMMAND_H
#define _FILEPRINTINGCOMMAND_H

#include "Command.h"

class NotepadForm;

class FilePrintingCommand : public Command
{
public:
	FilePrintingCommand(NotepadForm* notepadForm = 0);
	void Execute();
	~FilePrintingCommand();//�Ҹ���
};

#endif // !_FILEPRINTINGCOMMAND_H


