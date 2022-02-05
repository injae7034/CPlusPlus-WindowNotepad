#include "MacroCommand.h"
#include "NotepadForm.h"
#include "CommandHistory.h"

//����Ʈ ������ ����
MacroCommand::MacroCommand(NotepadForm* notepadForm)
:commands()
{
	this->notepadForm = notepadForm;
	this->capacity = 256;
	this->length = 0;
}

//CommandList�� �߰��ϱ�
Long MacroCommand::Push(Command* command)
{
	//1. ��뷮�� �Ҵ緮���� ũ�ų� ������
	if (this->length >= this->capacity)
	{
		//1.1 �Ҵ緮�� ������Ų��.
		this->capacity++;
	}
	//2. ������ �迭 ��� ������ �߰��Ѵ�.
	Long index = this->commands.Push(command);
	//3. ��뷮�� ������Ų��.
	this->length++;
	//4. ��ġ�� ��ȯ�Ѵ�.
	return index;
}

//CommandList���� ����
Command* MacroCommand::Pop()
{
	//������ �迭 ��Ҹ� ������.
	Command* command = this->commands.Pop();
	//�Ҵ緮�� ���ҽ�Ų��.
	this->capacity--;
	//��뷮�� ���ҽ�Ų��.
	this->length--;
	//������ �迭 ��Ҹ� ��ȯ�Ѵ�.
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

//�Ҹ��� ����
MacroCommand::~MacroCommand()
{

}