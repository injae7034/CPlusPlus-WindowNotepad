#ifndef _MACROCOMMAND_H
#define _MACROCOMMAND_H

#include "Command.h"
#include "Stack.h"
class NotepadForm;

class MacroCommand : public Command
{
public:
	MacroCommand(NotepadForm* notepadForm = 0);//생성자
	virtual ~MacroCommand();//소멸자
	Long Push(Command* command);
	Command* Pop();
	virtual void Execute();
	virtual void UnExecute();
	//인라인 함수 선언
	Long GetCapacity() const;
	Long GetLength() const;
private:
	Stack<Command*> commands;
	Long capacity;
	Long length;
};

//인라인함수 정의
inline Long MacroCommand::GetCapacity() const
{
	return this->capacity;
}

inline Long MacroCommand::GetLength() const
{
	return this->length;
}

#endif // !_MACROCOMMAND_H