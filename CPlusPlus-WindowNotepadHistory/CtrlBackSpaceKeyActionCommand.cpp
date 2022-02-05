#include "CtrlBackSpaceKeyActionCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"
#include "Row.h"
#include "DummyRow.h"
#include "RowAutoChange.h"

//디폴트생성자 정의
CtrlBackSpaceKeyActionCommand::CtrlBackSpaceKeyActionCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{
	this->rowIndex = notepadForm->note->GetCurrent();
	this->letterIndex = notepadForm->current->GetCurrent();
	this->glyph = 0;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
	this->isDirty = false;//처음에 생성될 때는 변경사항이 없으므로 false가 디폴트값임.
}

//실행
void CtrlBackSpaceKeyActionCommand::Execute()
{
	//1. RowAutoChange를 생성한다.
	RowAutoChange rowAutoChange(this->notepadForm);
	Long changedRowPos = 0;
	Long changedLetterPos = 0;
	Long originRowPos = this->rowIndex;
	Long originLetterPos = this->letterIndex;
	//2. 현재 줄의 위치와 글자 위치를 구한다.
	Long currentRowPos = this->notepadForm->note->GetCurrent();
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	//3. CtrlBackSpaceKeyActionCommand가 다시 실행되면
	if (this->isRedone == true)
	{
		//3.1 현재 줄의 위치와 글자위치를 재조정해준다.
		currentRowPos = this->notepadForm->note->Move(this->rowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
		//3.2 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치가 저장되어 있음)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//3.2.1 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
			rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
				&changedRowPos);
			//3.2.2 현재 줄의 위치와 글자 위치를 다시 조정한다.
			currentRowPos = this->notepadForm->note->Move(changedRowPos);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
		}
	}
	//4. 메모장에서 선택된 texts가 없으면
	if (this->notepadForm->isSelecting == false)
	{
		//제일 첫 줄이면 줄을 지울 수 없고, 제일 첫 줄에서 글자 위치가 제일 처음에 있으면
		//아무것도 안일어남. 현재 줄의 위치가 제일 처음이 아니고, 현재 글자 위치가 제일 처음이면
		//현재 줄을 이전 줄에 Join시킨다.
		//4.1 현재 줄의 위치가 0보다 크고, 현재 글자 위치가 0이면
		Glyph* letter = 0;
		if (currentRowPos > 0 && currentLetterPos == 0)
		{
			//4.1.1 현재 줄을 구한다.
			Glyph* currentRow = this->notepadForm->note->GetAt(currentRowPos);
			//4.1.2 현재 줄이 가짜줄이 아니면(진짜 줄이면)
			if (!dynamic_cast<DummyRow*>(currentRow))
			{
				//4.1.2.1 처음 실행이 되면
				if (this->isRedone == false)
				{
					//4.1.2.1.1 Row를 생성한다.
					this->glyph = new Row();
				}
				//4.1.2.2 현재 줄의 이전 줄을 구한다.
				Glyph* previousRow = this->notepadForm->note->GetAt(currentRowPos - 1);
				//4.1.2.3 현재 줄의 이전 줄의 마지막 글자 위치를 구한다.
				Long letterPos = previousRow->GetLength();
				//4.1.2.4 현재 줄을 이전 줄에 합친다.
				currentRow->Join(previousRow);
				//4.1.2.5 Note에서 현재 줄의 주소를 지운다.
				this->notepadForm->note->Remove(currentRowPos);
				//4.1.2.6 현재 줄이 지워졌기 때문에 현재 줄을 변경한다.
				currentRowPos = this->notepadForm->note->GetCurrent();
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				//4.1.2.7 현재 줄의 글자 위치가 지금은 마지막이기 때문에 변경해준다.
				//이전 줄의 마지막 현재 줄의 처음 사이에 위치하도록 조정한다.
				currentLetterPos = this->notepadForm->current->Move(letterPos);
			}
			//4.1.3 현재 줄이 가짜줄이면
			else
			{
				//4.1.3.1 이전 줄로 이동한다.
				currentRowPos = this->notepadForm->note->Move(currentRowPos - 1);
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				//4.1.3.2 처음 실행이 되면
				if (this->isRedone == false)
				{
					//4.1.3.2.1 지울 글자를 구한다.
					letter = this->notepadForm->current->
						GetAt(this->notepadForm->current->GetLength() - 1);
					//4.1.3.2.2 지울 글자를 깊은 복사해서 저장한다.
					this->glyph = letter->Clone();
				}
				//4.1.3.3 이전 줄의 마지막 글자를 지운다.
				this->notepadForm->current->Remove(this->notepadForm->current->GetLength() - 1);
				//4.1.3.4 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
				//OnSize에서 부분자동개행을 하도록 한다. 
				//가짜줄이 있다는게 자동개행이 진행중이라는 의미임.
				this->notepadForm->SendMessage(WM_SIZE);
			}
			//4.1.4 Command에 변경 사항이 있음을 표시한다.
			this->isDirty = true;
		}
		// 현재 글자 위치가 처음이 아닐 때(현재 줄이 처음이든 아니든 상관없음) 현재 글자를 지운다.
		//4.2 현재 글자 위치가 처음이 아니면
		else if (currentLetterPos > 0)
		{
			Long i = 0;
			Glyph* letter = 0;
			//4.2.1 처음 실행이 되면
			if (this->isRedone == false)
			{
				//DummyRow를 생성해서 글자를 담고 그걸 복사해서 this->glyph에 주소를 옮기고,
				//글자들을 단어단위로 지우기전에 옮겨서 저장한다.
				//4.2.1.1 DummyRow를 생성한다.
				this->glyph = new DummyRow();
				//4.2.1.2 왼쪽 방향으로 단어단위로 이동한 뒤의 글자위치를 구한다.
				Long letterPosAfterMoving = this->notepadForm->current->PreviousWord();
				//4.2.1.3 왼쪽 방향으로 단어단위로 이동한 뒤의 글자위치가 
				//현재 글자 위치보다 작은동안 반복한다.
				while (letterPosAfterMoving < currentLetterPos)
				{
					//4.2.1.3.1 글자를 지우기 전에 글자를 구한다.
					letter = this->notepadForm->current->GetAt(letterPosAfterMoving);
					//4.2.1.3.2 글자를 깊은 복사해서 DummyRow에 저장한다.
					this->glyph->Add(letter->Clone());
					//4.2.1.3.3 글자를 지운다.
					this->notepadForm->current->Remove(letterPosAfterMoving);
					currentLetterPos--;
				}
			}
			//4.2.2 처음 실행이 아니면
			else
			{
				//4.2.2.1 command에 저장되어 있는 줄의 글자개수를 구한다.
				Long lengthOfCommandRow = this->glyph->GetLength();
				//4.2.2.2 currentLetterPos가 lengthOfCommandRow보다 크거나 같으면
				if (currentLetterPos >= lengthOfCommandRow)
				{
					//4.2.2.2.1 lengthOfCommandRow가 0보다 큰동안 반복한다.
					while (lengthOfCommandRow > 0)
					{
						//4.2.2.2.1.1 글자를 지운다.
						this->notepadForm->current->Remove(currentLetterPos - 1);
						currentLetterPos--;
						lengthOfCommandRow--;
					}
				}
				//4.2.2.3 currentLetterPos가 lengthOfCommandRow보다 작으면
				else
				{
					//4.2.2.3.1 lengthOfCommandRow가 0보다 큰동안 반복한다.
					while (lengthOfCommandRow > 0)
					{
						//4.2.2.3.1.1 currentLetterPos가 0보다 큰 동안 그리고 
						//lengthOfCommandRow가 0보다 큰동안 반복한다.
						while (currentLetterPos > 0 && lengthOfCommandRow > 0)
						{
							//4.2.2.3.1.1.1 글자를 지운다.
							this->notepadForm->current->Remove(currentLetterPos - 1);
							currentLetterPos--;
							lengthOfCommandRow--;
						}
						//4.2.2.3.1.2 lengthOfCommandRow가 0보다 크면
						if (lengthOfCommandRow > 0)
						{
							//4.2.2.3.1.2.1 현재 줄의 이전 줄로 이동한다.
							currentRowPos = this->notepadForm->note->Previous();
							this->notepadForm->current = this->notepadForm->
								note->GetAt(currentRowPos);
							//4.2.2.3.1.2.1 줄의 길이를 currentLetterPos에 저장한다.
							currentLetterPos = this->notepadForm->current->GetLength();
						}
					}
				}
			}
			//4.2.4 자동 줄 바꿈 메뉴가 체크되어 있으면
			if (this->notepadForm->isRowAutoChanging == true)
			{
				//4.2.4.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
				//OnSize에서 부분자동개행을 하도록 한다. 
				this->notepadForm->SendMessage(WM_SIZE);
			}
			//4.2.5 Command에 변경 사항이 있음을 표시한다.
			this->isDirty = true;
		}
		//4.3 Command에 변경 사항이 있으면
		if (this->isDirty == true)
		{
			//4.3.1 메모장 제목에 *를 추가한다.
			string name = this->notepadForm->fileName;
			name.insert(0, "*");
			name += " - 메모장";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//4.3.2 메모장에 변경사항이 있음을 저장한다.
			this->notepadForm->isDirty = true;
			//4.3.3 글자를 지운 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
			this->rowIndex = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
			this->letterIndex = this->notepadForm->current->GetCurrent();
			//4.3.4 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치를 저장해야함)
			if (this->notepadForm->isRowAutoChanging == true)
			{
				Long changedRowPos = this->rowIndex;
				Long changedLetterPos = this->letterIndex;
				Long originRowPos = 0;
				Long originLetterPos = 0;
				//4.3.4.1 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
				rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
					&originRowPos);
				//4.3.4.2 command에 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
				this->rowIndex = originRowPos;
				this->letterIndex = originLetterPos;
			}
		}
	}
	//5. 메모장에서 선택된 texts가 있으면
	else
	{
		//5.1 RemoveCommand로 메세지를 보내서 선택영역을 지운다.
		this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_REMOVE);
	}
}

