#include "CommandHistory.h"
#include "NotepadForm.h"
#include "Command.h"
#include "Glyph.h"
#include "Row.h"

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
	if (this->notepadForm->commandHistory->undoListLength > 0)
	{
		//1.1 undoList에서 마지막 배열 요소(command)를 꺼낸다.
		Command* command = this->PopUndoList();
		//1.2 꺼낸 command를 redoList의 마지막에 추가한다.
		this->PushRedoList(command);
		//1.3 꺼낸 command를 Unexecute한다.(실행취소)
		command->Unexecute();
		bool isStop = true;
		//1.4. 이전 command를 구한다.(command를 뺐기 때문에 previousCommand가 마지막이됨)
		Command* previousCommand = this->undoList.Pop();
		if (previousCommand != 0)
		{
			isStop = false;
		}
		//1.5 previousCommand가 null이 아니고 undoMacroEnd가 아닌동안
		//그리고 command가 선택영역을 지우지 않은동안 반복한다.
		while (isStop == false && previousCommand->IsUndoMacroEnd() == false
			&& command->IsSelectedTextsRemoved() == false)
		{
			//1.5.1 undoList의 할당량을 감소시킨다.
			this->undoListCapacity--;
			//1.5.2 undoList의 사용량을 감소시킨다.
			this->undoListLength--;
			//1.5.3 pushRedoList의 마지막 배열 요소 다음에 추가한다.
			this->PushRedoList(previousCommand);
			//1.5.4 꺼낸 previousCommand를 Unexecute한다.(실행취소)
			previousCommand->Unexecute();
			//1.5.5. UndoList에서 새로운 previousCommand를 꺼내기 전에 저장한다.
			command = previousCommand;
			//1.5.6 undoList에서 마지막 배열 요소를 꺼낸다.
			previousCommand = this->undoList.Pop();
			//1.5.7 undoList에 더이상 배열요소가 없으면
			if (previousCommand == 0)
			{
				//1.5.7.1 멈춘다고 표시한다.
				isStop = true;
			}
		}
		
		//1.6 previousCommand가 undoMacroEnd이면
		if (isStop == false && previousCommand->IsUndoMacroEnd() == true
			|| command->IsSelectedTextsRemoved() == true)
		{
			//1.6.1 꺼낸 previousCommand를 undoList의 마지막 배열 요소에 다시 추가한다.
			this->undoList.Push(previousCommand);
		}
	}
	//redoList에 command가 있으면
	if (this->redoListLength > 0)
	{
		// 다시실행을 활성화 시킨다.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REDO, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_REDO, 
			MF_BYCOMMAND | MF_ENABLED);
	}
	//undoList에 command가 없으면
	if (this->undoListLength == 0)
	{
		// 실행취소를 비활성화 시킨다.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_UNDO, 
			MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_UNDO,
			MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
}

