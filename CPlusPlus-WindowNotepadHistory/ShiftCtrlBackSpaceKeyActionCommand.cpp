#include "ShiftCtrlBackSpaceKeyActionCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"
#include "Row.h"
#include "DummyRow.h"
#include "RowAutoChange.h"
#include "Note.h"
#include "SelectingTexts.h"

//디폴트생성자 정의
ShiftCtrlBackSpaceKeyActionCommand::ShiftCtrlBackSpaceKeyActionCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{
	this->rowIndex = notepadForm->note->GetCurrent();
	this->letterIndex = notepadForm->current->GetCurrent();
	this->glyph = 0;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
	this->isDirty = false;//처음에 생성될 때는 변경사항이 없으므로 false가 디폴트값임.
	this->selectedStartXPos = 0;
	this->selectedStartYPos = 0;
	this->isSelectedTextsRemoved = false;//처음에 생성될 때는 선택영역이 안지워졌으므로 false가 디폴트값
}

//실행
void ShiftCtrlBackSpaceKeyActionCommand::Execute()
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
	//3. ShiftCtrlBackSpaceKeyActionCommand가 다시 실행되면
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
	//4. 메모장에서 선택된 texts가 없고, 선택된 영역을 안지웠으면
	if (this->notepadForm->isSelecting == false && this->isSelectedTextsRemoved == false)
	{
		//4.1 현재 줄을 구한다.
		Glyph* currentRow = this->notepadForm->note->GetAt(currentRowPos);
		Glyph* row = 0;
		//4.2 현재 줄의 위치가 0이고, 현재 글자 위치가 0이면
		if (currentRowPos == 0 && currentLetterPos == 0)
		{
			//4.2.1 처음 실행이면
			if (this->isRedone == false)
			{
				//4.2.1.1 Command에 변경 사항이 없음을 표시한다.
				this->isDirty = false;
			}
		}
		//4.3 현재 줄의 위치가 0보다 크고, 현재 글자 위치가 0이고, 현재 줄이 진짜 줄이면
		else if (currentRowPos > 0 && currentLetterPos == 0
			&& !dynamic_cast<DummyRow*>(currentRow))
		{
			//4.3.1 처음 실행이면
			if (this->isRedone == false)
			{
				//4.3.1.1 Row를 생성한다.
				this->glyph = new Row();
			}
			//4.3.2 현재 줄의 이전 줄을 구한다.
			Glyph* previousRow = this->notepadForm->note->GetAt(currentRowPos - 1);
			//4.3.3 현재 줄의 이전 줄의 마지막 글자 위치를 구한다.
			Long letterPos = previousRow->GetLength();
			//4.3.4 현재 줄을 이전 줄에 합친다.
			currentRow->Join(previousRow);
			//4.3.5 Note에서 현재 줄의 주소를 지운다.
			this->notepadForm->note->Remove(currentRowPos);
			//4.3.6 현재 줄이 지워졌기 때문에 현재 줄을 변경한다.
			currentRowPos = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			//4.3.7 현재 줄의 글자 위치가 지금은 마지막이기 때문에 변경해준다.
			//이전 줄의 마지막 현재 줄의 처음 사이에 위치하도록 조정한다.
			currentLetterPos = this->notepadForm->current->Move(letterPos);
			//4.3.8 Command에 변경 사항이 있음을 표시한다.
			this->isDirty = true;
		}
		//4.4 그 이외에는
		else
		{
			Glyph* letter = 0;
			//4.4.1 처음 실행이면
			if (this->isRedone == false)
			{
				//4.4.1.1 DummyRow를 생성한다.
				this->glyph = new DummyRow();
				//4.4.1.2 Command에 변경 사항이 있음을 표시한다.
				this->isDirty = true;
			}
			//4.4.2 현재 줄이 가짜줄이면
			if (dynamic_cast<DummyRow*>(currentRow))
			{
				//4.4.2.1 처음 실행이면
				if (this->isRedone == false)
				{
					//4.4.2.1.1 현재 글자위치가 0보다 작은동안 반복한다.
					while (currentLetterPos > 0)
					{
						//4.4.2.1.1.1 글자를 지우기 전에 글자를 구한다.
						letter = currentRow->GetAt(currentLetterPos - 1);
						//4.4.2.1.1.2 글자를 깊은 복사해서 DummyRow에 저장한다.
						this->glyph->Add(0, letter->Clone());
						//4.4.2.1.1.3 글자를 지운다.
						currentRow->Remove(currentLetterPos - 1);
						currentLetterPos--;
					}
				}
				//4.4.2.2 다시 실행이면
				else
				{
					//4.4.2.2.1 현재 글자위치가 0보다 작은동안 반복한다.
					while (currentLetterPos > 0)
					{
						//4.4.2.2.1.1 글자를 지운다.
						currentRow->Remove(currentLetterPos - 1);
						currentLetterPos--;
					}
				}
				//4.4.2.3 줄의 주소를 저장한다.
				row = currentRow;
				//4.4.2.4 이전 줄의 마지막 글자위치로 이동한다.
				currentRowPos = this->notepadForm->note->Move(currentRowPos - 1);
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->Last();
				currentRow = this->notepadForm->current;
			}
			//4.4.5 처음 실행이면
			if (this->isRedone == false)
			{
				//4.4.5.1 가짜 줄인동안 반복한다. 
				while (dynamic_cast<DummyRow*>(currentRow))
				{
					//4.4.5.1.1 현재 글자위치가 0보다 작은동안 반복한다.
					while (currentLetterPos > 0)
					{
						//4.4.5.1.1.1 글자를 지우기 전에 글자를 구한다.
						letter = currentRow->GetAt(currentLetterPos - 1);
						//4.4.5.1.1.2 글자를 깊은 복사해서 DummyRow에 저장한다.
						this->glyph->Add(0, letter->Clone());
						//4.4.5.1.1.3 글자를 지운다.
						currentRow->Remove(currentLetterPos - 1);
						currentLetterPos--;
					}
					//4.4.5.1.2 글자가 다지워졌기 때문에 Note에서 현재 줄의 주소를 지운다.
					this->notepadForm->note->Remove(currentRowPos);
					//4.4.5.1.3 이전 줄의 마지막 글자위치로 이동한다.
					currentRowPos = this->notepadForm->note->Move(currentRowPos - 1);
					this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
					currentLetterPos = this->notepadForm->current->Last();
					currentRow = this->notepadForm->current;
				}
				//4.4.5.2 진짜 줄의 글자를 다 지우는 동안 반복한다.
				while (currentLetterPos > 0)
				{
					//4.4.5.2.1 글자를 지우기 전에 글자를 구한다.
					letter = currentRow->GetAt(currentLetterPos - 1);
					//4.4.5.2.2 글자를 깊은 복사해서 DummyRow에 저장한다.
					this->glyph->Add(0, letter->Clone());
					//4.4.5.2.3 글자를 지운다.
					currentRow->Remove(currentLetterPos - 1);
					currentLetterPos--;
				}
			}
			//4.4.6 다시 실행이면
			else
			{
				//4.4.6.1 가짜 줄인동안 반복한다. 
				while (dynamic_cast<DummyRow*>(currentRow))
				{
					//4.4.6.1.1 현재 글자위치가 0보다 작은동안 반복한다.
					while (currentLetterPos > 0)
					{
						//4.4.6.1.1.1 글자를 지운다.
						currentRow->Remove(currentLetterPos - 1);
						currentLetterPos--;
					}
					//4.4.6.1.2 글자가 다지워졌기 때문에 Note에서 현재 줄의 주소를 지운다.
					this->notepadForm->note->Remove(currentRowPos);
					//4.4.6.1.3 이전 줄의 마지막 글자위치로 이동한다.
					currentRowPos = this->notepadForm->note->Move(currentRowPos - 1);
					this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
					currentLetterPos = this->notepadForm->current->Last();
					currentRow = this->notepadForm->current;
				}
				//4.4.6.2 진짜 줄의 글자를 다 지우는 동안 반복한다.
				while (currentLetterPos > 0)
				{
					//4.4.6.2.1 글자를 지운다.
					currentRow->Remove(currentLetterPos - 1);
					currentLetterPos--;
				}
			}
			//4.4.7 줄에 글자가 남아있으면
			if (row != 0)
			{
				//4.4.7.1 글자가 남아있는 줄을 현재 줄에 합친다.
				row->Join(currentRow);
				//4.4.7.2 Note에서 글자가 남아있는 줄의 주소를 지운다.
				this->notepadForm->note->Remove(currentRowPos + 1);
				//4.4.7.3 현재줄에서 캐럿을 제일 처음으로 보낸다.
				currentRow->First();
			}
		}	
		//4.5 자동 줄 바꿈 메뉴가 체크되어 있으면
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//4.4.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
			//OnSize에서 부분자동개행을 하도록 한다. 
			this->notepadForm->SendMessage(WM_SIZE);
		}
	}
	//5. 메모장에서 선택된 texts가 있거나 또는 선택된 영역을 지웠으면
	else if (this->notepadForm->isSelecting == true || this->isSelectedTextsRemoved == true)
	{
		//5.1 다시 실행이면
		if (this->isRedone == true)
		{
			//5.1.1 notepadForm의 선택이 시작되는 줄의 위치와 글자 위치를 재설정해준다. 
			this->notepadForm->selectedStartYPos = this->selectedStartYPos;
			this->notepadForm->selectedStartXPos = this->selectedStartXPos;
		}
		//5.2 선택이 시작되는 줄과 글자 위치, 선택이 끝나는 줄과 글자 위치를 저장한다.
		Long selectedStartRowPos = this->notepadForm->selectedStartYPos;//선택이 시작되는 줄
		Long selectedStartLetterPos = this->notepadForm->selectedStartXPos;//선택이 시작되는 글자
		Long selectedEndRowPos = currentRowPos;//선택이 끝나는 줄
		Long selectedEndLetterPos = currentLetterPos;//선택이 끝나는 글자
		//5.3 처음 실행이면
		if (this->isRedone == false)
		{
			//5.3.1 content를 복사하고 지운다.
			this->glyph = this->notepadForm->note->CopySelectedTextsAndRemove(selectedStartRowPos,
				selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
			//5.3.2 Command에 변경 사항이 있음을 표시한다.
			this->isDirty = true;
		}
		//5.4 다시 실행이면
		else
		{
			//5.4.1 content를 지운다.
			this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
				selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
		}
		//5.5 연산이 끝났기 때문에 현재 줄의 위치를 다시 조정해준다.(note의연산안에서 현재 줄의 위치와 글자 위치는
		//조정이 되지만 notepadForm의 current(현재줄)는 조정할 수 없어서 notepadForm에서 해준다.)
		this->notepadForm->current = this->notepadForm->note->
		GetAt(this->notepadForm->note->GetCurrent());
		//5.6 자동 줄 바꿈 메뉴가 체크되어 있으면
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//5.6.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
			//OnSize에서 부분자동개행을 하도록 한다. 
			this->notepadForm->SendMessage(WM_SIZE);
		}
		//5.6 메모장에서 선택된 texts를 다 지웠기 때문에 메모장에서 선택이 안된 상태로 바꾼다.
		this->notepadForm->isSelecting = false;
		//5.7 선택된 texts를 지웠기 때문에 command가 선택된 영역을 지웠다고 표시한다.
		this->isSelectedTextsRemoved = true;
		//5.8 선택이 끝났기 때문에 캐럿의 x좌표를 0으로 저장한다.
		this->notepadForm->selectedStartXPos = 0;
		//5.9 선택이 끝났기 때문에 캐럿의 y좌표를 0으로 저장한다.
		this->notepadForm->selectedStartYPos = 0;
		//5.10 복사하기, 잘라내기, 삭제 메뉴를 비활성화 시킨다.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
	//6. Command에 변경 사항이 있으면
	if (this->isDirty == true)
	{
		//6.1 메모장 제목에 *를 추가한다.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - 메모장";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//6.2 메모장에 변경사항이 있음을 저장한다.
		this->notepadForm->isDirty = true;
	}
	//7. 글자를 지운 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();
	//8 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치를 저장해야함)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		Long changedRowPos = this->rowIndex;
		Long changedLetterPos = this->letterIndex;
		Long originRowPos = 0;
		Long originLetterPos = 0;
		//8.1 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
		rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
			&originRowPos);
		//8.2 command에 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
		this->rowIndex = originRowPos;
		this->letterIndex = originLetterPos;
	}
}

