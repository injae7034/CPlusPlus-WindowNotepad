#include "CommandHistory.h"
#include "NotepadForm.h"
#include "Command.h"
#include "OnCharCommand.h"
#include "Row.h"
#include "BackSpaceKeyActionCommand.h"
#include "DeleteKeyActionCommand.h"
#include "CtrlBackSpaceKeyActionCommand.h"
#include "CtrlDeleteKeyActionCommand.h"
#include "ShiftCtrlBackSpaceKeyActionCommand.h"
#include "ShiftCtrlDeleteKeyActionCommand.h"
#include "RemoveCommand.h"
#include "CutCommand.h"
#include "OnReplaceButtonClickedCommand.h"
#include "OnReplaceAllButtonClickedCommand.h"

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
	if (this->notepadForm->commandHistory->undoListLength > 0)
	{
		//1.1 undoList���� ������ �迭 ���(command)�� ������.
		Command* command = this->PopUndoList();
		//1.2 ���� command�� redoList�� �������� �߰��Ѵ�.
		this->PushRedoList(command);
		//1.3 ���� command�� Unexecute�Ѵ�.(�������)
		command->Unexecute();
		bool isStop = true;
		//1.4. ���� command�� ���Ѵ�.(command�� ���� ������ previousCommand�� �������̵�)
		Command* previousCommand = this->undoList.Pop();
		if (previousCommand != 0)
		{
			isStop = false;
		}
		//1.5 previousCommand�� null�� �ƴϰ� undoMacroEnd�� �ƴѵ���
		//�׸��� command�� ���ÿ����� ������ �������� �ݺ��Ѵ�.
		while (isStop == false && previousCommand->IsUndoMacroEnd() == false
			&& command->IsSelectedTextsRemoved() == false)
		{
			//1.5.1 undoList�� �Ҵ緮�� ���ҽ�Ų��.
			this->undoListCapacity--;
			//1.5.2 undoList�� ��뷮�� ���ҽ�Ų��.
			this->undoListLength--;
			//1.5.3 pushRedoList�� ������ �迭 ��� ������ �߰��Ѵ�.
			this->PushRedoList(previousCommand);
			//1.5.4 ���� previousCommand�� Unexecute�Ѵ�.(�������)
			previousCommand->Unexecute();
			//1.5.5. UndoList���� ���ο� previousCommand�� ������ ���� �����Ѵ�.
			command = previousCommand;
			//1.5.6 undoList���� ������ �迭 ��Ҹ� ������.
			previousCommand = this->undoList.Pop();
			//1.5.7 undoList�� ���̻� �迭��Ұ� ������
			if (previousCommand == 0)
			{
				//1.5.7.1 ����ٰ� ǥ���Ѵ�.
				isStop = true;
			}
		}
		//1.6 previousCommand�� undoMacroEnd�̸�
		if (isStop == false && previousCommand->IsUndoMacroEnd() == true
			|| command->IsSelectedTextsRemoved() == true)
		{
			//1.6.1 ���� previousCommand�� undoList�� ������ �迭 ��ҿ� �ٽ� �߰��Ѵ�.
			this->undoList.Push(previousCommand);
		}
	}
}

