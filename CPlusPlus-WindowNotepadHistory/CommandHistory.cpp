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
		//4. ���� command�� Unexecute�Ѵ�.
		command->Unexecute();
		//5. command�� OnCharCommand�̸�
		if (dynamic_cast<OnCharCommand*>(command))
		{
			bool areCharactersInARow = false;

			//5.1 OnCharCommand�� nChar�� ���๮�ڰ� �ƴϸ�
			if (dynamic_cast<OnCharCommand*>(command)->GetNChar() != '\n'
				&& dynamic_cast<OnCharCommand*>(command)->GetNChar() != '\r')
			{
				areCharactersInARow = true;
			}

			//5.2 true�ε��� �ݺ��Ѵ�.
			Command* nextCommand = 0;
			Long currentRowIndex = 0;
			Long currentLetterIndex = 0;
			Long nextRowIndex = 0;
			Long nextLetterIndex = 0;
			Long compareIndex = 0;
			while (areCharactersInARow == true && 
				this->notepadForm->commandHistory->IsUndoListEmpty() == false)
			{
				//5.2.1 undoList���� ������ Command�� ������.
				nextCommand = this->undoList.Pop();
				//5. command�� OnCharCommand�̸�
				if (dynamic_cast<OnCharCommand*>(nextCommand))
				{
					//5.1 OnCharCommand�� nChar�� ���๮�ڰ� �ƴϸ�
					if (dynamic_cast<OnCharCommand*>(nextCommand)->GetNChar() != '\n'
						&& dynamic_cast<OnCharCommand*>(nextCommand)->GetNChar() != '\r')
					{
						//5.1.1 nextCommand�� ���� ��ġ�� command�� ���� ��ġ�� ���ؼ� ������
						nextRowIndex = dynamic_cast<OnCharCommand*>(nextCommand)
							->GetRowIndex();
						currentRowIndex = dynamic_cast<OnCharCommand*>(command)->GetRowIndex();
						if (nextRowIndex == currentRowIndex)
						{
							//5.1.1.1 nextCommand�� ���� ��ġ�� command�� ���� ��ġ�� ���ؼ�
							//nextCommand�� ������ġ�� command������ġ���� 1������ ������
							nextLetterIndex = dynamic_cast<OnCharCommand*>(nextCommand)
								->GetLetterIndex();
							currentLetterIndex = dynamic_cast<OnCharCommand*>(command)
								->GetLetterIndex();
							compareIndex = currentLetterIndex - 1;
							if (nextLetterIndex == compareIndex)
							{
								areCharactersInARow = true;
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
								Long index = this->redoList.Push(nextCommand);
								//3. ��뷮�� ������Ų��.
								this->redoListLength++;
								nextCommand->Unexecute();
								//4. command�� nextCommand�� �����Ѵ�.
								command = nextCommand;
							}
							// command�� ���� ��ġ�� ��ũ��
							else
							{
								this->undoList.Push(nextCommand);
								areCharactersInARow = false;
							}
						}
						//5.1.2 ���� ��ġ�� ���� �ٸ���
						else
						{
							this->undoList.Push(nextCommand);
							areCharactersInARow = false;
						}
					}
					//���๮���̸�
					else
					{
						this->undoList.Push(nextCommand);
						areCharactersInARow = false;
					}
				}
				//OnCharCommand�� �ƴϸ�
				else
				{
					this->undoList.Push(nextCommand);
					areCharactersInARow = false;
				}
			}
		}
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
		//5. command�� OnCharCommand�̸�
		if (dynamic_cast<OnCharCommand*>(command))
		{
			bool areCharactersInARow = false;
			//5.1 OnCharCommand�� nChar�� ���๮�ڰ� �ƴϸ�
			if (dynamic_cast<OnCharCommand*>(command)->GetNChar() != '\n'
				&& dynamic_cast<OnCharCommand*>(command)->GetNChar() != '\r')
			{
				areCharactersInARow = true;
			}
			
			//5.2 true�ε��� �ݺ��Ѵ�.
			Command* nextCommand = 0;
			Long currentRowIndex = 0;
			Long currentLetterIndex = 0;
			Long nextRowIndex = 0;
			Long nextLetterIndex = 0;
			Long compareIndex = 0;
			while (areCharactersInARow == true &&
				this->notepadForm->commandHistory->IsRedoListEmpty() == false)
			{
				//5.2.1 redoList���� ������ Command�� ������.
				nextCommand = this->redoList.Pop();
				//5. command�� OnCharCommand�̸�
				if (dynamic_cast<OnCharCommand*>(nextCommand))
				{
					//5.1 OnCharCommand�� nChar�� ���๮�ڰ� �ƴϸ�
					if (dynamic_cast<OnCharCommand*>(nextCommand)->GetNChar() != '\n'
						&& dynamic_cast<OnCharCommand*>(nextCommand)->GetNChar() != '\r')
					{
						//5.1.1 nextCommand�� ���� ��ġ�� command�� ���� ��ġ�� ���ؼ� ������
						nextRowIndex = dynamic_cast<OnCharCommand*>(nextCommand)
							->GetRowIndex();
						currentRowIndex = dynamic_cast<OnCharCommand*>(command)->GetRowIndex();
						if (nextRowIndex == currentRowIndex)
						{
							//5.1.1.1 nextCommand�� ���� ��ġ�� command�� ���� ��ġ�� ���ؼ�
							//nextCommand�� ������ġ�� command������ġ���� 1���Ѱ��� ������
							nextLetterIndex = dynamic_cast<OnCharCommand*>(nextCommand)
								->GetLetterIndex();
							currentLetterIndex = dynamic_cast<OnCharCommand*>(command)
								->GetLetterIndex();
							compareIndex = currentLetterIndex + 1;
							if (nextLetterIndex == compareIndex)
							{
								areCharactersInARow = true;
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
								Long index = this->undoList.Push(nextCommand);
								//3. ��뷮�� ������Ų��.
								this->undoListLength++;
								//���� command�� Unexecute�Ѵ�.
								nextCommand->Execute();
								//4. command�� nextCommand�� �����Ѵ�.
								command = nextCommand;
							}
							// command�� ���� ��ġ�� ��ũ��
							else
							{
								this->redoList.Push(nextCommand);
								areCharactersInARow = false;
							}
						}
						//5.1.2 ���� ��ġ�� ���� �ٸ���
						else
						{
							this->redoList.Push(nextCommand);
							areCharactersInARow = false;
						}
					}
					//���๮���̸�
					else
					{
						this->redoList.Push(nextCommand);
						areCharactersInARow = false;
					}
				}
				//OnCharCommand�� �ƴϸ�
				else
				{
					this->redoList.Push(nextCommand);
					areCharactersInARow = false;
				}
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