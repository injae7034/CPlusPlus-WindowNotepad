#ifndef _REDOCOMMAND_H
#define _REDOCOMMAND_H

#include "Command.h"

class NotepadForm;

class RedoCommand :public Command
{
public:
	RedoCommand(NotepadForm* notepadForm = 0);//����Ʈ������
	void Execute();
	~RedoCommand();//�Ҹ���
};

#endif // !_REDOCOMMAND_H#pragma once
