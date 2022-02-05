#include "DeleteKeyActionCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"
#include "Row.h"
#include "RowAutoChange.h"
#include "DummyRow.h"

//디폴트생성자 정의
DeleteKeyActionCommand::DeleteKeyActionCommand(NotepadForm* notepadForm)
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
void DeleteKeyActionCommand::Execute()
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
	//3. DeleteKeyActionCommand가 다시 실행되면
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
		Glyph* letter = 0;
		//4.1 현재 노트의 마지막 줄의 위치를 구한다.
		Long lastRowPos = this->notepadForm->note->GetLength() - 1;
		//4.2 현재 줄에서 마지막 글자의 위치를 구한다.
		Long lastLetterPos = this->notepadForm->current->GetLength();
		//제일 마지막 줄이면 줄을 지울 수 없고, 마지막 줄에서 글자 위치가 마지막에 있으면 아무것도 안일어남
		// 현재 줄의 위치가 마지막이 아니고, 현재 글자 위치가 마지막이면 다음 줄을 현재 줄로 편입시킨다.
		//4.3 현재 줄의 위치가 노트의 마지막 줄 위치보다 작고, 현재 글자 위치가 마지막이면
		if (currentRowPos < lastRowPos && currentLetterPos == lastLetterPos)
		{
			//4.3.1 현재 줄의 다음 줄을 구한다.
			Glyph* nextRow = this->notepadForm->note->GetAt(currentRowPos + 1);
			//4.3.2 현재 줄의 다음 줄이 가짜줄이 아니면(진짜 줄이면)
			if (!dynamic_cast<DummyRow*>(nextRow))
			{
				//4.3.2.1 처음 실행이 되면
				if (this->isRedone == false)
				{
					//4.3.2.1.1 Row를 생성한다.
					this->glyph = new Row();
				}
				//4.3.2.2 현재 줄을 구한다.
				Glyph* currentRow = this->notepadForm->note->GetAt(currentRowPos);
				//4.3.2.3 다음 줄을 현재 줄에 합친다.
				nextRow->Join(currentRow);
				//4.3.2.4 Note에서 다음 줄의 주소를 지운다.
				this->notepadForm->note->Remove(currentRowPos + 1);
				//4.3.2.5 현재 줄이 지워졌기 때문에 현재 줄을 변경한다.
				currentRowPos = this->notepadForm->note->GetCurrent();
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				//4.3.2.6 현재 줄의 글자 위치가 지금은 줄의 마지막이기 때문에
				//합쳐진 줄의 사이인 lastLetterPos로 옮겨준다.
				currentLetterPos = this->notepadForm->current->Move(lastLetterPos);
			}
			//4.1.3 현재 줄의 다음 줄이 가짜줄이면
			else
			{
				//4.1.3.1 다음 줄의 첫글자로 이동한다.
				currentRowPos = this->notepadForm->note->Move(currentRowPos + 1);
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->First();
				//4.1.3.2 처음 실행이 되면
				if (this->isRedone == false)
				{
					//4.1.3.2.1 지울 글자를 구한다.
					letter = this->notepadForm->current->GetAt(currentLetterPos);
					//4.1.3.2.2 지울 글자를 깊은 복사해서 저장한다.
					this->glyph = letter->Clone();
				}
				//4.1.3.3 다음 줄의 첫번째 글자를 지운다.
				this->notepadForm->current->Remove(currentLetterPos);
				//4.1.3.4 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
				//OnSize에서 부분자동개행을 하도록 한다. 
				//가짜줄이 있다는게 자동개행이 진행중이라는 의미임.
				this->notepadForm->SendMessage(WM_SIZE);
				//4.1.3.5 글자 위치를 다시 현재 줄 마지막 글자로 조정해준다.
				currentRowPos = this->notepadForm->note->Move(currentRowPos - 1);
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->Last();
			}
			//4.1.4 Command에 변경 사항이 있음을 표시한다.
			this->isDirty = true;
		}
		// 현재 글자 위치가 마지막이 아닐 때(현재 줄이 마지막이든 아니든 상관없음)
		//현재 글자의 다음 글자를 지운다.
		//4.4 현재 글자 위치가 마지막이 아니면
		else if (currentLetterPos < lastLetterPos)
		{
			//4.4.1 처음 실행이 되면
			if (this->isRedone == false)
			{
				//4.4.1.1 지울 글자를 구한다.
				letter = this->notepadForm->current->GetAt(currentLetterPos);
				//4.4.1.2 지울 글자를 깊은 복사해서 저장한다.
				this->glyph = letter->Clone();
			}
			//4.4.2 현재 글자의 다음 글자를 지운다.
			this->notepadForm->current->Remove(currentLetterPos);
			//4.4.3 자동 줄 바꿈 메뉴가 체크되어 있으면
			if (this->notepadForm->isRowAutoChanging == true)
			{
				//4.4.5.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
				//OnSize에서 부분자동개행을 하도록 한다. 
				this->notepadForm->SendMessage(WM_SIZE);
			}
			//4.4.4 Command에 변경 사항이 있음을 표시한다.
			this->isDirty = true;
		}
		//4.5 Command에 변경 사항이 있으면
		if (this->isDirty == true)
		{
			//4.5.1 메모장 제목에 *를 추가한다.
			string name = this->notepadForm->fileName;
			name.insert(0, "*");
			name += " - 메모장";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//4.5.2 메모장에 변경사항이 있음을 저장한다.
			this->notepadForm->isDirty = true;
			//4.5.3 글자를 지운 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
			this->rowIndex = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
			this->letterIndex = this->notepadForm->current->GetCurrent();
			//4.5.4 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치를 저장해야함)
			if (this->notepadForm->isRowAutoChanging == true)
			{
				Long changedRowPos = this->rowIndex;
				Long changedLetterPos = this->letterIndex;
				Long originRowPos = 0;
				Long originLetterPos = 0;
				//4.5.4.1 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
				rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
					&originRowPos);
				//4.5.4.2 command에 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
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
void DeleteKeyActionCommand::Unexecute()
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
	if (!dynamic_cast<Row*>(this->glyph))
	{
		//5.1 현재 줄의 글자 위치가 현재 줄의 글자개수와 같으면
		if (currentLetterPos == this->notepadForm->current->GetLength())
		{
			//5.1.1 현재 줄의 마지막 글자 뒤에 새로운 글자를 추가한다.
			//여기서 command가 가지고 있는 glyph를 깊은 복사해서 줄에 추가해준다.
			//Command의 glyph는 note와는 따로 가지고 있어야 나중에 에러가 안나고 할당해제하기 편하다.
			//그리고 glyph의 원래 주인은 command이기 때문에 note가 깊은 복사를 해서 가져가는게 논리상 맞다.
			//그럼 이제 note가 할당해제될 때 자기가 깊은 복사를 한 glyph를 할당해제하고,
			//command가 할당해제될 때 자기가 가지고 있는 glyph를 할당해제 하기 때문에 어디서 할당해제할지
			//말지 머리 아플 일이 없고, 여러모로 command의 glyph를 관리하기 편하기 때문에 
			//command는 note와는 별도로 자기만의 glyph를 가지고 있어야한다.
			//아니면 나중에 Backspace를 다지우고 다시 글자를 입력할 때 PushUndo에서 에러가 난다.!
			currentLetterPos = this->notepadForm->current->Add(this->glyph->Clone());
		}
		//5.2 현재 줄의 글자 위치가 현재 줄의 글자개수와 다르면
		else
		{
			//5.2.1 현재 줄의 글자 위치에 글자를 끼워서 추가한다.
			currentLetterPos = this->notepadForm->current->
				Add(currentLetterPos, this->glyph->Clone());
		}
		//5.3 글자위치를 현재 글자보다 앞에 위치하게 조정한다.
		currentLetterPos = this->notepadForm->current->Previous();
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
		//자동개행이 끝난 후에 현재 줄의 이전 줄의 마지막 글자 위치로 이동시킨다.
		//6.7 현재 줄을 새로 저장한다.(새로 생성된 줄이 아니라 분리한 줄을 현재 줄로 한다.)
		currentRowPos = this->notepadForm->note->Move(currentRowPos - 1);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		//6.8 현재 줄의 글자 위치를 마지막으로 이동시킨다.
		this->notepadForm->current->Last();
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
void DeleteKeyActionCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void DeleteKeyActionCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}

//SetRedone(다시 실행이라고 설정함)
void DeleteKeyActionCommand::SetRedone()
{
	this->isRedone = true;
}

//줄의 위치 구하기
Long DeleteKeyActionCommand::GetRowIndex()
{
	return this->rowIndex;
}
//글자 위치 구하기
Long DeleteKeyActionCommand::GetLetterIndex()
{
	return this->letterIndex;
}
//실행취소 종료지점여부 구하기
bool DeleteKeyActionCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//다시실행 종료지점여부 구하기 
bool DeleteKeyActionCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}
//다시실행인지 여부 구하기
bool DeleteKeyActionCommand::IsRedone()
{
	return this->isRedone;
}
//변경사항이 있는지 확인 여부
bool DeleteKeyActionCommand::IsDirty()
{
	return this->isDirty;
}

//소멸자 정의
DeleteKeyActionCommand::~DeleteKeyActionCommand()
{
	//1. Row이든 letter이든 할당된게 있으면 할당해제한다.
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}