//실행취소
void CtrlBackSpaceKeyActionCommand::Unexecute()
{
	//1. RowAutoChange를 생성한다.
	RowAutoChange rowAutoChange(this->notepadForm);
	Long changedRowPos = 0;
	Long changedLetterPos = 0;
	Long originRowPos = this->rowIndex;
	Long originLetterPos = this->letterIndex;
	//2. 현재 줄의 위치를 이동시킨다.(캐럿이 다른 곳에 있으면 그 곳에 글자가 지워지기 때문에)
	Long currentRowPos = this->notepadForm->note->Move(this->rowIndex);
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	//3. 현재 글자의 위치를 이동시킨다.
	Long currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
	//4. 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치가 저장되어 있음)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//4.1 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
		rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
			&changedRowPos);
		//4.2 현재 줄의 위치와 글자 위치를 다시 조정한다.
		currentRowPos = this->notepadForm->note->Move(changedRowPos);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
	}
	//5. 지울 때 저장한 glyph가 줄(개행문자)이 아니면
	if (dynamic_cast<DummyRow*>(this->glyph))
	{
		//5.1 현재 줄의 글자 위치가 현재 줄의 글자개수와 같으면
		if (currentLetterPos == this->notepadForm->current->GetLength())
		{
			//5.1.1 현재 줄을 구한다.
			Glyph* currentRow = this->notepadForm->current;
			//5.1.2 dummyRow를 깊은 복사한다.
			Glyph* dummyRow = this->glyph->Clone();
			//5.1.3 dummyRow를 현재 줄에 합친다.
			dummyRow->Join(currentRow);
			//5.1.4 dummyRow를 할당해제한다.
			if (dummyRow != 0)
			{
				delete dummyRow;
			}
			//5.1.5 글자 위치를 현재 줄에서 제일 마지막으로 보낸다.
			currentLetterPos = currentRow->Last();
		}
		//5.2 현재 줄의 글자 위치가 현재 줄의 글자개수와 다르면
		else
		{
			//5.2.1 dummyRow의 개수만큼 반복한다.
			Glyph* letter = 0;
			Long i = this->glyph->GetLength() - 1;//마지막 배열요소부터 끼워야 순서가 제대로 된다.
			Long distance = currentLetterPos;
			while (i >= 0)
			{
				//5.2.1.1 글자를 구한다.
				letter = this->glyph->GetAt(i);
				//5.2.1.2 현재 줄의 글자 위치에 dummyRow에서 깊은 복사를 한 letter를 끼워 넣는다.
				currentLetterPos = this->notepadForm->current->
					Add(currentLetterPos, letter->Clone());
				//5.2.1.3 이동할 위치를 증가시킨다.
				distance++;
				//5.2.1.3 i를 증가시킨다.
				i--;
			}
			//5.2.2 글자 위치를 재조정한다.
			this->notepadForm->current->Move(distance);
		}
		//OnSize에 가서 자동으로 한줄 자동개행이 되고 줄의 위치와 글자위치가 자동개행에 맞게 다시 조정된다.
	}
	//6. 지울 때 저장한 glyph가 줄(개행문자)이면
	else
	{
		//6.1 현재 줄에서 현재 글자 다음 위치에 있는 글자들을 떼어내 새로운 줄을 만든다.
		Glyph* row = this->notepadForm->current->Split(currentLetterPos);
		//6.2 현재 줄의 위치가 노트의 줄의 개수-1 과 같고(현재 줄의 위치가 마지막 줄이면)
		if (currentRowPos == this->notepadForm->note->GetLength() - 1)
		{
			//6.2.1 새로운 줄을 마지막 줄 다음에 추가한다.
			currentRowPos = this->notepadForm->note->Add(row);
		}
		//6.3 그게 아니면
		else
		{
			//6.3.1 새로운 줄을 현재 줄의 다음 위치에 끼워 넣는다.
			currentRowPos = this->notepadForm->note->
				Add(currentRowPos + 1, row);
		}
		//6.4 현재 줄을 새로 저장한다.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		//6.5 현재 줄의 글자 위치를 처음으로 이동시킨다.
		this->notepadForm->current->First();
		//6.6 자동 줄 바꿈이 진행중이면
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//6.6.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
			//OnSize에서 부분자동개행을 하도록 한다. 
			this->notepadForm->SendMessage(WM_SIZE);
		}
	}
	//변경사항을 갱신함
	//7. isComposing을 false로 바꾼다.
	this->notepadForm->isComposing = false;
	//8. 메모장 제목에 *를 추가한다.
	string name = this->notepadForm->fileName;
	name.insert(0, "*");
	name += " - 메모장";
	this->notepadForm->SetWindowText(CString(name.c_str()));
	//9. 메모장에 변경사항이 있음을 저장한다.
	this->notepadForm->isDirty = true;
	//10. 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();
	//11. 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치를 저장해야함)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		Long changedRowPos = this->rowIndex;
		Long changedLetterPos = this->letterIndex;
		Long originRowPos = 0;
		Long originLetterPos = 0;
		//11.1 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
		rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
			&originRowPos);
		//11.2 command에 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
		this->rowIndex = originRowPos;
		this->letterIndex = originLetterPos;
	}
}

