#ifndef _ROWAUTOCHANGECOMMAND_H
#define _ROWAUTOCHANGECOMMAND_H

#include "Command.h"

class NotepadForm;

class RowAutoChangeCommand: public Command
{
public:
	RowAutoChangeCommand(NotepadForm* notepadForm = 0);//������
	void Execute();
	~RowAutoChangeCommand();//�Ҹ���
};

#endif // !_ROWAUTOCHANGECOMMAND_H
