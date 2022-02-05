#include "CommandHistory.h"
#include "NotepadForm.h"
#include "Command.h"


//����Ʈ ������ ����
CommandHistory::CommandHistory(NotepadForm* notepadForm, Long undoListCapacity,
	Long redoListCapacity)
	:undoList(), redoList()
{
	this->notepadForm = notepadForm;
	this->undoListCapacity = undoListCapacity;
	this->undoListLength = 0;
	this->redoListCapacity = redoListCapacity;
	this->redoListLength = 0;
}

//������� ����
void CommandHistory::Undo()
{
	//UndoList�� ������� ������
	if (this->notepadForm->commandHistory->IsUndoListEmpty() == false)
	{
		//������ �迭 ��Ҹ� ������.
		Command* command = this->undoList.Pop();
		//�Ҵ緮�� ���ҽ�Ų��.
		this->undoListCapacity--;
		//��뷮�� ���ҽ�Ų��.
		this->undoListLength--;
		//RedoList�� ���� �迭��Ҹ� �߰��Ѵ�.
		//1. ��뷮�� �Ҵ緮���� ũ�ų� ������
		if (this->redoListLength >= this->redoListCapacity)
		{
			//1.1 �Ҵ緮�� ������Ų��.
			this->redoListCapacity++;
		}
		//2. ������ �迭 ��� ������ �߰��Ѵ�.
		Long index = this->redoList.Push(command);
		//3. ��뷮�� ������Ų��.
		this->redoListLength++;
		//���� command�� Unexecute�Ѵ�.
		command->Unexecute();
	}
}

//�ٽý��� ����
void CommandHistory::Redo()
{
	//RedoList�� ������� ������
	if (this->notepadForm->commandHistory->IsRedoListEmpty() == false)
	{
		//������ �迭 ��Ҹ� ������.
		Command* command = this->redoList.Pop();
		//�Ҵ緮�� ���ҽ�Ų��.
		this->redoListCapacity--;
		//��뷮�� ���ҽ�Ų��.
		this->redoListLength--;
		//UNdoList�� ���� �迭��Ҹ� �߰��Ѵ�.
		//1. ��뷮�� �Ҵ緮���� ũ�ų� ������
		if (this->undoListLength >= this->undoListCapacity)
		{
			//1.1 �Ҵ緮�� ������Ų��.
			this->undoListCapacity++;
		}
		//2. ������ �迭 ��� ������ �߰��Ѵ�.
		Long index = this->undoList.Push(command);
		//3. ��뷮�� ������Ų��.
		this->undoListLength++;
		//���� command�� Unexecute�Ѵ�.
		command->Execute();
	}
}

//UndoList�� ���� ������ �迭 ��� ���ϱ�
Command* CommandHistory::GetUndoListTop()
{
	return this->undoList.GetTop();
}

//RedoList�� ���� ������ �迭 ��� ���ϱ�
Command* CommandHistory::GetRedoListTop()
{
	return this->redoList.GetTop();
}

//UndoList�� ���� ������ �迭 ��� ������ �߰��ϱ�
Long CommandHistory::AddUndoList(Command* command)
{
	//1. ��뷮�� �Ҵ緮���� ũ�ų� ������
	if (this->undoListLength >= this->undoListCapacity)
	{
		//1.1 �Ҵ緮�� ������Ų��.
		this->undoListCapacity++;
	}
	//2. ������ �迭 ��� ������ �߰��Ѵ�.
	Long index = this->undoList.Push(command);
	//3. ��뷮�� ������Ų��.
	this->undoListLength++;
	//4. ��ġ�� ��ȯ�Ѵ�.
	return index;
}

//RedoList�� ���� ������ �迭 ��� ������ �߰��ϱ�
Long CommandHistory::AddRedoList(Command* command)
{
	//1. ��뷮�� �Ҵ緮���� ũ�ų� ������
	if (this->redoListLength >= this->redoListCapacity)
	{
		//1.1 �Ҵ緮�� ������Ų��.
		this->redoListCapacity++;
	}
	//2. ������ �迭 ��� ������ �߰��Ѵ�.
	Long index = this->redoList.Push(command);
	//3. ��뷮�� ������Ų��.
	this->redoListLength++;
	//4. ��ġ�� ��ȯ�Ѵ�.
	return index;
}

//UndoList�� ���� ������ �迭 ��Ҹ� ������
Command* CommandHistory::RemoveUndoList()
{
	//������ �迭 ��Ҹ� ������.
	Command* command = this->undoList.Pop();
	//�Ҵ緮�� ���ҽ�Ų��.
	this->undoListCapacity--;
	//��뷮�� ���ҽ�Ų��.
	this->undoListLength--;
	//������ �迭 ��Ҹ� ��ȯ�Ѵ�.
	return command;
}

//RedoList�� ���� ������ �迭 ��Ҹ� ������
Command* CommandHistory::RemoveRedoList()
{
	//������ �迭 ��Ҹ� ������.
	Command* command = this->redoList.Pop();
	//�Ҵ緮�� ���ҽ�Ų��.
	this->redoListCapacity--;
	//��뷮�� ���ҽ�Ų��.
	this->redoListLength--;
	//������ �迭 ��Ҹ� ��ȯ�Ѵ�.
	return command;
}

//�Ҹ���
CommandHistory::~CommandHistory()
{
	Command* command = 0;
	//UndoList�� �Ҵ��������ش�.
	while (this->undoList.IsEmpty() == false)
	{
		command = this->undoList.Pop();
		if (command != 0)
		{
			delete command;
			command = 0;
		}
		//�Ҵ緮�� ���ҽ�Ų��.
		this->undoListCapacity--;
		//��뷮�� ���ҽ�Ų��.
		this->undoListLength--;
	}
	//RedoList�� �Ҵ��������ش�.
	while (this->redoList.IsEmpty() == false)
	{
		command = this->redoList.Pop();
		if (command != 0)
		{
			delete command;
			command = 0;
		}
		//�Ҵ緮�� ���ҽ�Ų��.
		this->redoListCapacity--;
		//��뷮�� ���ҽ�Ų��.
		this->redoListLength--;
	}
}