//실행취소
void ShiftCtrlBackSpaceKeyActionCommand::Unexecute()
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
	//5. 지울 때 저장한 glyph가 가짜줄이면
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
	else if (dynamic_cast<Row*>(this->glyph))
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
	//7. 지울 때 저장한 glyph가 노트(선택영역)이면
	else if (dynamic_cast<Note*>(this->glyph))
	{
		//7.1 선택이 진행되고 있는 중이었으면
		if (this->notepadForm->isSelecting == true)
		{
			//7.1.1 선택된 텍스트를 선택해제한다.(선택을 끝낸다.)
			this->notepadForm->selectingTexts->Undo();
			//7.1.2 선택이 끝난 상태로 바꾼다.
			this->notepadForm->isSelecting = false;
			//7.1.3 선택이 끝났기 때문에 캐럿의 x좌표를 0으로 저장한다.
			this->notepadForm->selectedStartXPos = 0;
			//7.1.4 선택이 끝났기 때문에 캐럿의 y좌표를 0으로 저장한다.
			this->notepadForm->selectedStartYPos = 0;
		}
		//7.2 메모장에서 선택된 texts를 다시 복구하기 때문에 메모장에서 선택이 된 상태로 바꾼다.
		this->notepadForm->isSelecting = true;
		//7.3 선택이 다시 시작했기 때문에 캐럿의 x좌표를 command에 저장된 현재 글자 위치로 저장한다.
		this->notepadForm->selectedStartXPos = currentLetterPos;
		this->selectedStartXPos = this->notepadForm->selectedStartXPos;
		//7.4 선택이 다시 시작했기 때문에 캐럿의 y좌표를 command에 저장된 현재 줄의 위치로 저장한다.
		this->notepadForm->selectedStartYPos = currentRowPos;
		this->selectedStartYPos = this->notepadForm->selectedStartYPos;
		//7.5 현재 위치에 command가 가지고 있는 content를 삽입한다.
		Long rowIndex = this->notepadForm->note->
			InsertTexts(currentRowPos, currentLetterPos, this->glyph);
		//7.6 메모장의 현재 줄을 저장한다.
		this->notepadForm->current = this->notepadForm->note->GetAt(rowIndex);
		//7.7 자동개행이 진행중이면 붙여넣은 줄들을 자동개행시켜준다.
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//7.7.1 부분자동개행을 한다.
			Long endPastedRowPos = rowAutoChange.DoPartRows(currentRowPos, rowIndex);
			//7.7.2 붙여넣기가 끝나는 줄로 이동시킨다.
			//붙여넣기가 끝나는 줄은 OnSize에서 부분자동개행을 해서 처리되기 때문에 캐럿의 위치만 조정해주면 됨!
			currentRowPos = this->notepadForm->note->Move(endPastedRowPos);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->GetCurrent();
			this->notepadForm->current->Move(currentLetterPos);
		}
		//7.8 선택영역이 다시 생겼기 때문에 복사하기, 잘라내기, 삭제 메뉴를 활성화 시킨다.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
	}
	//8. 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();
	//9. 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치를 저장해야함)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		Long changedRowPos = this->rowIndex;
		Long changedLetterPos = this->letterIndex;
		Long originRowPos = 0;
		Long originLetterPos = 0;
		//9.1 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
		rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
			&originRowPos);
		//9.2 command에 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
		this->rowIndex = originRowPos;
		this->letterIndex = originLetterPos;
	}
}