//SetMacroEnd(실행취소 및 다시실행 매크로출력 종료지점 설정)
void CtrlBackSpaceKeyActionCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void CtrlBackSpaceKeyActionCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}


//SetRedone(다시 실행이라고 설정함)
void CtrlBackSpaceKeyActionCommand::SetRedone()
{
	this->isRedone = true;
}

//줄의 위치 구하기
Long CtrlBackSpaceKeyActionCommand::GetRowIndex()
{
	return this->rowIndex;
}
//글자 위치 구하기
Long CtrlBackSpaceKeyActionCommand::GetLetterIndex()
{
	return this->letterIndex;
}
//실행취소 종료지점여부 구하기
bool CtrlBackSpaceKeyActionCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//다시실행 종료지점여부 구하기 
bool CtrlBackSpaceKeyActionCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}
//다시실행인지 여부 구하기
bool CtrlBackSpaceKeyActionCommand::IsRedone()
{
	return this->isRedone;
}

//변경사항이 있는지 확인 여부
bool CtrlBackSpaceKeyActionCommand::IsDirty()
{
	return this->isDirty;
}

//소멸자 정의
CtrlBackSpaceKeyActionCommand::~CtrlBackSpaceKeyActionCommand()
{
	//1. DummyRow나 Row를 할당해제한다.
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}