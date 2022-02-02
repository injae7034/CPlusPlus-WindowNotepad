#ifndef _ROWAUTOCHANGECOMMAND_H
#define _ROWAUTOCHANGECOMMAND_H

#include "Command.h"

class NotepadForm;

class RowAutoChangeCommand: public Command
{
public:
	RowAutoChangeCommand(NotepadForm* notepadForm = 0);//持失切
	void Execute();
	~RowAutoChangeCommand();//社瑚切
};

#endif // !_ROWAUTOCHANGECOMMAND_H
