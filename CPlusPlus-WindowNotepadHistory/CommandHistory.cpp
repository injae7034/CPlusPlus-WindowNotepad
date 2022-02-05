#include "CommandHistory.h"
#include "NotepadForm.h"
#include "Command.h"
#include "OnCharCommand.h"
#include "Note.h"

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
	//1. UndoList가 비어있지 않으면
	if (this->notepadForm->commandHistory->IsUndoListEmpty() == false)
	{
		//1.1 undoList에서 마지막 배열 요소(command)를 꺼낸다.
		Command* command = this->PopUndoList();
		//1.2 꺼낸 command를 redoList의 마지막에 추가한다.
		Long index = this->PushRedoList(command);
		//1.3 꺼낸 command를 Unexecute한다.(실행취소)
		command->Unexecute();
		bool isStop = true;
		//1.4 command가 OnCharCommand이면
		if (dynamic_cast<OnCharCommand*>(command))
		{
			//1.4.1 이전 command를 구한다.(command를 뺐기 때문에 previousCommand가 마지막이됨)
			Command* previousCommand = this->undoList.Pop();
			if (previousCommand != 0)
			{
				isStop = false;
			}
			//1.4.2 previousCommand가 null이 아니고  previousCommand가 OnCharCommand이면
			if (isStop == false && dynamic_cast<OnCharCommand*>(previousCommand))
			{
				//1.4.2.1 previousCommand가 undoMacroEnd가 아닌동안 반복한다.
				while (isStop == false && dynamic_cast<OnCharCommand*>(previousCommand)
					->IsUndoMacroEnd() == false)
				{
					//1.4.2.1.1 undoList의 할당량을 감소시킨다.
					this->undoListCapacity--;
					//1.4.2.1.2 undoList의 사용량을 감소시킨다.
					this->undoListLength--;
					//1.4.2.1.3 pushRedoList의 마지막 배열 요소 다음에 추가한다.
					Long index = this->PushRedoList(previousCommand);
					//1.4.2.1.4 꺼낸 previousCommand를 Unexecute한다.(실행취소)
					previousCommand->Unexecute();
					//1.4.2.1.5 undoList에서 마지막 배열 요소를 꺼낸다.
					previousCommand = this->undoList.Pop();
					if (previousCommand == 0)
					{
						isStop = true;
					}
				}
				//1.4.2.2 previousCommand가 undoMacroEnd이면
				if (isStop == false)
				{
					//1.4.2.2.1 꺼낸 previousCommand를 undoList의 마지막 배열 요소에 다시 추가한다.
					this->undoList.Push(previousCommand);
				}

			}
			//1.4.3 previousCommand가 null이 아니고  previousCommand가 OnCharCommand가 아니면
			else if (isStop == false && !dynamic_cast<OnCharCommand*>(previousCommand))
			{
				//1.4.3.1 undoList에 다시 넣어준다.
				this->undoList.Push(previousCommand);
			}
		}
	}
}

