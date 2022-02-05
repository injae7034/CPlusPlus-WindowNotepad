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
		Command* command = this->PopUndoList();
		
		Long index = this->PushRedoList(command);
		
		//4. ���� command�� Unexecute�Ѵ�.
		command->Unexecute();
		bool isStop = true;
		//5. command�� OnCharCommand�̸�
		if (dynamic_cast<OnCharCommand*>(command))
		{
			//command�� ���� command�� ���Ѵ�.(command�� ���� ������ previousCommand�� �������̵�)
			Command* previousCommand = this->undoList.Pop();
			if (previousCommand != 0)
			{
				isStop = false;
			}
			//5. previousCommand�� OnCharCommand�̸�
			if (isStop == false && dynamic_cast<OnCharCommand*>(previousCommand))
			{
				//5.1 false�ε��� �ݺ��Ѵ�.
				while (isStop == false && dynamic_cast<OnCharCommand*>(previousCommand)
					->IsUndoMacroEnd() == false)
				{
					//�Ҵ緮�� ���ҽ�Ų��.
					this->undoListCapacity--;
					//��뷮�� ���ҽ�Ų��.
					this->undoListLength--;
					
					//2. ������ �迭 ��� ������ �߰��Ѵ�.
					Long index = this->PushRedoList(previousCommand);
				
					previousCommand->Unexecute();
					//4. command�� nextCommand�� �����Ѵ�.
					//command = previousCommand;
					previousCommand = this->undoList.Pop();
					if (previousCommand == 0)
					{
						isStop = true;
					}
				}
				if (isStop == false)
				{
					this->undoList.Push(previousCommand);
				}

			}
			//6. previousCommand�� OnCharCommand�� �ƴϸ�
			else if (isStop == false && !dynamic_cast<OnCharCommand*>(previousCommand))
			{
				//�ٽ� �־��ش�.
				this->undoList.Push(previousCommand);
			}
		}
#if 0
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
#if 0
							//5.1.1.1 nextCommand�� ���� ��ġ�� command�� ���� ��ġ�� ���ؼ�
							//nextCommand�� ������ġ�� command������ġ���� 1������ ������
							nextLetterIndex = dynamic_cast<OnCharCommand*>(nextCommand)
								->GetLetterIndex();
							currentLetterIndex = dynamic_cast<OnCharCommand*>(command)
								->GetLetterIndex();
							compareIndex = currentLetterIndex - 1;
							if (nextLetterIndex == compareIndex)
#endif
							if (dynamic_cast<OnCharCommand*>(nextCommand)
								->IsSameTurn() == true)
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
								if (dynamic_cast<OnCharCommand*>(command)
									->IsSameTurn() == true)
								{
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
									//command = nextCommand;
								}
								//this->undoList.Push(nextCommand);
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
#endif
	}
}

