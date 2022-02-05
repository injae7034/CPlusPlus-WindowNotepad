#include "CommandHistory.h"
#include "NotepadForm.h"
#include "Command.h"
#include "OnCharCommand.h"
#include "Note.h"

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
	//1. UndoList�� ������� ������
	if (this->notepadForm->commandHistory->IsUndoListEmpty() == false)
	{
		//1.1 undoList���� ������ �迭 ���(command)�� ������.
		Command* command = this->PopUndoList();
		//1.2 ���� command�� redoList�� �������� �߰��Ѵ�.
		Long index = this->PushRedoList(command);
		//1.3 ���� command�� Unexecute�Ѵ�.(�������)
		command->Unexecute();
		bool isStop = true;
		//1.4 command�� OnCharCommand�̸�
		if (dynamic_cast<OnCharCommand*>(command))
		{
			//1.4.1 ���� command�� ���Ѵ�.(command�� ���� ������ previousCommand�� �������̵�)
			Command* previousCommand = this->undoList.Pop();
			if (previousCommand != 0)
			{
				isStop = false;
			}
			//1.4.2 previousCommand�� null�� �ƴϰ�  previousCommand�� OnCharCommand�̸�
			if (isStop == false && dynamic_cast<OnCharCommand*>(previousCommand))
			{
				//1.4.2.1 previousCommand�� undoMacroEnd�� �ƴѵ��� �ݺ��Ѵ�.
				while (isStop == false && dynamic_cast<OnCharCommand*>(previousCommand)
					->IsUndoMacroEnd() == false)
				{
					//1.4.2.1.1 undoList�� �Ҵ緮�� ���ҽ�Ų��.
					this->undoListCapacity--;
					//1.4.2.1.2 undoList�� ��뷮�� ���ҽ�Ų��.
					this->undoListLength--;
					//1.4.2.1.3 pushRedoList�� ������ �迭 ��� ������ �߰��Ѵ�.
					Long index = this->PushRedoList(previousCommand);
					//1.4.2.1.4 ���� previousCommand�� Unexecute�Ѵ�.(�������)
					previousCommand->Unexecute();
					//1.4.2.1.5 undoList���� ������ �迭 ��Ҹ� ������.
					previousCommand = this->undoList.Pop();
					if (previousCommand == 0)
					{
						isStop = true;
					}
				}
				//1.4.2.2 previousCommand�� undoMacroEnd�̸�
				if (isStop == false)
				{
					//1.4.2.2.1 ���� previousCommand�� undoList�� ������ �迭 ��ҿ� �ٽ� �߰��Ѵ�.
					this->undoList.Push(previousCommand);
				}

			}
			//1.4.3 previousCommand�� null�� �ƴϰ�  previousCommand�� OnCharCommand�� �ƴϸ�
			else if (isStop == false && !dynamic_cast<OnCharCommand*>(previousCommand))
			{
				//1.4.3.1 undoList�� �ٽ� �־��ش�.
				this->undoList.Push(previousCommand);
			}
		}
	}
}

