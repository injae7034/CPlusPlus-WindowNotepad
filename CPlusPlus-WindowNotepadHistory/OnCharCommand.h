#ifndef _ONCHARCOMMAND_H
#define _ONCHARCOMMAND_H

#include "Command.h"

class NotepadForm;

class OnCharCommand :public Command
{
public:
	OnCharCommand(NotepadForm* notepadForm = 0);//����Ʈ������
	virtual void Execute();
	virtual void Unexecute();
	virtual ~OnCharCommand();//�Ҹ���
};

#endif // !_ONCHARCOMMAND_H