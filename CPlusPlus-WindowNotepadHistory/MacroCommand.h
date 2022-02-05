#ifndef _MACROCOMMAND_H
#define _MACROCOMMAND_H

#include "Command.h"
#include "Stack.h"
class NotepadForm;

class MacroCommand : public Command
{
public:
	MacroCommand(NotepadForm* notepadForm = 0);//������
	virtual ~MacroCommand();//�Ҹ���
	Long Push(Command* command);
	Command* Pop();
	virtual void Execute();
	virtual void UnExecute();
	//�ζ��� �Լ� ����
	Long GetCapacity() const;
	Long GetLength() const;
private:
	Stack<Command*> commands;
	Long capacity;
	Long length;
};

//�ζ����Լ� ����
inline Long MacroCommand::GetCapacity() const
{
	return this->capacity;
}

inline Long MacroCommand::GetLength() const
{
	return this->length;
}

#endif // !_MACROCOMMAND_H