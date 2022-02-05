#include "MacroCommand.h"
#include "NotepadForm.h"
#include "CommandHistory.h"

//디폴트 생성자 정의
MacroCommand::MacroCommand(NotepadForm* notepadForm)
:commands()
{
	this->notepadForm = notepadForm;
	this->capacity = 256;
	this->length = 0;
}

//CommandList에 추가하기
Long MacroCommand::Push(Command* command)
{
	//1. 사용량이 할당량보다 크거나 같으면
	if (this->length >= this->capacity)
	{
		//1.1 할당량을 증가시킨다.
		this->capacity++;
	}
	//2. 마지막 배열 요소 다음에 추가한다.
	Long index = this->commands.Push(command);
	//3. 사용량을 증가시킨다.
	this->length++;
	//4. 위치를 반환한다.
	return index;
}

//CommandList에서 빼기
Command* MacroCommand::Pop()
{
	//마지막 배열 요소를 꺼낸다.
	Command* command = this->commands.Pop();
	//할당량을 감소시킨다.
	this->capacity--;
	//사용량을 감소시킨다.
	this->length--;
	//마지막 배열 요소를 반환한다.
	return command;
}
//Execute
void MacroCommand::Execute()
{
	Long i = 0;
	Command* command = 0;
	while (i < this->length)
	{
		command = this->commands.Pop();
		this->notepadForm->commandHistory->PushUndoList(command);
		command->Execute();
		this->length--;
	}
}
//Unexecute
void MacroCommand::UnExecute()
{
	Long i = 0;
	Command* command = 0;
	while (i < this->length)
	{
		command = this->commands.Pop();
		this->notepadForm->commandHistory->PushRedoList(command);
		command->Unexecute();
		this->length--;
	}
}

//소멸자 정의
MacroCommand::~MacroCommand()
{

}