//�ٽý��� ����
void CommandHistory::Redo()
{
	//1. RedoList�� ������� ������
	if (this->notepadForm->commandHistory->redoListLength > 0)
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
		this->undoList.Push(command);
		//1.4 undoList�� ��뷮�� ������Ų��.
		this->undoListLength++;
		//1.5 ���� command�� Execute �Ǳ� ���� �ٽ� �����̶�� ǥ�ø� �Ѵ�.
		command->SetRedone();
		//1.6 ���� command�� execute�Ѵ�.
		command->Execute();
		bool isStop = true;
		//1.7 previousCommand�� ���Ѵ�.(command�� ���� ������ previousCommand�� �������̵�)
		Command* previousCommand = this->redoList.Pop();
		if (previousCommand != 0)
		{
			isStop = false;
		}
		//1.8 previousCommand�� null�� �ƴϰ� RedoMacroEnd�ε��� �ݺ��Ѵ�.
		while (isStop == false && previousCommand->IsRedoMacroEnd() == false)
		{
			//1.8.1 redoList�� �Ҵ緮�� ���ҽ�Ų��.
			this->redoListCapacity--;
			//1.8.2 redoList�� ��뷮�� ���ҽ�Ų��.
			this->redoListLength--;
			//1.8.3 undoList�� ��뷮�� �Ҵ緮���� ũ�ų� ������
			if (this->undoListLength >= this->undoListCapacity)
			{
				//1.8.3.1 undoList�� �Ҵ緮�� ������Ų��.
				this->undoListCapacity++;
			}
			//1.8.4 undoList�� ������ �迭 ��� ������ �߰��Ѵ�.
			this->undoList.Push(previousCommand);
			//1.8.5 undoList�� ��뷮�� ������Ų��.
			this->undoListLength++;
			//1.8.6 previousCommand�� Execute�Ǳ� ���� �ٽ� �����̶�� ǥ�ø� �Ѵ�.
			previousCommand->SetRedone();
			//1.8.7 previousCommand�� Execute�Ѵ�.
			previousCommand->Execute();
			//1.8.8 redoList���� ������ �迭 ��Ҹ� ������.
			previousCommand = this->redoList.Pop();
			if (previousCommand == 0)
			{
				isStop = true;
			}
		}
		//1.9 previousCommand�� RedoMacroEnd�̸�
		if (isStop == false)
		{
			//1.9.1 ���´� previousCommand�� redoList�� �ٽ� �־��ش�.
			this->redoList.Push(previousCommand);
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
Long CommandHistory::PushUndoList(Command* command)
{
	Long lastCommandLetterIndex = 0;
	//1. ���� undoList���� ������ ���� ���Ѵ�.
	Command* lastCommand = this->undoList.GetTop();
	bool isDone = false;
	//2. undoList���� lastCommand�� ������(undoList�� ����� command�� �� ���� ������)
	if (lastCommand != 0)
	{
		//2.1 lastCommand�� �ٽ� ����� Command�̸�
		if (lastCommand->IsRedone() == true)
		{
			//2.1.1 lastCommand�� undoMacro����� ������ �������� ǥ���Ѵ�.
			lastCommand->SetUndoMacroEnd();
			//2.1.2  ǥ�ð� �������� ��Ÿ����.
			isDone = true;
		}
		//2.2 lastCommand�� OnCharCommand�̸�
		else if (dynamic_cast<OnCharCommand*>(lastCommand))
		{
			Glyph* glyph = dynamic_cast<OnCharCommand*>(lastCommand)->GetGlyph();
			//2.2.1 ���๮���̸�
			if (dynamic_cast<Row*>(glyph))
			{
				//2.2.1.1 lastCommand�� undoMacro����� ������ �������� ǥ���Ѵ�.
				lastCommand->SetUndoMacroEnd();
				//2.2.1.2  ǥ�ð� �������� ��Ÿ����.
				isDone = true;
			}
		}
		//2.3 lastCommand�� ����� ���� Command�̰ų� �߶󳻱� �Ǵ� �ٲٱ� command�̸�
		else if (dynamic_cast<BackSpaceKeyActionCommand*>(lastCommand) ||
			dynamic_cast<DeleteKeyActionCommand*>(lastCommand) ||
			dynamic_cast<CtrlBackSpaceKeyActionCommand*>(lastCommand) ||
			dynamic_cast<CtrlDeleteKeyActionCommand*>(lastCommand) ||
			dynamic_cast<ShiftCtrlBackSpaceKeyActionCommand*>(lastCommand) ||
			dynamic_cast<ShiftCtrlDeleteKeyActionCommand*>(lastCommand) ||
			dynamic_cast<RemoveCommand*>(lastCommand) ||
			dynamic_cast<CutCommand*>(lastCommand) ||
			dynamic_cast<OnReplaceButtonClickedCommand*>(lastCommand) ||
			dynamic_cast<OnReplaceAllButtonClickedCommand*>(lastCommand))
		{
			//2.3.1 lastCommand�� undoMacro����� ������ �������� ǥ���Ѵ�.
			lastCommand->SetUndoMacroEnd();
			//2.3.2 ǥ�ð� �������� ��Ÿ����.
			isDone = true;
		}
		//2.4 ǥ�ð� ���� �ȵǾ�����
		if (isDone == false)
		{
			//2.4.1 lastCommand�� command�� ���� ��ġ�� ������
			if (lastCommand->GetRowIndex() == command->GetRowIndex())
			{
				//2.4.1.1 lastCommand�� command�� ���� ��ġ�� ���� �� ĭ ���̰� �ȳ���
				lastCommandLetterIndex = lastCommand->GetLetterIndex() + 1;
				if (lastCommandLetterIndex != command->GetLetterIndex())
				{
					//2.4.1.1.1 lastCommand�� undoMacro����� ������ �������� ǥ���Ѵ�.
					lastCommand->SetUndoMacroEnd();
				}
			}
			//2.4.2 lastCommand�� command�� ���� ��ġ�� ���� �ٸ���
			else if (lastCommand->GetRowIndex() != command->GetRowIndex())
			{
				//2.4.2.1 lastCommand�� undoMacro����� ������ �������� ǥ���Ѵ�.
				lastCommand->SetUndoMacroEnd();
			}
		}
	}
	//3. undoList�� ��뷮�� �Ҵ緮���� ũ�ų� ������
	if (this->undoListLength >= this->undoListCapacity)
	{
		//3.1 undoList�� �Ҵ緮�� ������Ų��.
		this->undoListCapacity++;
	}
	//4. undoList�� ������ �迭 ��� ������ �߰��Ѵ�.
	Long index = this->undoList.Push(command);
	//5. undoList�� ��뷮�� ������Ų��.
	this->undoListLength++;
	//6. undoList�� �߰��� �迭����� ��ġ�� ��ȯ�Ѵ�.
	return index;
}

//RedoList�� ���� ������ �迭 ��� ������ �߰��ϱ�(Undo(�������)�� �� ��, OnCharCommand���� �߰���)
Long CommandHistory::PushRedoList(Command* command)
{
	//1. �Ű������� �Է¹��� command�� OnCharCommand�̸�
	if (dynamic_cast<OnCharCommand*>(command))
	{
		Glyph* glyph = dynamic_cast<OnCharCommand*>(command)->GetGlyph();
		//1.1 �Ű������� �Է¹��� command�� ���๮���̸�
		if (dynamic_cast<Row*>(glyph))
		{
			//1.1.1 command�� redoMacro����� ������ �������� ǥ���Ѵ�.
			command->SetRedoMacroEnd();
		}
	}
	//2. ���� redoList���� ������ �迭��Ҹ� ���Ѵ�.
	Command* lastCommand = this->redoList.GetTop();
	Long lastCommandLetterIndex = 0;
	//3. redoList�� ������ �迭��Ұ� ������(redoList�� ����� command�� �� ���� ������)
	if (lastCommand != 0)
	{
		//3.1 �Ű������� �Է¹��� command�� undoMacro����� ������ �����̸�
		if (command->IsUndoMacroEnd() == true)
		{
			//3.1.1  redoList���� ������ �迭���(lastCommand)��
			//redoMacro����� ������ �������� ǥ���Ѵ�.
			lastCommand->SetRedoMacroEnd();
		}
		//3.2 lastCommand�� command�� ���� ��ġ�� ������
		else if (lastCommand->GetRowIndex() == command->GetRowIndex())
		{
			//3.2.1 lastCommand�� ���ÿ����� ������ �ʾ�����
			if (lastCommand->IsSelectedTextsRemoved() == false)
			{
				//3.2.1.1 lastCommand�� command�� ���� ��ġ�� ���� ���̰� �ȳ���
				//Unexecute�� ���������� ������ġ ���̰� 1�� ���ٵ�, Unexecute�ϱ� ����
				//���� PushRedoList�� ����Ǳ� ������ ������ġ ���̰� �ȳ��� ������ ���ؾ� �Ѵ�!
				if (lastCommand->GetLetterIndex() != command->GetLetterIndex())
				{
					//3.2.1.1.1 �Ű������� �Է¹��� command�� redoMacro����� ������ �������� ǥ���Ѵ�.
					command->SetRedoMacroEnd();
				}
			}
			//3.2.2 lastCommand�� ���ÿ����� ��������
			else
			{
				//3.2.2.1 lastCommand�� redoMacro ����� ������ �������� ǥ���Ѵ�.
				lastCommand->SetRedoMacroEnd();
			}
		}
		//3.3 lastCommand�� command�� ���� ��ġ�� ���� �ٸ���
		else if (lastCommand->GetRowIndex() != command->GetRowIndex())
		{
			//3.3.1  �Ű������� �Է¹��� command�� redoMacro����� ������ �������� ǥ���Ѵ�.
			command->SetRedoMacroEnd();
		}
	}
	//4. redoList�� ��뷮�� �Ҵ緮���� ũ�ų� ������
	if (this->redoListLength >= this->redoListCapacity)
	{
		//4.1 redoList�� �Ҵ緮�� ������Ų��.
		this->redoListCapacity++;
	}
	//5. redoList�� ������ �迭 ��� ������ �Ű������� �Է¹��� command�� �߰��Ѵ�.
	Long index = this->redoList.Push(command);
	//6. redoList�� ��뷮�� ������Ų��.
	this->redoListLength++;
	//7. redoList�� �߰��� ������ �迭����� ��ġ�� ��ȯ�Ѵ�.
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
	while (this->notepadForm->commandHistory->redoListLength > 0)
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