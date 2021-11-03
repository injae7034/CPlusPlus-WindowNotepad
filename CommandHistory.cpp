#include "CommandHistory.h"
#include "NotepadForm.h"
#include "Command.h"
#include "Glyph.h"
#include "Row.h"

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
	//redoList�� command�� ������
	if (this->redoListLength > 0)
	{
		// �ٽý����� Ȱ��ȭ ��Ų��.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REDO, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_REDO, 
			MF_BYCOMMAND | MF_ENABLED);
	}
	//undoList�� command�� ������
	if (this->undoListLength == 0)
	{
		// ������Ҹ� ��Ȱ��ȭ ��Ų��.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_UNDO, 
			MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_UNDO,
			MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
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
	//undoList�� command�� ������
	if (this->undoListLength > 0)
	{
		// ������Ҹ� Ȱ��ȭ ��Ų��.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_UNDO, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_UNDO, MF_BYCOMMAND | MF_ENABLED);
	}
	//redoList�� command�� ������
	if (this->redoListLength == 0)
	{
		// �ٽý����� ��Ȱ��ȭ ��Ų��.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REDO,
			MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_REDO,
			MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
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

//OnCharCommand, OnImeCharCommand�� ���� �޸��忡 �ؽ�Ʈ�� �Է��ϴ� command�� undoList�� �ֱ�
Long CommandHistory::PushUndoList(Command* command, bool isStop)
{
	//1. ���� undoList���� ������ ���� ���Ѵ�.
	Command* lastCommand = this->undoList.GetTop();
	//2. isStop�� false�̸�
	if (isStop == false)
	{
		//2.1 glyph�� ���Ѵ�.
		Glyph* glyph = command->GetGlyph();
		//2.2 �Ű������� �Է¹��� command�� ���๮�ڸ� ������ ������
		if (dynamic_cast<Row*>(glyph))
		{
			//2.2.1 command�� undoMacro����� ������ �������� ǥ���Ѵ�.
			command->SetUndoMacroEnd();
		}
		Long lastCommandLetterIndex = 0;//�ֽ� command�� ������ġ
		//2.3 undoList���� lastCommand�� ������(undoList�� ����� command�� �� ���� ������)
		if (lastCommand != 0)
		{
			//2.3.1 lastCommand�� command�� ���� ��ġ�� ������
			if (lastCommand->GetPastingEndYPos() == command->GetPastingEndYPos())
			{
				//2.3.1.1 lastCommand�� command�� ���� ��ġ�� ���� �� ĭ ���̰� �ȳ���
				lastCommandLetterIndex = lastCommand->GetPastingEndXPos() + 1;
				if (lastCommandLetterIndex != command->GetPastingEndXPos())
				{
					//2.3.1.1.1 lastCommand�� undoMacro����� ������ �������� ǥ���Ѵ�.
					lastCommand->SetUndoMacroEnd();
				}
			}
			//2.3.2 lastCommand�� command�� ���� ��ġ�� ���� �ٸ���
			else if (lastCommand->GetPastingEndYPos() != command->GetPastingEndYPos())
			{
				//2.3.2.1 lastCommand�� undoMacro����� ������ �������� ǥ���Ѵ�.
				lastCommand->SetUndoMacroEnd();
			}
		}
	}
	//3. isStop�� true�̸�
	else
	{
		//3.1 command�� ������ �������� ǥ���Ѵ�.
		command->SetUndoMacroEnd();
		//3.2 undoList���� lastCommand�� ������(undoList�� ����� command�� �� ���� ������)
		if (lastCommand != 0)
		{
			//3.2.1 lastCommand�� ������ �������� ǥ���Ѵ�.
			lastCommand->SetUndoMacroEnd();
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
	CString message;
	Long number = command->GetStartXPos();
	//1. Glyph�� ���Ѵ�.
	Glyph* glyph = command->GetGlyph();
	//2. �Ű������� �Է¹��� command�� ���๮�ڸ� ������ ������
	if (dynamic_cast<Row*>(glyph))
	{
		//2.1 command�� redoMacro����� ������ �������� ǥ���Ѵ�.
		command->SetRedoMacroEnd();
	}
	//3. ���� redoList���� ������ �迭��Ҹ� ���Ѵ�.
	Command* lastCommand = this->redoList.GetTop();
	Long lastCommandLetterIndex = 0;
	//4. redoList�� ������ �迭��Ұ� ������(redoList�� ����� command�� �� ���� ������)
	if (lastCommand != 0)
	{
		//4.1 �Ű������� �Է¹��� command�� undoMacro����� ������ �����̸�
		if (command->IsUndoMacroEnd() == true)
		{
			//4.1.1  redoList���� ������ �迭���(lastCommand)��
			//redoMacro����� ������ �������� ǥ���Ѵ�.
			lastCommand->SetRedoMacroEnd();
		}
		//4.2 lastCommand�� command�� ���� ��ġ�� ������
		else if (lastCommand->GetStartYPos() == command->GetStartYPos())
		{
			//4.2.1 lastCommand�� ���ÿ����� ������ �ʾ�����
			if (lastCommand->IsSelectedTextsRemoved() == false)
			{
				//1.4.1.1 lastCommand�� command�� ���� ��ġ�� ���� �� ĭ ���̰� �ȳ���
				lastCommandLetterIndex = command->GetStartXPos() + 1;
				if (lastCommandLetterIndex != lastCommand->GetStartXPos())
				{
					//4.2.1.1.1 �Ű������� �Է¹��� command�� redoMacro����� ������ �������� ǥ���Ѵ�.
					lastCommand->SetRedoMacroEnd();
				}
			}
			//4.2.2 lastCommand�� ���ÿ����� ��������
			else
			{
				//4.2.2.1 lastCommand�� redoMacro ����� ������ �������� ǥ���Ѵ�.
				lastCommand->SetRedoMacroEnd();
			}
		}
		//4.3 lastCommand�� command�� ���� ��ġ�� ���� �ٸ���
		else if (lastCommand->GetStartYPos() != command->GetStartYPos())
		{
			//4.3.1  �Ű������� �Է¹��� command�� redoMacro����� ������ �������� ǥ���Ѵ�.
			command->SetRedoMacroEnd();
		}
	}
	//5. redoList�� ��뷮�� �Ҵ緮���� ũ�ų� ������
	if (this->redoListLength >= this->redoListCapacity)
	{
		//5.1 redoList�� �Ҵ緮�� ������Ų��.
		this->redoListCapacity++;
	}
	//6. redoList�� ������ �迭 ��� ������ �Ű������� �Է¹��� command�� �߰��Ѵ�.
	Long index = this->redoList.Push(command);
	//7. redoList�� ��뷮�� ������Ų��.
	this->redoListLength++;
	//8. redoList�� �߰��� ������ �迭����� ��ġ�� ��ȯ�Ѵ�.
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