#ifndef _PASTECOMMAND_H
#define _PASTECOMMAND_H

#include "Command.h"

class NotepadForm;

class PasteCommand :public Command
{
public:
	PasteCommand(NotepadForm* notepadForm = 0);
	void Execute();
	~PasteCommand();//�Ҹ���
};

#endif // !_PASTECOMMAND_H