//다시실행 정의
void CommandHistory::Redo()
{
	//1. RedoList가 비어있지 않으면
	if (this->notepadForm->commandHistory->IsRedoListEmpty() == false)
	{
		//1.1 redoList의 마지막 배열 요소를 꺼낸다.
		Command* command = this->PopRedoList();
		//1.2 undoList의 사용량이 할당량보다 크거나 같으면
		if (this->undoListLength >= this->undoListCapacity)
		{
			//1.2.1 undoList의 할당량을 증가시킨다.
			this->undoListCapacity++;
		}
		//1.3 undoList의 마지막 배열 요소 다음에 추가한다.
		Long index = this->undoList.Push(command);
		//1.4 undoList의 사용량을 증가시킨다.
		this->undoListLength++;
		//1.5 꺼낸 command를 execute한다.
		command->Execute();
		bool isStop = true;
		//1.7 command가 OnCharCommand이면
		if (dynamic_cast<OnCharCommand*>(command))
		{
			//1.7.1 previousCommand를 구한다.(command를 뺐기 때문에 previousCommand가 마지막이됨)
			Command* previousCommand = this->redoList.Pop();
			if (previousCommand != 0)
			{
				isStop = false;
			}
			//1.7.2 previousCommand가 null이 아니고, OnCharCommand이면
			if (isStop == false && dynamic_cast<OnCharCommand*>(previousCommand))
			{
				//1.7.2.1 previousCommand가 RedoMacroEnd인동안 반복한다.
				while (isStop == false && dynamic_cast<OnCharCommand*>(previousCommand)
					->IsRedoMacroEnd() == false)
				{
					//1.7.2.1.1 redoList의 할당량을 감소시킨다.
					this->redoListCapacity--;
					//1.7.2.1.2 redoList의 사용량을 감소시킨다.
					this->redoListLength--;
					//1.7.2.1.3 undoList의 사용량이 할당량보다 크거나 같으면
					if (this->undoListLength >= this->undoListCapacity)
					{
						//1.7.2.1.3.1 undoList의 할당량을 증가시킨다.
						this->undoListCapacity++;
					}
					//1.7.2.1.4 undoList의 마지막 배열 요소 다음에 추가한다.
					this->undoList.Push(previousCommand);
					//1.7.2.1.5 undoList의 사용량을 증가시킨다.
					this->undoListLength++;
					//1.7.2.1.6 previousCommand를 Execute한다.
					previousCommand->Execute();
					//1.7.2.1.7 redoList에서 마지막 배열 요소를 꺼낸다.
					previousCommand = this->redoList.Pop();
					if (previousCommand == 0)
					{
						isStop = true;
					}
				}
				//1.7.2.2 previousCommand가 RedoMacroEnd이면
				if (isStop == false)
				{
					//1.7.2.2.1 꺼냈던 previousCommand를 redoList에 다시 넣어준다.
					this->redoList.Push(previousCommand);
				}
			}
			//1.7.3 previousCommand가 null이 아니고 OnCharCommand가 아니면
			else if (isStop == false &&
				!dynamic_cast<OnCharCommand*>(previousCommand))
			{
				//1.7.3.1 꺼낸 command를 redoList에 다시 넣어준다.
				this->redoList.Push(previousCommand);
			}
		}
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

//UndoList의 제일 마지막 배열 요소 다음에 추가하기(OnCharCommand가 실행될 때 OnCharCommand들이 추가됨)
Long CommandHistory::PushUndoList(Command* command)
{
	Command* lastCommand = 0;
	Long lastCommandLetterIndex = 0;
	//1. 매개변수로 입력받은 command가 OnCharCommand이면
	if (dynamic_cast<OnCharCommand*>(command))
	{
		//1.1 현재 undoList에서 마지막 값을 구한다.
		lastCommand = this->undoList.GetTop();
		//1.2 undoList에서 lastCommand가 있으면(undoList에 저장된 command가 한 개라도 있으면)
		if (lastCommand != 0)
		{
			//1.2.1 lastCommand가 OnCharCommand이면
			if (dynamic_cast<OnCharCommand*>(lastCommand))
			{
				//1.2.1.1 lastCommand가 개행문자이면
				if (dynamic_cast<OnCharCommand*>(lastCommand)->GetNChar() == '\n'
					|| dynamic_cast<OnCharCommand*>(lastCommand)->GetNChar() == '\r')
				{
					//1.2.1.1.1 lastCommand를 undoMacro출력이 끝나는 지점으로 표시한다.
					dynamic_cast<OnCharCommand*>(lastCommand)->SetUndoMacroEnd();
				}
				//1.2.1.2 lastCommand와 command의 줄의 위치가 같으면
				else if (dynamic_cast<OnCharCommand*>(lastCommand)->GetRowIndex()
					== dynamic_cast<OnCharCommand*>(command)->GetRowIndex())
				{
					//1.2.1.2.1 lastCommand와 command의 글자 위치를 비교해 한 칸 차이가 안나면
					lastCommandLetterIndex = dynamic_cast<OnCharCommand*>(lastCommand)
						->GetLetterIndex() + 1;
					if (lastCommandLetterIndex !=
						dynamic_cast<OnCharCommand*>(command)->GetLetterIndex())
					{
						//1.2.1.2.1.1 lastCommand를 undoMacro출력이 끝나는 지점으로 표시한다.
						dynamic_cast<OnCharCommand*>(lastCommand)->SetUndoMacroEnd();
					}
				}
				//1.2.1.3 lastCommand와 command의 줄의 위치가 서로 다르면
				else if(dynamic_cast<OnCharCommand*>(lastCommand)->GetRowIndex()
					!= dynamic_cast<OnCharCommand*>(command)->GetRowIndex())
				{
					//1.2.1.3.1 lastCommand를 undoMacro출력이 끝나는 지점으로 표시한다.
					dynamic_cast<OnCharCommand*>(lastCommand)->SetUndoMacroEnd();
				}
			}
		}
	}
	//2. undoList의 사용량이 할당량보다 크거나 같으면
	if (this->undoListLength >= this->undoListCapacity)
	{
		//2.1 undoList의 할당량을 증가시킨다.
		this->undoListCapacity++;
	}
	//3. undoList의 마지막 배열 요소 다음에 추가한다.
	Long index = this->undoList.Push(command);
	//4. undoList의 사용량을 증가시킨다.
	this->undoListLength++;
	//5. undoList에 추가한 배열요소의 위치를 반환한다.
	return index;
}

//RedoList의 제일 마지막 배열 요소 다음에 추가하기(Undo(실행취소)가 될 때, OnCharCommand들이 추가됨)
Long CommandHistory::PushRedoList(Command* command)
{
	Command* lastCommand = 0;
	Long lastCommandLetterIndex = 0;
	//1. 매개변수로 입력받은 command가 OnCharCommand이면
	if (dynamic_cast<OnCharCommand*>(command))
	{
		//1.1 매개변수로 입력박은 command가 개행문자이면
		if (dynamic_cast<OnCharCommand*>(command)->GetNChar() == '\n'
			|| dynamic_cast<OnCharCommand*>(command)->GetNChar() == '\r')
		{
			//1.1.1 command를 redoMacro출력이 끝나는 지점으로 표시한다.
			dynamic_cast<OnCharCommand*>(command)->SetRedoMacroEnd();
		}
		//1.2 현재 redoList에서 마지막 배열요소를 구한다.
		lastCommand = this->redoList.GetTop();
		//1.3 redoList의 마지막 배열요소가 있으면(redoList에 저장된 command가 한 개라도 있으면)
		if (lastCommand != 0)
		{
			//1.3.1 매개변수로 입력받은 command가 undoMacro출력의 끝나는 지점이면
			if (dynamic_cast<OnCharCommand*>(command)->IsUndoMacroEnd() == true)
			{
				//1.3.1.1  redoList에서 마지막 배열요소(lastCommand)를
				//redoMacro출력이 끝나는 지점으로 표시한다.
				dynamic_cast<OnCharCommand*>(lastCommand)->SetRedoMacroEnd();
			}
			//1.3.2 lastCommand와 command의 줄의 위치가 같으면
			else if (dynamic_cast<OnCharCommand*>(lastCommand)->GetRowIndex()
				== dynamic_cast<OnCharCommand*>(command)->GetRowIndex())
			{
				//1.3.2.1 lastCommand와 command의 글자 위치를 비교해 한 칸 차이가 안나면
				lastCommandLetterIndex = dynamic_cast<OnCharCommand*>(lastCommand)
					->GetLetterIndex() - 1;
				if (lastCommandLetterIndex !=
					dynamic_cast<OnCharCommand*>(command)->GetLetterIndex())
				{
					//1.3.2.1.1 매개변수로 입력박은 command를 redoMacro출력이 끝나는 지점으로 표시한다.
					dynamic_cast<OnCharCommand*>(command)->SetRedoMacroEnd();
				}
			}
			//1.3.2.1 lastCommand와 command의 줄의 위치가 서로 다르면
			else if (dynamic_cast<OnCharCommand*>(lastCommand)->GetRowIndex()
				!= dynamic_cast<OnCharCommand*>(command)->GetRowIndex())
			{
				//1.3.2.1.1  매개변수로 입력박은 command를 redoMacro출력이 끝나는 지점으로 표시한다.
				dynamic_cast<OnCharCommand*>(command)->SetRedoMacroEnd();
			}
			
		}
	}
	//2. redoList의 사용량이 할당량보다 크거나 같으면
	if (this->redoListLength >= this->redoListCapacity)
	{
		//2.1 redoList의 할당량을 증가시킨다.
		this->redoListCapacity++;
	}
	//3. redoList의 마지막 배열 요소 다음에 매개변수로 입력받은 command를 추가한다.
	Long index = this->redoList.Push(command);
	//4. redoList의 사용량을 증가시킨다.
	this->redoListLength++;
	//5. redoList에 추가한 마지막 배열요소의 위치를 반환한다.
	return index;
}

//UndoList의 제일 마지막 배열 요소를 꺼내기
Command* CommandHistory::PopUndoList()
{
	//1. 마지막 배열 요소를 꺼낸다.
	Command* command = this->undoList.Pop();
	//2. 할당량을 감소시킨다.
	this->undoListCapacity--;
	//3. 사용량을 감소시킨다.
	this->undoListLength--;
	//4. 꺼낸 마지막 배열 요소를 반환한다.
	return command;
}

//RedoList의 제일 마지막 배열 요소를 꺼내기
Command* CommandHistory::PopRedoList()
{
	//1. 마지막 배열 요소를 꺼낸다.
	Command* command = this->redoList.Pop();
	//2. 할당량을 감소시킨다.
	this->redoListCapacity--;
	//3. 사용량을 감소시킨다.
	this->redoListLength--;
	//4. 마지막 배열 요소를 반환한다.
	return command;
}

//RedoList 초기화시키기(OnCharCommand가 처음 실행될 때 redoList는 무조건 다 초기화된다.)
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

//소멸자
CommandHistory::~CommandHistory()
{
	Command* command = 0;
	//1. UndoList를 할당해제해준다.
	while (this->undoList.IsEmpty() == false)
	{
		//1.1 undoList에 저장된 주소가 가르키는 힙에 할당된 내용들을 할당해제해준다.
		command = this->undoList.Pop();
		if (command != 0)
		{
			delete command;
			command = 0;
		}
		//1.2 할당량을 감소시킨다.
		this->undoListCapacity--;
		//1.3 사용량을 감소시킨다.
		this->undoListLength--;
	}
	//2. RedoList를 할당해제해준다.
	while (this->redoList.IsEmpty() == false)
	{
		//2.1 redoList에 저장된 주소가 가르키는 힙에 할당된 내용들을 할당해제해준다.
		command = this->redoList.Pop();
		if (command != 0)
		{
			delete command;
			command = 0;
		}
		//2.2 할당량을 감소시킨다.
		this->redoListCapacity--;
		//2.3 사용량을 감소시킨다.
		this->redoListLength--;
	}
}