//�ٽý��� ����
void CommandHistory::Redo()
{
	//RedoList�� ������� ������
	if (this->notepadForm->commandHistory->IsRedoListEmpty() == false)
	{
		//������ �迭 ��Ҹ� ������.
		Command* command = this->PopRedoList();
		//2. ��뷮�� �Ҵ緮���� ũ�ų� ������
		if (this->undoListLength >= this->undoListCapacity)
		{
			//1.1 �Ҵ緮�� ������Ų��.
			this->undoListCapacity++;
		}
		//3. ������ �迭 ��� ������ �߰��Ѵ�.
		Long index = this->undoList.Push(command);
		//4. ��뷮�� ������Ų��.
		this->undoListLength++;
		//���� command�� OnCharCommand�̸�
		if (dynamic_cast<OnCharCommand*>(command))
		{
			dynamic_cast<OnCharCommand*>(command)->SetRedone();
		}
		//���� command�� execute�Ѵ�.
		command->Execute();
		bool isStop = true;
		//5. command�� OnCharCommand�̸�
		if (dynamic_cast<OnCharCommand*>(command))
		{
			//command�� ���� command�� ���Ѵ�.(command�� ���� ������ previousCommand�� �������̵�)
			Command* previousCommand = this->redoList.Pop();
			if (previousCommand != 0)
			{
				isStop = false;
			}
			//5. nextCommand�� OnCharCommand�̸�
			if (isStop == false && dynamic_cast<OnCharCommand*>(previousCommand))
			{
				//5.1 false�ε��� �ݺ��Ѵ�.
				while (isStop == false && dynamic_cast<OnCharCommand*>(previousCommand)
					->IsRedoMacroEnd() == false)
				{
					//�Ҵ緮�� ���ҽ�Ų��.
					this->redoListCapacity--;
					//��뷮�� ���ҽ�Ų��.
					this->redoListLength--;
					//2. ��뷮�� �Ҵ緮���� ũ�ų� ������
					if (this->undoListLength >= this->undoListCapacity)
					{
						//1.1 �Ҵ緮�� ������Ų��.
						this->undoListCapacity++;
					}
					//3. ������ �迭 ��� ������ �߰��Ѵ�.
					this->undoList.Push(previousCommand);
					//4. ��뷮�� ������Ų��.
					this->undoListLength++;
					dynamic_cast<OnCharCommand*>(previousCommand)->SetRedone();
					previousCommand->Execute();
					//4. command�� nextCommand�� �����Ѵ�.
					previousCommand = this->redoList.Pop();
					if (previousCommand == 0)
					{
						isStop = true;
					}
				}
				if (isStop == false)
				{
					this->redoList.Push(previousCommand);
				}
			}
			//6. previousCommand�� OnCharCommand�� �ƴϸ�
			else if (isStop == false &&
				!dynamic_cast<OnCharCommand*>(previousCommand))
			{
				//�ٽ� �־��ش�.
				this->redoList.Push(previousCommand);
			}
		}

#if 0
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
#endif
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
Long CommandHistory::PushUndoList(Command* command)
{
	Command* lastCommand = 0;
	Long lastCommandLetterIndex = 0;
	//1. command�� OnCharCommand�̸�
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
				//1.2.1.3 ���� command�� ���๮���̸�
				if (dynamic_cast<OnCharCommand*>(lastCommand)->GetNChar() == '\n'
					|| dynamic_cast<OnCharCommand*>(lastCommand)->GetNChar() == '\r')
				{
					//1.2.1.1.1.1 ���� command�� ���������� ǥ���Ѵ�.
					dynamic_cast<OnCharCommand*>(lastCommand)->SetUndoMacroEnd();
				}
				//1.2.1.1 lastCommand�� command�� ���� ��ġ�� ������
				else if (dynamic_cast<OnCharCommand*>(lastCommand)->GetRowIndex()
					== dynamic_cast<OnCharCommand*>(command)->GetRowIndex())
				{
					//1.2.1.1.1 lastCommand�� command�� ���� ��ġ�� ���Ѵ�.
					lastCommandLetterIndex = dynamic_cast<OnCharCommand*>(lastCommand)
						->GetLetterIndex() + 1;
					if (lastCommandLetterIndex !=
						dynamic_cast<OnCharCommand*>(command)->GetLetterIndex())
					{
						//1.2.1.1.1.1 ���� command�� ���������� ǥ���Ѵ�.
						dynamic_cast<OnCharCommand*>(lastCommand)->SetUndoMacroEnd();
					}
				}
				//1.2.1.2 ���� ��ġ�� ���� �ٸ���
				else if(dynamic_cast<OnCharCommand*>(lastCommand)->GetRowIndex()
					!= dynamic_cast<OnCharCommand*>(command)->GetRowIndex())
				{
					//1.2.1.1.1.1 ���� command�� ���������� ǥ���Ѵ�.
					dynamic_cast<OnCharCommand*>(lastCommand)->SetUndoMacroEnd();
				}
			}
		}
	}
	//2. ��뷮�� �Ҵ緮���� ũ�ų� ������
	if (this->undoListLength >= this->undoListCapacity)
	{
		//1.1 �Ҵ緮�� ������Ų��.
		this->undoListCapacity++;
	}
	//3. ������ �迭 ��� ������ �߰��Ѵ�.
	Long index = this->undoList.Push(command);
	//4. ��뷮�� ������Ų��.
	this->undoListLength++;
	//5. ��ġ�� ��ȯ�Ѵ�.
	return index;
}