//다시실행 정의
void CommandHistory::Redo()
{
	//1. RedoList가 비어있지 않으면
	if (this->notepadForm->commandHistory->redoListLength > 0)
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
		this->undoList.Push(command);
		//1.4 undoList의 사용량을 증가시킨다.
		this->undoListLength++;
		//1.5 꺼낸 command가 Execute 되기 전에 다시 실행이라는 표시를 한다.
		command->SetRedone();
		//1.6 꺼낸 command를 execute한다.
		command->Execute();
		bool isStop = true;
		//1.7 previousCommand를 구한다.(command를 뺐기 때문에 previousCommand가 마지막이됨)
		Command* previousCommand = this->redoList.Pop();
		if (previousCommand != 0)
		{
			isStop = false;
		}
		//1.8 previousCommand가 null이 아니고 RedoMacroEnd인동안 반복한다.
		while (isStop == false && previousCommand->IsRedoMacroEnd() == false)
		{
			//1.8.1 redoList의 할당량을 감소시킨다.
			this->redoListCapacity--;
			//1.8.2 redoList의 사용량을 감소시킨다.
			this->redoListLength--;
			//1.8.3 undoList의 사용량이 할당량보다 크거나 같으면
			if (this->undoListLength >= this->undoListCapacity)
			{
				//1.8.3.1 undoList의 할당량을 증가시킨다.
				this->undoListCapacity++;
			}
			//1.8.4 undoList의 마지막 배열 요소 다음에 추가한다.
			this->undoList.Push(previousCommand);
			//1.8.5 undoList의 사용량을 증가시킨다.
			this->undoListLength++;
			//1.8.6 previousCommand가 Execute되기 전에 다시 실행이라는 표시를 한다.
			previousCommand->SetRedone();
			//1.8.7 previousCommand를 Execute한다.
			previousCommand->Execute();
			//1.8.8 redoList에서 마지막 배열 요소를 꺼낸다.
			previousCommand = this->redoList.Pop();
			if (previousCommand == 0)
			{
				isStop = true;
			}
		}
		//1.9 previousCommand가 RedoMacroEnd이면
		if (isStop == false)
		{
			//1.9.1 꺼냈던 previousCommand를 redoList에 다시 넣어준다.
			this->redoList.Push(previousCommand);
		}
	}
	//undoList에 command가 있으면
	if (this->undoListLength > 0)
	{
		// 실행취소를 활성화 시킨다.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_UNDO, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_UNDO, MF_BYCOMMAND | MF_ENABLED);
	}
	//redoList에 command가 없으면
	if (this->redoListLength == 0)
	{
		// 다시실행을 비활성화 시킨다.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REDO,
			MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_REDO,
			MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
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

//OnCharCommand, OnImeCharCommand와 같이 메모장에 텍스트를 입력하는 command를 undoList에 넣기
Long CommandHistory::PushUndoList(Command* command, bool isStop)
{
	//1. 현재 undoList에서 마지막 값을 구한다.
	Command* lastCommand = this->undoList.GetTop();
	//2. isStop이 false이면
	if (isStop == false)
	{
		//2.1 glyph를 구한다.
		Glyph* glyph = command->GetGlyph();
		//2.2 매개변수로 입력받은 command가 개행문자를 가지고 있으면
		if (dynamic_cast<Row*>(glyph))
		{
			//2.2.1 command를 undoMacro출력이 끝나는 지점으로 표시한다.
			command->SetUndoMacroEnd();
		}
		Long lastCommandLetterIndex = 0;//최신 command의 글자위치
		//2.3 undoList에서 lastCommand가 있으면(undoList에 저장된 command가 한 개라도 있으면)
		if (lastCommand != 0)
		{
			//2.3.1 lastCommand와 command의 줄의 위치가 같으면
			if (lastCommand->GetPastingEndYPos() == command->GetPastingEndYPos())
			{
				//2.3.1.1 lastCommand와 command의 글자 위치를 비교해 한 칸 차이가 안나면
				lastCommandLetterIndex = lastCommand->GetPastingEndXPos() + 1;
				if (lastCommandLetterIndex != command->GetPastingEndXPos())
				{
					//2.3.1.1.1 lastCommand를 undoMacro출력이 끝나는 지점으로 표시한다.
					lastCommand->SetUndoMacroEnd();
				}
			}
			//2.3.2 lastCommand와 command의 줄의 위치가 서로 다르면
			else if (lastCommand->GetPastingEndYPos() != command->GetPastingEndYPos())
			{
				//2.3.2.1 lastCommand를 undoMacro출력이 끝나는 지점으로 표시한다.
				lastCommand->SetUndoMacroEnd();
			}
		}
	}
	//3. isStop이 true이면
	else
	{
		//3.1 command를 끝나는 지점으로 표시한다.
		command->SetUndoMacroEnd();
		//3.2 undoList에서 lastCommand가 있으면(undoList에 저장된 command가 한 개라도 있으면)
		if (lastCommand != 0)
		{
			//3.2.1 lastCommand를 끝나는 지점으로 표시한다.
			lastCommand->SetUndoMacroEnd();
		}
	}
	//3. undoList의 사용량이 할당량보다 크거나 같으면
	if (this->undoListLength >= this->undoListCapacity)
	{
		//3.1 undoList의 할당량을 증가시킨다.
		this->undoListCapacity++;
	}
	//4. undoList의 마지막 배열 요소 다음에 추가한다.
	Long index = this->undoList.Push(command);
	//5. undoList의 사용량을 증가시킨다.
	this->undoListLength++;
	//6. undoList에 추가한 배열요소의 위치를 반환한다.
	return index;
}

//RedoList의 제일 마지막 배열 요소 다음에 추가하기(Undo(실행취소)가 될 때, OnCharCommand들이 추가됨)
Long CommandHistory::PushRedoList(Command* command)
{
	CString message;
	Long number = command->GetStartXPos();
	//1. Glyph를 구한다.
	Glyph* glyph = command->GetGlyph();
	//2. 매개변수로 입력박은 command가 개행문자를 가지고 있으면
	if (dynamic_cast<Row*>(glyph))
	{
		//2.1 command를 redoMacro출력이 끝나는 지점으로 표시한다.
		command->SetRedoMacroEnd();
	}
	//3. 현재 redoList에서 마지막 배열요소를 구한다.
	Command* lastCommand = this->redoList.GetTop();
	Long lastCommandLetterIndex = 0;
	//4. redoList의 마지막 배열요소가 있으면(redoList에 저장된 command가 한 개라도 있으면)
	if (lastCommand != 0)
	{
		//4.1 매개변수로 입력받은 command가 undoMacro출력의 끝나는 지점이면
		if (command->IsUndoMacroEnd() == true)
		{
			//4.1.1  redoList에서 마지막 배열요소(lastCommand)를
			//redoMacro출력이 끝나는 지점으로 표시한다.
			lastCommand->SetRedoMacroEnd();
		}
		//4.2 lastCommand와 command의 줄의 위치가 같으면
		else if (lastCommand->GetStartYPos() == command->GetStartYPos())
		{
			//4.2.1 lastCommand가 선택영역을 지우지 않았으면
			if (lastCommand->IsSelectedTextsRemoved() == false)
			{
				//1.4.1.1 lastCommand와 command의 글자 위치를 비교해 한 칸 차이가 안나면
				lastCommandLetterIndex = command->GetStartXPos() + 1;
				if (lastCommandLetterIndex != lastCommand->GetStartXPos())
				{
					//4.2.1.1.1 매개변수로 입력박은 command를 redoMacro출력이 끝나는 지점으로 표시한다.
					lastCommand->SetRedoMacroEnd();
				}
			}
			//4.2.2 lastCommand가 선택영역을 지웠으면
			else
			{
				//4.2.2.1 lastCommand를 redoMacro 출력이 끝나는 지점으로 표시한다.
				lastCommand->SetRedoMacroEnd();
			}
		}
		//4.3 lastCommand와 command의 줄의 위치가 서로 다르면
		else if (lastCommand->GetStartYPos() != command->GetStartYPos())
		{
			//4.3.1  매개변수로 입력박은 command를 redoMacro출력이 끝나는 지점으로 표시한다.
			command->SetRedoMacroEnd();
		}
	}
	//5. redoList의 사용량이 할당량보다 크거나 같으면
	if (this->redoListLength >= this->redoListCapacity)
	{
		//5.1 redoList의 할당량을 증가시킨다.
		this->redoListCapacity++;
	}
	//6. redoList의 마지막 배열 요소 다음에 매개변수로 입력받은 command를 추가한다.
	Long index = this->redoList.Push(command);
	//7. redoList의 사용량을 증가시킨다.
	this->redoListLength++;
	//8. redoList에 추가한 마지막 배열요소의 위치를 반환한다.
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