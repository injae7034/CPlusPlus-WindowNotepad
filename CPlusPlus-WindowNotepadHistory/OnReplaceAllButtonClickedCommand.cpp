#include "OnReplaceAllButtonClickedCommand.h"
#include "CommandHistory.h"
#include "Row.h"
#include "RowAutoChange.h"
#include "DummyRow.h"
#include "TextExtent.h"
#include "SelectingTexts.h"
#include "ReplacingDialog.h"
#include "GlyphFinder.h"
#include "OnReplaceButtonClickedCommand.h"

//디폴트생성자
OnReplaceAllButtonClickedCommand::OnReplaceAllButtonClickedCommand(NotepadForm* notepadForm,
	Long undoListCapacity, Long redoListCapacity)
	:Command(notepadForm), 
	findingKeyword(((ReplacingDialog*)(this->notepadForm->findReplaceDialog))->findingKeyword),
	replacingKeyword(((ReplacingDialog*)(this->notepadForm->findReplaceDialog))->replacingKeyword),
	undoList(), redoList()
{
	this->matchCaseChecked = ((ReplacingDialog*)(this->notepadForm->findReplaceDialog))
		->matchCaseChecked;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
	this->undoListCapacity = undoListCapacity;
	this->undoListLength = 0;
	this->redoListCapacity = redoListCapacity;
	this->redoListLength = 0;
}

