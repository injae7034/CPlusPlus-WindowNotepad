#include "CommandHistory.h"
#include "NotepadForm.h"
#include "Command.h"


//디폴트 생성자 정의
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

//실행취소 정의
void CommandHistory::Undo()
{
	//UndoList가 비어있지 않으면
	if (this->notepadForm->commandHistory->IsUndoListEmpty() == false)
	{
		//마지막 배열 요소를 꺼낸다.
		Command* command = this->undoList.Pop();
		//할당량을 감소시킨다.
		this->undoListCapacity--;
		//사용량을 감소시킨다.
		this->undoListLength--;
		//RedoList에 꺼낸 배열요소를 추가한다.
		//1. 사용량이 할당량보다 크거나 같으면
		if (this->redoListLength >= this->redoListCapacity)
		{
			//1.1 할당량을 증가시킨다.
			this->redoListCapacity++;
		}
		//2. 마지막 배열 요소 다음에 추가한다.
		Long index = this->redoList.Push(command);
		//3. 사용량을 증가시킨다.
		this->redoListLength++;
		//꺼낸 command를 Unexecute한다.
		command->Unexecute();
	}
}

//다시실행 정의
void CommandHistory::Redo()
{
	//RedoList가 비어있지 않으면
	if (this->notepadForm->commandHistory->IsRedoListEmpty() == false)
	{
		//마지막 배열 요소를 꺼낸다.
		Command* command = this->redoList.Pop();
		//할당량을 감소시킨다.
		this->redoListCapacity--;
		//사용량을 감소시킨다.
		this->redoListLength--;
		//UNdoList에 꺼낸 배열요소를 추가한다.
		//1. 사용량이 할당량보다 크거나 같으면
		if (this->undoListLength >= this->undoListCapacity)
		{
			//1.1 할당량을 증가시킨다.
			this->undoListCapacity++;
		}
		//2. 마지막 배열 요소 다음에 추가한다.
		Long index = this->undoList.Push(command);
		//3. 사용량을 증가시킨다.
		this->undoListLength++;
		//꺼낸 command를 Unexecute한다.
		command->Execute();
	}
}

//UndoList의 제일 마지막 배열 요소 구하기
Command* CommandHistory::GetUndoListTop()
{
	return this->undoList.GetTop();
}

//RedoList의 제일 마지막 배열 요소 구하기
Command* CommandHistory::GetRedoListTop()
{
	return this->redoList.GetTop();
}

//UndoList의 제일 마지막 배열 요소 다음에 추가하기
Long CommandHistory::AddUndoList(Command* command)
{
	//1. 사용량이 할당량보다 크거나 같으면
	if (this->undoListLength >= this->undoListCapacity)
	{
		//1.1 할당량을 증가시킨다.
		this->undoListCapacity++;
	}
	//2. 마지막 배열 요소 다음에 추가한다.
	Long index = this->undoList.Push(command);
	//3. 사용량을 증가시킨다.
	this->undoListLength++;
	//4. 위치를 반환한다.
	return index;
}

//RedoList의 제일 마지막 배열 요소 다음에 추가하기
Long CommandHistory::AddRedoList(Command* command)
{
	//1. 사용량이 할당량보다 크거나 같으면
	if (this->redoListLength >= this->redoListCapacity)
	{
		//1.1 할당량을 증가시킨다.
		this->redoListCapacity++;
	}
	//2. 마지막 배열 요소 다음에 추가한다.
	Long index = this->redoList.Push(command);
	//3. 사용량을 증가시킨다.
	this->redoListLength++;
	//4. 위치를 반환한다.
	return index;
}

//UndoList의 제일 마지막 배열 요소를 꺼내기
Command* CommandHistory::RemoveUndoList()
{
	//마지막 배열 요소를 꺼낸다.
	Command* command = this->undoList.Pop();
	//할당량을 감소시킨다.
	this->undoListCapacity--;
	//사용량을 감소시킨다.
	this->undoListLength--;
	//마지막 배열 요소를 반환한다.
	return command;
}

//RedoList의 제일 마지막 배열 요소를 꺼내기
Command* CommandHistory::RemoveRedoList()
{
	//마지막 배열 요소를 꺼낸다.
	Command* command = this->redoList.Pop();
	//할당량을 감소시킨다.
	this->redoListCapacity--;
	//사용량을 감소시킨다.
	this->redoListLength--;
	//마지막 배열 요소를 반환한다.
	return command;
}

//소멸자
CommandHistory::~CommandHistory()
{
	Command* command = 0;
	//UndoList를 할당해제해준다.
	while (this->undoList.IsEmpty() == false)
	{
		command = this->undoList.Pop();
		if (command != 0)
		{
			delete command;
			command = 0;
		}
		//할당량을 감소시킨다.
		this->undoListCapacity--;
		//사용량을 감소시킨다.
		this->undoListLength--;
	}
	//RedoList를 할당해제해준다.
	while (this->redoList.IsEmpty() == false)
	{
		command = this->redoList.Pop();
		if (command != 0)
		{
			delete command;
			command = 0;
		}
		//할당량을 감소시킨다.
		this->redoListCapacity--;
		//사용량을 감소시킨다.
		this->redoListLength--;
	}
}