//�ٽý��� ����
void CommandHistory::Redo()
{
	//1. RedoList�� ������� ������
	if (this->notepadForm->commandHistory->IsRedoListEmpty() == false)
	{
		//1.1 redoList�� ������ �迭 ��Ҹ� ������.
		Command* command = this->PopRedoList();
		//1.2 undoList�� ��뷮�� �Ҵ緮���� ũ�ų� ������
		if (this->undoListLength >= this->undoListCapacity)
		{
			//1.2.1 undoList�� �Ҵ緮�� ������Ų��.
			this->undoListCapacity++;
		}
		//1.3 undoList�� ������ �迭 ��� ������ �߰��Ѵ�.
		Long index = this->undoList.Push(command);
		//1.4 undoList�� ��뷮�� ������Ų��.
		this->undoListLength++;
		//1.5 ���� command�� execute�Ѵ�.
		command->Execute();
		bool isStop = true;
		//1.7 command�� OnCharCommand�̸�
		if (dynamic_cast<OnCharCommand*>(command))
		{
			//1.7.1 previousCommand�� ���Ѵ�.(command�� ���� ������ previousCommand�� �������̵�)
			Command* previousCommand = this->redoList.Pop();
			if (previousCommand != 0)
			{
				isStop = false;
			}
			//1.7.2 previousCommand�� null�� �ƴϰ�, OnCharCommand�̸�
			if (isStop == false && dynamic_cast<OnCharCommand*>(previousCommand))
			{
				//1.7.2.1 previousCommand�� RedoMacroEnd�ε��� �ݺ��Ѵ�.
				while (isStop == false && dynamic_cast<OnCharCommand*>(previousCommand)
					->IsRedoMacroEnd() == false)
				{
					//1.7.2.1.1 redoList�� �Ҵ緮�� ���ҽ�Ų��.
					this->redoListCapacity--;
					//1.7.2.1.2 redoList�� ��뷮�� ���ҽ�Ų��.
					this->redoListLength--;
					//1.7.2.1.3 undoList�� ��뷮�� �Ҵ緮���� ũ�ų� ������
					if (this->undoListLength >= this->undoListCapacity)
					{
						//1.7.2.1.3.1 undoList�� �Ҵ緮�� ������Ų��.
						this->undoListCapacity++;
					}
					//1.7.2.1.4 undoList�� ������ �迭 ��� ������ �߰��Ѵ�.
					this->undoList.Push(previousCommand);
					//1.7.2.1.5 undoList�� ��뷮�� ������Ų��.
					this->undoListLength++;
					//1.7.2.1.6 previousCommand�� Execute�Ѵ�.
					previousCommand->Execute();
					//1.7.2.1.7 redoList���� ������ �迭 ��Ҹ� ������.
					previousCommand = this->redoList.Pop();
					if (previousCommand == 0)
					{
						isStop = true;
					}
				}
				//1.7.2.2 previousCommand�� RedoMacroEnd�̸�
				if (isStop == false)
				{
					//1.7.2.2.1 ���´� previousCommand�� redoList�� �ٽ� �־��ش�.
					this->redoList.Push(previousCommand);
				}
			}
			//1.7.3 previousCommand�� null�� �ƴϰ� OnCharCommand�� �ƴϸ�
			else if (isStop == false &&
				!dynamic_cast<OnCharCommand*>(previousCommand))
			{
				//1.7.3.1 ���� command�� redoList�� �ٽ� �־��ش�.
				this->redoList.Push(previousCommand);
			}
		}
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

//UndoList�� ���� ������ �迭 ��� ������ �߰��ϱ�(OnCharCommand�� ����� �� OnCharCommand���� �߰���)
Long CommandHistory::PushUndoList(Command* command)
{
	Command* lastCommand = 0;
	Long lastCommandLetterIndex = 0;
	//1. �Ű������� �Է¹��� command�� OnCharCommand�̸�
	if (dynamic_cast<OnCharCommand*>(command))
	{
		//1.1 ���� undoList���� ������ ���� ���Ѵ�.
		lastCommand = this->undoList.GetTop();
		//1.2 undoList���� lastCommand�� ������(undoList�� ����� command�� �� ���� ������)
		if (lastCommand != 0)
		{
			//1.2.1 lastCommand�� OnCharCommand�̸�
			if (dynamic_cast<OnCharCommand*>(lastCommand))
			{
				//1.2.1.1 lastCommand�� ���๮���̸�
				if (dynamic_cast<OnCharCommand*>(lastCommand)->GetNChar() == '\n'
					|| dynamic_cast<OnCharCommand*>(lastCommand)->GetNChar() == '\r')
				{
					//1.2.1.1.1 lastCommand�� undoMacro����� ������ �������� ǥ���Ѵ�.
					dynamic_cast<OnCharCommand*>(lastCommand)->SetUndoMacroEnd();
				}
				//1.2.1.2 lastCommand�� command�� ���� ��ġ�� ������
				else if (dynamic_cast<OnCharCommand*>(lastCommand)->GetRowIndex()
					== dynamic_cast<OnCharCommand*>(command)->GetRowIndex())
				{
					//1.2.1.2.1 lastCommand�� command�� ���� ��ġ�� ���� �� ĭ ���̰� �ȳ���
					lastCommandLetterIndex = dynamic_cast<OnCharCommand*>(lastCommand)
						->GetLetterIndex() + 1;
					if (lastCommandLetterIndex !=
						dynamic_cast<OnCharCommand*>(command)->GetLetterIndex())
					{
						//1.2.1.2.1.1 lastCommand�� undoMacro����� ������ �������� ǥ���Ѵ�.
						dynamic_cast<OnCharCommand*>(lastCommand)->SetUndoMacroEnd();
					}
				}
				//1.2.1.3 lastCommand�� command�� ���� ��ġ�� ���� �ٸ���
				else if(dynamic_cast<OnCharCommand*>(lastCommand)->GetRowIndex()
					!= dynamic_cast<OnCharCommand*>(command)->GetRowIndex())
				{
					//1.2.1.3.1 lastCommand�� undoMacro����� ������ �������� ǥ���Ѵ�.
					dynamic_cast<OnCharCommand*>(lastCommand)->SetUndoMacroEnd();
				}
			}
		}
	}
	//2. undoList�� ��뷮�� �Ҵ緮���� ũ�ų� ������
	if (this->undoListLength >= this->undoListCapacity)
	{
		//2.1 undoList�� �Ҵ緮�� ������Ų��.
		this->undoListCapacity++;
	}
	//3. undoList�� ������ �迭 ��� ������ �߰��Ѵ�.
	Long index = this->undoList.Push(command);
	//4. undoList�� ��뷮�� ������Ų��.
	this->undoListLength++;
	//5. undoList�� �߰��� �迭����� ��ġ�� ��ȯ�Ѵ�.
	return index;
}

//RedoList�� ���� ������ �迭 ��� ������ �߰��ϱ�(Undo(�������)�� �� ��, OnCharCommand���� �߰���)
Long CommandHistory::PushRedoList(Command* command)
{
	Command* lastCommand = 0;
	Long lastCommandLetterIndex = 0;
	//1. �Ű������� �Է¹��� command�� OnCharCommand�̸�
	if (dynamic_cast<OnCharCommand*>(command))
	{
		//1.1 �Ű������� �Է¹��� command�� ���๮���̸�
		if (dynamic_cast<OnCharCommand*>(command)->GetNChar() == '\n'
			|| dynamic_cast<OnCharCommand*>(command)->GetNChar() == '\r')
		{
			//1.1.1 command�� redoMacro����� ������ �������� ǥ���Ѵ�.
			dynamic_cast<OnCharCommand*>(command)->SetRedoMacroEnd();
		}
		//1.2 ���� redoList���� ������ �迭��Ҹ� ���Ѵ�.
		lastCommand = this->redoList.GetTop();
		//1.3 redoList�� ������ �迭��Ұ� ������(redoList�� ����� command�� �� ���� ������)
		if (lastCommand != 0)
		{
			//1.3.1 �Ű������� �Է¹��� command�� undoMacro����� ������ �����̸�
			if (dynamic_cast<OnCharCommand*>(command)->IsUndoMacroEnd() == true)
			{
				//1.3.1.1  redoList���� ������ �迭���(lastCommand)��
				//redoMacro����� ������ �������� ǥ���Ѵ�.
				dynamic_cast<OnCharCommand*>(lastCommand)->SetRedoMacroEnd();
			}
			//1.3.2 lastCommand�� command�� ���� ��ġ�� ������
			else if (dynamic_cast<OnCharCommand*>(lastCommand)->GetRowIndex()
				== dynamic_cast<OnCharCommand*>(command)->GetRowIndex())
			{
				//1.3.2.1 lastCommand�� command�� ���� ��ġ�� ���� �� ĭ ���̰� �ȳ���
				lastCommandLetterIndex = dynamic_cast<OnCharCommand*>(lastCommand)
					->GetLetterIndex() - 1;
				if (lastCommandLetterIndex !=
					dynamic_cast<OnCharCommand*>(command)->GetLetterIndex())
				{
					//1.3.2.1.1 �Ű������� �Է¹��� command�� redoMacro����� ������ �������� ǥ���Ѵ�.
					dynamic_cast<OnCharCommand*>(command)->SetRedoMacroEnd();
				}
			}
			//1.3.2.1 lastCommand�� command�� ���� ��ġ�� ���� �ٸ���
			else if (dynamic_cast<OnCharCommand*>(lastCommand)->GetRowIndex()
				!= dynamic_cast<OnCharCommand*>(command)->GetRowIndex())
			{
				//1.3.2.1.1  �Ű������� �Է¹��� command�� redoMacro����� ������ �������� ǥ���Ѵ�.
				dynamic_cast<OnCharCommand*>(command)->SetRedoMacroEnd();
			}
			
		}
	}
	//2. redoList�� ��뷮�� �Ҵ緮���� ũ�ų� ������
	if (this->redoListLength >= this->redoListCapacity)
	{
		//2.1 redoList�� �Ҵ緮�� ������Ų��.
		this->redoListCapacity++;
	}
	//3. redoList�� ������ �迭 ��� ������ �Ű������� �Է¹��� command�� �߰��Ѵ�.
	Long index = this->redoList.Push(command);
	//4. redoList�� ��뷮�� ������Ų��.
	this->redoListLength++;
	//5. redoList�� �߰��� ������ �迭����� ��ġ�� ��ȯ�Ѵ�.
	return index;
}

//UndoList�� ���� ������ �迭 ��Ҹ� ������
Command* CommandHistory::PopUndoList()
{
	//1. ������ �迭 ��Ҹ� ������.
	Command* command = this->undoList.Pop();
	//2. �Ҵ緮�� ���ҽ�Ų��.
	this->undoListCapacity--;
	//3. ��뷮�� ���ҽ�Ų��.
	this->undoListLength--;
	//4. ���� ������ �迭 ��Ҹ� ��ȯ�Ѵ�.
	return command;
}

//RedoList�� ���� ������ �迭 ��Ҹ� ������
Command* CommandHistory::PopRedoList()
{
	//1. ������ �迭 ��Ҹ� ������.
	Command* command = this->redoList.Pop();
	//2. �Ҵ緮�� ���ҽ�Ų��.
	this->redoListCapacity--;
	//3. ��뷮�� ���ҽ�Ų��.
	this->redoListLength--;
	//4. ������ �迭 ��Ҹ� ��ȯ�Ѵ�.
	return command;
}

//RedoList �ʱ�ȭ��Ű��(OnCharCommand�� ó�� ����� �� redoList�� ������ �� �ʱ�ȭ�ȴ�.)
void CommandHistory::MakeRedoListEmpty()
{
	Command* redoCommand = 0;
	while (this->notepadForm->commandHistory->IsRedoListEmpty() == false)
	{
		redoCommand = this->notepadForm->commandHistory->PopRedoList();
		if (redoCommand != 0)
		{
			delete redoCommand;
			redoCommand = 0;
		}
	}
}

//�Ҹ���
CommandHistory::~CommandHistory()
{
	Command* command = 0;
	//1. UndoList�� �Ҵ��������ش�.
	while (this->undoList.IsEmpty() == false)
	{
		//1.1 undoList�� ����� �ּҰ� ����Ű�� ���� �Ҵ�� ������� �Ҵ��������ش�.
		command = this->undoList.Pop();
		if (command != 0)
		{
			delete command;
			command = 0;
		}
		//1.2 �Ҵ緮�� ���ҽ�Ų��.
		this->undoListCapacity--;
		//1.3 ��뷮�� ���ҽ�Ų��.
		this->undoListLength--;
	}
	//2. RedoList�� �Ҵ��������ش�.
	while (this->redoList.IsEmpty() == false)
	{
		//2.1 redoList�� ����� �ּҰ� ����Ű�� ���� �Ҵ�� ������� �Ҵ��������ش�.
		command = this->redoList.Pop();
		if (command != 0)
		{
			delete command;
			command = 0;
		}
		//2.2 �Ҵ緮�� ���ҽ�Ų��.
		this->redoListCapacity--;
		//2.3 ��뷮�� ���ҽ�Ų��.
		this->redoListLength--;
	}
}