//처음 및 다시 실행
void OnReplaceAllButtonClickedCommand::Execute()
{
	//1. 선택이 진행되고 있는 중이었으면
	if (this->notepadForm->isSelecting == true)
	{
		//1.1. 선택된 텍스트를 선택해제한다.(선택을 끝낸다.)
		this->notepadForm->selectingTexts->Undo();
		//1.2 선택이 끝난 상태로 바꾼다.
		this->notepadForm->isSelecting = false;
		//1.3 선택이 끝났기 때문에 캐럿의 x좌표를 0으로 저장한다.
		this->notepadForm->selectedStartXPos = 0;
		//1.4 선택이 끝났기 때문에 캐럿의 y좌표를 0으로 저장한다.
		this->notepadForm->selectedStartYPos = 0;
		//1.5 복사하기, 잘라내기, 삭제 메뉴를 비활성화 시킨다.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
	//2. 현재 줄의 위치와 글자 위치를 처음으로 이동시킨다.
	Long currentRowPos = this->notepadForm->note->First();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	Long currentLetterPos = this->notepadForm->current->First();
	//3. GlyphFinder를 생성한다.
	GlyphFinder glyphFinder(this->notepadForm->note);

	Long findingStartRowIndex = 0;//찾은 단어의 시작 줄위치
	Long findingStartLetterIndex = 0;//찾은 단어의 시작 글자위치
	Long findingEndRowIndex = 0;//찾은 단어의 끝 줄위치
	Long findingEndLetterIndex = 0;//찾은 단어의 끝 글자위치
	bool isFounded = true;//찾을 단어가 발견이 되었는지 아닌지를 판별할 flag
	Command* command = 0;//OnRepalceButtonClickedCommand를 담을 공간
	Long i = 0;//반복제어변수

	//4. 처음 실행이면
	if (this->isRedone == false)
	{
		//4.1 대/소문자 구분이 되어 있으면
		if (this->matchCaseChecked == BST_CHECKED)
		{
			//4.1.1 찾은게 있는 동안 반복한다.
			while (isFounded == true)
			{
				//4.1.1.1 아래로 찾기를 실행한다.
				glyphFinder.FindDown(this->findingKeyword, &findingStartRowIndex,
					&findingStartLetterIndex, &findingEndRowIndex, &findingEndLetterIndex);
				//4.1.1.2 찾은 게 있으면
				if (findingStartRowIndex != findingEndRowIndex ||
					findingStartLetterIndex != findingEndLetterIndex)
				{
					//4.1.1.2.1 선택이 시작되기 때문에 선택이 시작됨을 표시한다.
					this->notepadForm->isSelecting = true;
					//4.1.1.2.2 선택이 시작되는 캐럿의 x좌표를 저장한다.
					this->notepadForm->selectedStartXPos = findingStartLetterIndex;
					//4.1.1.2.3 선택이 시작되는 캐럿의 y좌표를 저장한다.
					this->notepadForm->selectedStartYPos = findingStartRowIndex;
					//4.1.2.2.4 찾은 글자를 선택한다.
					this->notepadForm->selectingTexts->DoNext(findingStartRowIndex,
						findingStartLetterIndex, findingEndRowIndex, findingEndLetterIndex);
					//4.1.2.2.5 캐럿의 위치를 메모장의 찾은 문자열이 있는 줄의 찾은 문자열 마지막 글자위치로 이동한다.
					currentRowPos = this->notepadForm->note->Move(findingEndRowIndex);
					this->notepadForm->current = this->notepadForm->note->GetAt(findingEndRowIndex);
					currentLetterPos = this->notepadForm->current->Move(findingEndLetterIndex);
					//4.1.2.2.6 OnReplaceButtonClikedCommand를 생성한다.
					command = new OnReplaceButtonClickedCommand(this->notepadForm);
					//4.1.2.2.7 undoList의 사용량이 할당량보다 크거나 같으면
					if (this->undoListLength >= this->undoListCapacity)
					{
						//4.1.2.2.7.1 undoList의 할당량을 증가시킨다.
						this->undoListCapacity++;
					}
					//4.1.2.2.8 undoList에 생성한 OnReplaceButtonClikedCommand를 넣는다.
					this->undoList.Push(command);
					//4.1.2.2.9 undoList의 사용량을 증가시킨다.
					this->undoListLength++;
					//4.1.2.2.10 OnReplaceButtonClikedCommand를 Execute한다.
					command->Execute();
				}
				//4.1.1.3 찾은 게 없으면
				else
				{
					//4.1.1.3.1 찾은 게 없다고 표시한다.
					isFounded = false;
				}
			}

		}
		//4.2 대/소문자 구분이 안되어었으면
		else
		{
			//4.2.1 찾은게 있는 동안 반복한다.
			while (isFounded == true)
			{
				//4.2.1.1 대/소문자 구분없이 아래로 찾기를 실행한다.
				glyphFinder.FindDownWithMatchCase(this->findingKeyword, &findingStartRowIndex,
					&findingStartLetterIndex, &findingEndRowIndex, &findingEndLetterIndex);
				//4.2.1.2 찾은 게 있으면
				if (findingStartRowIndex != findingEndRowIndex ||
					findingStartLetterIndex != findingEndLetterIndex)
				{
					//4.2.1.2.1 선택이 시작되기 때문에 선택이 시작됨을 표시한다.
					this->notepadForm->isSelecting = true;
					//4.2.1.2.2 선택이 시작되는 캐럿의 x좌표를 저장한다.
					this->notepadForm->selectedStartXPos = findingStartLetterIndex;
					//4.2.1.2.3 선택이 시작되는 캐럿의 y좌표를 저장한다.
					this->notepadForm->selectedStartYPos = findingStartRowIndex;
					//4.2.2.2.4 찾은 글자를 선택한다.
					this->notepadForm->selectingTexts->DoNext(findingStartRowIndex,
						findingStartLetterIndex, findingEndRowIndex, findingEndLetterIndex);
					//4.2.2.2.5 캐럿의 위치를 메모장의 찾은 문자열이 있는 줄의 찾은 문자열 마지막 글자위치로 이동한다.
					currentRowPos = this->notepadForm->note->Move(findingEndRowIndex);
					this->notepadForm->current = this->notepadForm->note->GetAt(findingEndRowIndex);
					currentLetterPos = this->notepadForm->current->Move(findingEndLetterIndex);
					//4.2.2.2.6 OnReplaceButtonClikedCommand를 생성한다.
					command = new OnReplaceButtonClickedCommand(this->notepadForm);
					//4.2.2.2.7 undoList의 사용량이 할당량보다 크거나 같으면
					if (this->undoListLength >= this->undoListCapacity)
					{
						//4.2.2.2.7.1 undoList의 할당량을 증가시킨다.
						this->undoListCapacity++;
					}
					//4.2.2.2.8 undoList에 생성한 OnReplaceButtonClikedCommand를 넣는다.
					this->undoList.Push(command);
					//4.2.2.2.9 undoList의 사용량을 증가시킨다.
					this->undoListLength++;
					//4.2.2.2.10 OnReplaceButtonClikedCommand를 Execute한다.
					command->Execute();
				}
				//4.2.1.3 찾은 게 없으면
				else
				{
					//4.2.1.3.1 찾은 게 없다고 표시한다.
					isFounded = false;
				}
			}
		}
		//4.3 메모장 제목에 *를 추가한다.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - 메모장";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//4.4 메모장에 변경사항이 있음을 저장한다.
		this->notepadForm->isDirty = true;
	}
	//5. 다시 실행이면
	else
	{
		//5.1 redoList 사용량만큼 반복한다.
		i = 0;
		while (i < this->redoListLength)
		{
			//5.1.1 redoList에서 가장 최근 command를 뺀다.
			command = this->redoList.Pop();
			//5.1.2 redoList의 할당량을 감소시킨다.
			this->redoListCapacity--;
			//5.1.3 redoList의 사용량을 감소시킨다.
			this->redoListLength--;
			//5.1.4 undoList의 사용량이 할당량보다 크거나 같으면
			if (this->undoListLength >= this->undoListCapacity)
			{
				//5.1.4.1 undoList의 할당량을 증가시킨다.
				this->undoListCapacity++;
			}
			//5.1.5 undoList에 OnReplaceButtonClickedCommand를 넣는다.
			this->undoList.Push(command);
			//5.1.6 undoList의 사용량을 증가시킨다.
			this->undoListLength++;
			//5.1.7 꺼낸 OnReplaceButtonClickedCommand가 Execute 되기 전에 다시 실행이라는 표시를 한다.
			command->SetRedone();
			//5.1.8 OnReplaceButtonClickedCommand를 Execute한다.
			command->Execute();
		}
	}
	//6. 현재 줄의 위치와 글자 위치를 처음으로 이동시킨다.
	currentRowPos = this->notepadForm->note->First();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	currentLetterPos = this->notepadForm->current->First();
}

//※실행취소했을경우에 자동개행이 제대로 안됨!
//실행취소
void OnReplaceAllButtonClickedCommand::Unexecute()
{
	//1. 선택이 진행되고 있는 중이었으면
	if (this->notepadForm->isSelecting == true)
	{
		//1.1. 선택된 텍스트를 선택해제한다.(선택을 끝낸다.)
		this->notepadForm->selectingTexts->Undo();
		//1.2 선택이 끝난 상태로 바꾼다.
		this->notepadForm->isSelecting = false;
		//1.3 선택이 끝났기 때문에 캐럿의 x좌표를 0으로 저장한다.
		this->notepadForm->selectedStartXPos = 0;
		//1.4 선택이 끝났기 때문에 캐럿의 y좌표를 0으로 저장한다.
		this->notepadForm->selectedStartYPos = 0;
	}
	//2. undoList 사용량만큼 반복한다.
	Long i = 0;
	Command* command = 0;//OnReplaceButtonClickedCommand를 담을 공간
	while (i < this->undoListLength)
	{
		//2.1 undoList에서 가장 최근 command를 뺀다.
		command = this->undoList.Pop();
		//2.2 undoList의 할당량을 감소시킨다.
		this->undoListCapacity--;
		//2.3 undoList의 사용량을 감소시킨다.
		this->undoListLength--;
		//2.4 redoList의 사용량이 할당량보다 크거나 같으면
		if (this->redoListLength >= this->redoListCapacity)
		{
			//2.4.1 redoList의 할당량을 증가시킨다.
			this->redoListCapacity++;
		}
		//2.5 redoList에 OnReplaceButtonClickedCommand를 넣는다.
		this->redoList.Push(command);
		//2.6 redoList의 사용량을 증가시킨다.
		this->redoListLength++;
		//2.7 OnReplaceButtonClickedCommand를 Unexecute한다.
		command->Unexecute();
	}
	//OnReplaceButtonClickedCommand를 Unexecute하면 선택영역이 복원되서 선택영역이 생기기 때문에
	//선택영역을 다시 없애준다.
	//3. 선택이 진행되고 있는 중이었으면()
	if (this->notepadForm->isSelecting == true)
	{
		//3.1. 선택된 텍스트를 선택해제한다.(선택을 끝낸다.)
		this->notepadForm->selectingTexts->Undo();
		//3.2 선택이 끝난 상태로 바꾼다.
		this->notepadForm->isSelecting = false;
		//3.3 선택이 끝났기 때문에 캐럿의 x좌표를 0으로 저장한다.
		this->notepadForm->selectedStartXPos = 0;
		//3.4 선택이 끝났기 때문에 캐럿의 y좌표를 0으로 저장한다.
		this->notepadForm->selectedStartYPos = 0;
	}
	//4. 현재 줄의 위치와 글자 위치를 마지막으로 이동시킨다.
	Long currentRowPos = this->notepadForm->note->Last();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	Long currentLetterPos = this->notepadForm->current->Last();
}

//다시실행인지 여부 구하기
bool OnReplaceAllButtonClickedCommand::IsRedone()
{
	return this->isRedone;
}

//SetMacroEnd(실행취소 및 다시실행 매크로출력 종료지점 설정)
void OnReplaceAllButtonClickedCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void OnReplaceAllButtonClickedCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}

//실행취소 종료지점여부 구하기
bool OnReplaceAllButtonClickedCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//다시실행 종료지점여부 구하기 
bool OnReplaceAllButtonClickedCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}

//SetRedone(다시 실행이라고 설정함)
void OnReplaceAllButtonClickedCommand::SetRedone()
{
	this->isRedone = true;
}

//소멸자
OnReplaceAllButtonClickedCommand::~OnReplaceAllButtonClickedCommand()
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