//RedoList�� ���� ������ �迭 ��� ������ �߰��ϱ�
Long CommandHistory::PushRedoList(Command* command)
{
	Command* lastCommand = 0;
	Long lastCommandLetterIndex = 0;
	//1. command�� OnCharCommand�̸�
	if (dynamic_cast<OnCharCommand*>(command))
	{
		if (dynamic_cast<OnCharCommand*>(command)->GetNChar() == '\n'
			|| dynamic_cast<OnCharCommand*>(command)->GetNChar() == '\r')
		{
			dynamic_cast<OnCharCommand*>(command)->SetRedoMacroEnd();
		}
		//1.1 ���� redoList���� ������ ���� ���Ѵ�.
		lastCommand = this->redoList.GetTop();
		if (lastCommand != 0)
		{
			//if (dynamic_cast<OnCharCommand*>(lastCommand)->GetNChar() == '\n'
			//	|| dynamic_cast<OnCharCommand*>(lastCommand)->GetNChar() == '\r')
			//{
			//	dynamic_cast<OnCharCommand*>(command)->SetRedoMacroEnd();
			//}
			if (dynamic_cast<OnCharCommand*>(command)->IsUndoMacroEnd() == true)
			{
				dynamic_cast<OnCharCommand*>(lastCommand)->SetRedoMacroEnd();
			}
			//1.2.1.1 lastCommand�� command�� ���� ��ġ�� ������
			else if (dynamic_cast<OnCharCommand*>(lastCommand)->GetRowIndex()
				== dynamic_cast<OnCharCommand*>(command)->GetRowIndex())
			{
				//1.2.1.1.1 lastCommand�� command�� ���� ��ġ�� ���Ѵ�.
				lastCommandLetterIndex = dynamic_cast<OnCharCommand*>(lastCommand)
					->GetLetterIndex() - 1;
				if (lastCommandLetterIndex !=
					dynamic_cast<OnCharCommand*>(command)->GetLetterIndex())
				{
					//1.2.1.1.1.1 ���� command�� ���������� ǥ���Ѵ�.
					dynamic_cast<OnCharCommand*>(command)->SetRedoMacroEnd();
				}
			}
			//1.2.1.2 ���� ��ġ�� ���� �ٸ���
			else if (dynamic_cast<OnCharCommand*>(lastCommand)->GetRowIndex()
				!= dynamic_cast<OnCharCommand*>(command)->GetRowIndex())
			{
				//1.2.1.1.1.1 ���� command�� ���������� ǥ���Ѵ�.
				dynamic_cast<OnCharCommand*>(command)->SetRedoMacroEnd();
			}
			
		}


#if 0
		//1.1 ���� redoList���� ������ ���� ���Ѵ�.
		lastCommand = this->redoList.GetTop();
		//1.2 redoList���� lastCommand�� ������(redoList�� ����� command�� �� ���� ������)
		if (lastCommand != 0)
		{
			//1.2.1 lastCommand�� OnCharCommand�̸�
			if (dynamic_cast<OnCharCommand*>(lastCommand))
			{
				//1.2.1.3 ���� command�� ���๮���̸�
				if (dynamic_cast<OnCharCommand*>(lastCommand)->GetNChar() == '\n'
					|| dynamic_cast<OnCharCommand*>(lastCommand)->GetNChar() == '\r')
				{
					//1.2.1.1.1.1 ���� command�� ���������� ǥ���Ѵ�.
					dynamic_cast<OnCharCommand*>(lastCommand)->SetRedoMacroEnd();
				}
				//1.2.1.1 lastCommand�� command�� ���� ��ġ�� ������
				else if (dynamic_cast<OnCharCommand*>(lastCommand)->GetRowIndex()
					== dynamic_cast<OnCharCommand*>(command)->GetRowIndex())
				{
					//1.2.1.1.1 lastCommand�� command�� ���� ��ġ�� ���Ѵ�.
					lastCommandLetterIndex = dynamic_cast<OnCharCommand*>(lastCommand)
						->GetLetterIndex() - 1;
					if (lastCommandLetterIndex !=
						dynamic_cast<OnCharCommand*>(command)->GetLetterIndex())
					{
						//1.2.1.1.1.1 ���� command�� ���������� ǥ���Ѵ�.
						dynamic_cast<OnCharCommand*>(lastCommand)->SetRedoMacroEnd();
					}
				}
				//1.2.1.2 ���� ��ġ�� ���� �ٸ���
				else if (dynamic_cast<OnCharCommand*>(lastCommand)->GetRowIndex()
					!= dynamic_cast<OnCharCommand*>(command)->GetRowIndex())
				{
					//1.2.1.1.1.1 ���� command�� ���������� ǥ���Ѵ�.
					dynamic_cast<OnCharCommand*>(lastCommand)->SetRedoMacroEnd();
				}
			}
		}
#endif
	}
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
Command* CommandHistory::PopUndoList()
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
Command* CommandHistory::PopRedoList()
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

//RedoList �ʱ�ȭ��Ű��
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