//SetMacroEnd(실행취소 및 다시실행 매크로출력 종료지점 설정)
void ShiftCtrlBackSpaceKeyActionCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void ShiftCtrlBackSpaceKeyActionCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}


//SetRedone(다시 실행이라고 설정함)
void ShiftCtrlBackSpaceKeyActionCommand::SetRedone()
{
	this->isRedone = true;
}

//줄의 위치 구하기
Long ShiftCtrlBackSpaceKeyActionCommand::GetRowIndex()
{
	return this->rowIndex;
}
//글자 위치 구하기
Long ShiftCtrlBackSpaceKeyActionCommand::GetLetterIndex()
{
	return this->letterIndex;
}
//선택이 시작되는 글자의 위치구하기
Long ShiftCtrlBackSpaceKeyActionCommand::GetSelectedStartXPos()
{
	return this->selectedStartXPos;
}

//선택이 시작되는 줄의 위치구하기
Long ShiftCtrlBackSpaceKeyActionCommand::GetSelectedStartYPos()
{
	return this->selectedStartYPos;
}
//실행취소 종료지점여부 구하기
bool ShiftCtrlBackSpaceKeyActionCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//다시실행 종료지점여부 구하기 
bool ShiftCtrlBackSpaceKeyActionCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}
//다시실행인지 여부 구하기
bool ShiftCtrlBackSpaceKeyActionCommand::IsRedone()
{
	return this->isRedone;
}

//변경사항이 있는지 확인 여부
bool ShiftCtrlBackSpaceKeyActionCommand::IsDirty()
{
	return this->isDirty;
}

//선택영역이 지워졌는지 확인 여부
bool ShiftCtrlBackSpaceKeyActionCommand::IsSelectedTextsRemoved()
{
	return this->isSelectedTextsRemoved;
}

//소멸자 정의
ShiftCtrlBackSpaceKeyActionCommand::~ShiftCtrlBackSpaceKeyActionCommand()
{
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}