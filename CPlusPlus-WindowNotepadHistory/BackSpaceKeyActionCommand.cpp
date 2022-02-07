#include "BackSpaceKeyActionCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"
#include "Row.h"
#include "RowAutoChange.h"
#include "DummyRow.h"
#include "Note.h"
#include "SelectingTexts.h"

//디폴트생성자 정의
BackSpaceKeyActionCommand::BackSpaceKeyActionCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{
	this->startYPos = notepadForm->note->GetCurrent();
	this->startXPos = notepadForm->current->GetCurrent();
	this->selectedEndYPos = 0;
	this->selectedEndXPos = 0;
	this->glyph = 0;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
	this->isDirty = false;//처음에 생성될 때는 변경사항이 없으므로 false가 디폴트값임.
	this->isSelectedTextsRemoved = false;//처음에 생성될 때는 선택영역이 안지워졌으므로 false가 디폴트값
}

//실행
void BackSpaceKeyActionCommand::Execute()
{
	//1. RowAutoChange를 생성한다.
	RowAutoChange rowAutoChange(this->notepadForm);

	Long changedRowPos = 0;//자동개행 후 줄 위치
	Long changedLetterPos = 0;//자동개행 후 글자 위치
	Long originRowPos = 0;//자동개행 전 줄 위치
	Long originLetterPos = 0;//자동개행 전 글자 위치
	Long currentRowPos = 0;//현재 줄의 위치
	Long currentLetterPos = 0;//현재 글자의 위치
	Long selectedStartRowPos = 0;//선택이 시작되는 줄 위치
	Long selectedStartLetterPos = 0;//선택이 시작되는 글자 위치
	Long selectedEndRowPos = 0;//선택이 끝나는 줄 위치
	Long selectedEndLetterPos = 0;//선택이 끝나는 글자 위치

	//2. 메모장에서 선택된 texts가 없고, 선택된 영역을 안지웠으면
	if (this->notepadForm->isSelecting == false && this->isSelectedTextsRemoved == false)
	{
		//2.1 처음 실행이면
		if (this->isRedone == false)
		{
			//2.1.1 현재 줄의 위치와 글자 위치를 구한다.
			currentRowPos = this->notepadForm->note->GetCurrent();
			currentLetterPos = this->notepadForm->current->GetCurrent();
			//2.1.2 현재 줄의 위치와 글자위치를 시작하는 줄과 글자 위치로 저장한다.
			this->startYPos = currentRowPos;
			this->startXPos = currentLetterPos;
			//2.1.3 자동 줄 바꿈 메뉴가 체크되어 있으면(진짜 줄의 위치와 글자위치로 저장한다.)
			if (this->notepadForm->isRowAutoChanging == true)
			{
				//2.1.3.1 지금 현재 startYPos와 startXPos의 위치는 자동개행이 적용된 상태의 위치이기
				//때문에 자동개행을 안한 상태에서 선택하기가 시작되는 진짜 줄의 위치와 글자위치를 구한다.
				changedRowPos = this->startYPos;
				changedLetterPos = this->startXPos;
				originRowPos = 0;
				originLetterPos = 0;
				rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
					&originRowPos);
				this->startYPos = originRowPos;
				this->startXPos = originLetterPos;
			}
		}
		//2.2 다시 실행이면
		else
		{
			//2.2.1 자동개행이 진행중이면(command의 시작하는 줄과 글자 위치는 
			//항상 진짜 줄과 글자 위치가 저장되어 있기 때문에 자동개행에 맞게 바꿔줘야함)
			if (this->notepadForm->isRowAutoChanging == true)
			{
				//2.2.1.1 자동개행을 반영하여 시작되는 줄과 글자 위치를 구한다.
				changedRowPos = 0;
				changedLetterPos = 0;
				originRowPos = this->startYPos;
				originLetterPos = this->startXPos;
				rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
					&changedRowPos);
				//2.2.1.2 자동개행을 반영해서 현재 줄의 위치와 글자 위치를 다시 조정한다.
				currentRowPos = this->notepadForm->note->Move(changedRowPos);
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
			}
			//2.2.2 자동개행 진행중이 아니면
			else
			{
				//2.2.2.1 현재 줄의 위치와 글자 위치를 다시 조정한다.
				currentRowPos = this->notepadForm->note->Move(this->startYPos);
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->Move(this->startXPos);
			}
		}
		//2.3 현재 줄을 구한다.
		Glyph* currentRow = this->notepadForm->note->GetAt(currentRowPos);
		Glyph* letter = 0;
		//2.4 현재 줄의 위치가 0이고, 현재 글자 위치가 0이면
		if (currentRowPos == 0 && currentLetterPos == 0)
		{
			//2.4.1 처음 실행이면
			if (this->isRedone == false)
			{
				//2.4.1.1 Command에 변경 사항이 없음을 표시한다.
				this->isDirty = false;
			}
		}
		//2.5 현재 줄의 위치가 0보다 크고, 현재 글자 위치가 0이면
		else if (currentRowPos > 0 && currentLetterPos == 0)
		{
			//2.5.1 현재 줄이 진짜 줄이면
			if (!dynamic_cast<DummyRow*>(currentRow))
			{
				//2.5.1.1 처음 실행이면
				if (this->isRedone == false)
				{
					//2.5.1.1.1 Row를 생성한다.
					this->glyph = new Row();
				}
				//2.5.1.2 현재 줄의 이전 줄을 구한다.
				Glyph* previousRow = this->notepadForm->note->GetAt(currentRowPos - 1);
				//2.5.1.3 현재 줄의 이전 줄의 마지막 글자 위치를 구한다.
				Long letterPos = previousRow->GetLength();
				//2.5.1.4 현재 줄을 이전 줄에 합친다.
				currentRow->Join(previousRow);
				//2.5.1.5 Note에서 현재 줄의 주소를 지운다.
				this->notepadForm->note->Remove(currentRowPos);
				//2.5.1.6 현재 줄이 지워졌기 때문에 현재 줄을 변경한다.
				currentRowPos = this->notepadForm->note->GetCurrent();
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				//2.5.1.7 현재 줄의 글자 위치가 지금은 마지막이기 때문에 변경해준다.
				//이전 줄의 마지막 현재 줄의 처음 사이에 위치하도록 조정한다.
				currentLetterPos = this->notepadForm->current->Move(letterPos);
			}
			//2.5.2 현재 줄이 가짜줄이면
			else
			{
				//2.5.2.1 이전 줄로 이동한다.
				currentRowPos = this->notepadForm->note->Move(currentRowPos - 1);
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				//2.5.2.2 처음 실행이 되면
				if (this->isRedone == false)
				{
					//2.5.2.2.1 지울 글자를 구한다.
					letter = this->notepadForm->current->
						GetAt(this->notepadForm->current->GetLength() - 1);
					//2.5.2.2.2 지울 글자를 깊은 복사해서 저장한다.
					this->glyph = letter->Clone();
				}
				//2.5.2.3 이전 줄의 마지막 글자를 지운다.
				this->notepadForm->current->Remove(this->notepadForm->current->GetLength() - 1);
			}
			///2.5.3 Command에 변경 사항이 있음을 표시한다.
			this->isDirty = true;
		}
		//2.6 그 이외에는
		else
		{
			//2.6.1 처음 실행이 되면
			if (this->isRedone == false)
			{
				//2.6.1.1 지울 글자를 구한다.
				letter = this->notepadForm->current->GetAt(currentLetterPos - 1);
				//2.6.1.2 지울 글자를 깊은 복사해서 저장한다.
				this->glyph = letter->Clone();
			}
			//2.6.2 현재 글자를 지운다.
			this->notepadForm->current->Remove(currentLetterPos - 1);
			///2.6.4 Command에 변경 사항이 있음을 표시한다.
			this->isDirty = true;
		}
		//2.7 변경사항이 있으면
		if (this->isDirty == true)
		{
			//2.7.1 연산이 끝났기 때문에 현재 줄의 위치를 다시 조정해준다.(note의 연산 안에서 
			//현재 줄의 위치와 글자 위치는 조정이 되지만 notepadForm의 current(현재줄)는 
			//조정할 수 없어서 notepadForm에서 해준다.)
			currentRowPos = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->GetCurrent();
			//2.7.2 글자나 줄을 지웠기 때문에 다시 시작하는 줄의 위치와 글자위치를 갱신한다.
			this->startYPos = currentRowPos;
			this->startXPos = currentLetterPos;
			//2.7.3 자동 줄 바꿈 메뉴가 체크되어 있으면(선택영역이 삭제 됬기 때문에 자동개행을 다시해줘야함)
			if (this->notepadForm->isRowAutoChanging == true)
			{
				//2.7.3.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
				//OnSize에서 부분자동개행을 하도록 한다. 
				this->notepadForm->SendMessage(WM_SIZE);
				//2.7.3.2 선택영역이 삭제되었기 때문에 자동개행이 끝나고 나면 줄의 위치와 글자 위치가
				//바뀔 수있기 때문에 시작하는 줄의 위치를 현재 줄의 위치와 글자위치로 재갱신해줘야한다.
				this->startYPos = this->notepadForm->note->GetCurrent();
				this->startXPos = this->notepadForm->current->GetCurrent();
				//2.7.3.3 지금 현재 startYPos와 startXPos의 위치는 자동개행이 적용된 상태의 위치이기
				//때문에 자동개행을 안한 상태에서 선택하기가 시작되는 진짜 줄의 위치와 글자위치를 구한다.
				changedRowPos = this->startYPos;
				changedLetterPos = this->startXPos;
				originRowPos = 0;
				originLetterPos = 0;
				rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
					&originRowPos);
				this->startYPos = originRowPos;
				this->startXPos = originLetterPos;
			}
		}
	}
	//3. 메모장에서 선택된 texts가 있거나 또는 선택된 영역을 지웠으면
	else if (this->notepadForm->isSelecting == true || this->isSelectedTextsRemoved == true)
	{
		//3.1 OnCharCommand가 처음 실행되면
		if (this->isRedone == false)
		{
			//3.1.1 현재 줄의 위치와 글자 위치를 구한다.
			currentRowPos = this->notepadForm->note->GetCurrent();
			currentLetterPos = this->notepadForm->current->GetCurrent();
			//3.1.2 선택이 시작되는 줄의 위치와 글자 위치를 구한다.
			selectedStartRowPos = this->notepadForm->selectedStartYPos;
			this->startYPos = selectedStartRowPos;
			selectedStartLetterPos = this->notepadForm->selectedStartXPos;
			this->startXPos = selectedStartLetterPos;
			//3.1.3 선택이 끝나는 줄의 위치와 글자 위치를 구한다.
			selectedEndRowPos = currentRowPos;
			this->selectedEndYPos = selectedEndRowPos;
			selectedEndLetterPos = currentLetterPos;
			this->selectedEndXPos = selectedEndLetterPos;
			//3.1.4 선택된 단어(찾은 단어)를 command에 복사해서 저장하고 지운다.
			this->glyph = this->notepadForm->note->CopySelectedTextsAndRemove(selectedStartRowPos,
				selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
		}
		//3.2 OnCharCommand가 다시 실행되면
		else
		{
			//3.2.1 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치가
			//저장되어 있음)
			if (this->notepadForm->isRowAutoChanging == true)
			{
				//3.2.1.1 변경된 화면 크기에 맞는 현재 줄과 글자 위치를 구한다.
				changedRowPos = 0;
				changedLetterPos = 0;
				originRowPos = this->selectedEndYPos;
				originLetterPos = this->selectedEndXPos;
				rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
					&changedRowPos);
				//3.2.1.2 자동개행을 반영해서 현재 줄의 위치와 글자 위치를 다시 조정한다.
				currentRowPos = this->notepadForm->note->Move(changedRowPos);
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
				//3.2.1.3 자동개행을 반영해서 선택이 끝나는 줄의 위치와 글자 위치를 다시 조정한다.
				selectedEndRowPos = currentRowPos;
				selectedEndLetterPos = currentLetterPos;
				//3.2.1.4 변경된 화면 크기에 맞는 선택이 시작되는 줄과 글자 위치를 구한다.
				changedRowPos = 0;
				changedLetterPos = 0;
				originRowPos = this->startYPos;
				originLetterPos = this->startXPos;
				rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
					&changedRowPos);
				//3.2.1.5 자동개행을 반영해서 선택이 시작되는 줄과 글자 위치를 다시 조정한다.
				selectedStartRowPos = changedRowPos;//선택이 시작되는 줄
				selectedStartLetterPos = changedLetterPos;//선택이 시작되는 글자
			}
			//3.2.2 자동개행 진행 중이 아니면
			else
			{
				//3.2.2.1 현재 줄의 위치와 글자 위치를 다시 조정한다.
				currentRowPos = this->notepadForm->note->Move(this->selectedEndYPos);
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->Move(this->selectedEndXPos);
				//3.2.2.2 선택이 시작되는 줄의 위치와 글자 위치를 다시 조정한다.
				selectedStartRowPos = this->startYPos;
				selectedStartLetterPos = this->startXPos;
				//3.2.2.3 선택이 끝나는 줄의 위치와 글자 위치를 다시 조정한다.
				selectedEndRowPos = currentRowPos;
				selectedEndLetterPos = currentLetterPos;
			}
			//3.2.3 선택된 단어(찾은 단어)를 지운다.
			this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
				selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
		}
		//3.3 연산이 끝났기 때문에 현재 줄의 위치를 다시 조정해준다.(note의 연산 안에서 
		//현재 줄의 위치와 글자 위치는 조정이 되지만 notepadForm의 current(현재줄)는 
		//조정할 수 없어서 notepadForm에서 해준다.)
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//3.4 선택된 단어를 지웠기 때문에 다시 시작하는 줄의 위치와 글자위치를 갱신한다.
		this->startYPos = currentRowPos;
		this->startXPos = currentLetterPos;
		//3.5 자동 줄 바꿈 메뉴가 체크되어 있으면(선택영역이 삭제 됬기 때문에 자동개행을 다시해줘야함)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//3.5.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
			//OnSize에서 부분자동개행을 하도록 한다. 
			this->notepadForm->SendMessage(WM_SIZE);
			//3.5.2 선택영역이 삭제되었기 때문에 자동개행이 끝나고 나면 줄의 위치와 글자 위치가
			//바뀔 수있기 때문에 시작하는 줄의 위치를 현재 줄의 위치와 글자위치로 재갱신해줘야한다.
			this->startYPos = this->notepadForm->note->GetCurrent();
			this->startXPos = this->notepadForm->current->GetCurrent();
			//3.5.3 지금 현재 startYPos와 startXPos의 위치는 자동개행이 적용된 상태의 위치이기
			//때문에 자동개행을 안한 상태에서 선택하기가 시작되는 진짜 줄의 위치와 글자위치를 구한다.
			changedRowPos = this->startYPos;
			changedLetterPos = this->startXPos;
			originRowPos = 0;
			originLetterPos = 0;
			rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
				&originRowPos);
			this->startYPos = originRowPos;
			this->startXPos = originLetterPos;
		}
		//3.6 메모장에서 선택된 단어를 지웠기 때문에 메모장에서 선택이 안된 상태로 바꾼다.
		this->notepadForm->isSelecting = false;
		//3.7 선택이 끝났기 때문에 선택이 시작되는 x좌표를 0으로 저장한다.
		this->notepadForm->selectedStartXPos = 0;
		//3.8 선택이 끝났기 때문에 선택이 시작되는 y좌표를 0으로 저장한다.
		this->notepadForm->selectedStartYPos = 0;
		//3.9 선택된 영역이 없어졌기 때문에 복사하기, 잘라내기, 삭제 메뉴를 비활성화 시킨다.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		//3.10 command가 선택영역을 지웠다고 표시한다.
		this->isSelectedTextsRemoved = true;
		//3.11 Command에 변경 사항이 있음을 표시한다.
		this->isDirty = true;
	}
	//4. Command에 변경 사항이 있으면
	if (this->isDirty == true)
	{
		//4.1 메모장 제목에 *를 추가한다.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - 메모장";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//4.2 메모장에 변경사항이 있음을 저장한다.
		this->notepadForm->isDirty = true;
	}
}

//실행취소
void BackSpaceKeyActionCommand::Unexecute()
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
	//2. RowAutoChange를 생성한다.
	RowAutoChange rowAutoChange(this->notepadForm);

	Long changedRowPos = 0;//자동개행 후 줄 위치
	Long changedLetterPos = 0;//자동개행 후 글자 위치
	Long originRowPos = 0;//자동개행 전 줄 위치
	Long originLetterPos = 0;//자동개행 전 글자 위치
	Long currentRowPos = 0;//현재 줄의 위치
	Long currentLetterPos = 0;//현재 글자의 위치
	Long selectedStartRowPos = 0;//선택이 시작되는 줄 위치
	Long selectedStartLetterPos = 0;//선택이 시작되는 글자 위치
	Long selectedEndRowPos = 0;//선택이 끝나는 줄 위치
	Long selectedEndLetterPos = 0;//선택이 끝나는 글자 위치

	//3. 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치가 저장되어 있음)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//3.1 자동개행을 반영해서 붙여넣기가 시작되는 줄과 글자 위치를 구한다.
		changedRowPos = 0;
		changedLetterPos = 0;
		originRowPos = this->startYPos;
		originLetterPos = this->startXPos;
		rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
			&changedRowPos);
		//3.2 자동개행을 반영해서 현재 줄의 위치와 글자 위치를 다시 조정한다.
		currentRowPos = this->notepadForm->note->Move(changedRowPos);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
	}
	//4. 자동개행 진행 중이 아니면
	else
	{
		//4.1 현재 줄의 위치와 글자 위치를 다시 조정한다.
		currentRowPos = this->notepadForm->note->Move(this->startYPos);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(this->startXPos);
	}
	//5. 지울 때 저장한 glyph가 줄(개행문자)이 아니고 선택영역(Note)이 아니면
	if (!dynamic_cast<Row*>(this->glyph) && !dynamic_cast<Note*>(this->glyph))
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
		//8. 연산이 끝났기 때문에 현재 줄의 위치를 다시 조정해준다.(note의연산안에서 현재 줄의 위치와 글자 위치는
		//조정이 되지만 notepadForm의 current(현재줄)는 조정할 수 없어서 notepadForm에서 해준다.)
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//9. 선택영역이 삭제되었기 때문에 자동개행이 끝나고 나면 줄의 위치와 글자 위치가 바뀔 수
		//있기 때문에 시작하는 줄의 위치를 현재 줄의 위치와 글자위치로 재갱신해줘야한다.
		this->startYPos = currentRowPos;
		this->startXPos = currentLetterPos;
		//10. 자동 줄 바꿈 메뉴가 체크되어 있으면
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//10.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
			//OnSize에서 부분자동개행을 하도록 한다. 
			this->notepadForm->SendMessage(WM_SIZE);
			//10.2 선택영역이 삭제되었기 때문에 자동개행이 끝나고 나면 줄의 위치와 글자 위치가 바뀔 수
			//있기 때문에 시작하는 줄의 위치를 현재 줄의 위치와 글자위치로 재갱신해줘야한다.
			this->startYPos = this->notepadForm->note->GetCurrent();
			this->startXPos = this->notepadForm->current->GetCurrent();
			//10.3 지금 현재 startYPos와 startXPos의 위치는 자동개행이 적용된 상태의 위치이기 때문에
			//자동개행을 안한 상태에서 선택하기가 시작되는 진짜 줄의 위치와 글자위치를 구한다.
			changedRowPos = this->startYPos;
			changedLetterPos = this->startXPos;
			originRowPos = 0;
			originLetterPos = 0;
			rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
				&originRowPos);
			this->startYPos = originRowPos;
			this->startXPos = originLetterPos;
		}
	}
	//6. 지울 때 저장한 glyph가 줄(개행문자)이면
	else if(dynamic_cast<Row*>(this->glyph))
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
		//8. 연산이 끝났기 때문에 현재 줄의 위치를 다시 조정해준다.(note의연산안에서 현재 줄의 위치와 글자 위치는
		//조정이 되지만 notepadForm의 current(현재줄)는 조정할 수 없어서 notepadForm에서 해준다.)
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//9. 선택영역이 삭제되었기 때문에 자동개행이 끝나고 나면 줄의 위치와 글자 위치가 바뀔 수
		//있기 때문에 시작하는 줄의 위치를 현재 줄의 위치와 글자위치로 재갱신해줘야한다.
		this->startYPos = currentRowPos;
		this->startXPos = currentLetterPos;
		//10. 자동 줄 바꿈 메뉴가 체크되어 있으면
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//10.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
			//OnSize에서 부분자동개행을 하도록 한다. 
			this->notepadForm->SendMessage(WM_SIZE);
			//10.2 선택영역이 삭제되었기 때문에 자동개행이 끝나고 나면 줄의 위치와 글자 위치가 바뀔 수
			//있기 때문에 시작하는 줄의 위치를 현재 줄의 위치와 글자위치로 재갱신해줘야한다.
			this->startYPos = this->notepadForm->note->GetCurrent();
			this->startXPos = this->notepadForm->current->GetCurrent();
			//10.3 지금 현재 startYPos와 startXPos의 위치는 자동개행이 적용된 상태의 위치이기 때문에
			//자동개행을 안한 상태에서 선택하기가 시작되는 진짜 줄의 위치와 글자위치를 구한다.
			changedRowPos = this->startYPos;
			changedLetterPos = this->startXPos;
			originRowPos = 0;
			originLetterPos = 0;
			rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
				&originRowPos);
			this->startYPos = originRowPos;
			this->startXPos = originLetterPos;
		}
	}
	//8. 지울 때 저장한 glyph가 노트(선택영역)이면
	else if (dynamic_cast<Note*>(this->glyph))
	{
		//11.1 바꾼단어를 지웠고 이제 선택영역을 다시 복원하기 때문에 현재 줄과 글자의 위치를
		//선택영역을 시작 줄과 글자 위치로 설정한다.
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		this->notepadForm->selectedStartYPos = currentRowPos;
		this->notepadForm->selectedStartXPos = currentLetterPos;
		//11.2 메모장에서 선택된 texts를 다시 복구하기 때문에 메모장에서 선택이 된 상태로 바꾼다.
		this->notepadForm->isSelecting = true;
		//11.3 선택영역이 다시 생겼기 때문에 복사하기, 잘라내기, 삭제 메뉴를 활성화 시킨다.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
		//11.4 선택영역을 다시 복원하기 전에 줄의 위치를 저장한다.
		Long previousRowIndex = currentRowPos;
		//11.5 선택영역을 다시 복원하고 현재 줄의 위치를 반환받는다.
		currentRowPos = this->notepadForm->note->
			InsertTexts(currentRowPos, currentLetterPos, this->glyph);
		//11.6 메모장의 현재 줄을 저장한다.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//11.7 메모장에 선택영역이 복원되었기 때문에 선택이 끝나는 줄과 글자위치를 갱신한다.
		this->selectedEndYPos = currentRowPos;
		this->selectedEndXPos = currentLetterPos;
		//11.8 자동개행이 진행중이면 붙여넣은 줄들을 자동개행시켜준다.
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//11.8.1 부분자동개행을 한다.(마지막 줄은 제외하고 자동개행함)
			Long endPastedRowPos = rowAutoChange.DoPartRows(previousRowIndex, currentRowPos);
			//11.8.2 선택영역이 끝나는 줄로 이동시킨다.
			//선택영역이 끝나는 줄은 OnSize에서 부분자동개행을 해서 처리되기 때문에 캐럿의 위치만 조정해주면 됨!
			currentRowPos = this->notepadForm->note->Move(endPastedRowPos);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->GetCurrent();
			this->notepadForm->current->Move(currentLetterPos);
			//11.8.3 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
			//OnSize에서 부분자동개행을 하도록 한다. (마지막 줄을 자동개행시킴)
			this->notepadForm->SendMessage(WM_SIZE);
			//11.8.4 메모장의 현재 줄을 저장한다.
			currentRowPos = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->GetCurrent();
			//11.8.5 메모장에 선택영역이 복원되었고, 자동개행인 반영된 선택이 끝나는 줄과 글자위치를 저장한다. 
			this->selectedEndYPos = currentRowPos;
			this->selectedEndXPos = currentLetterPos;
			//11.8.6 지금 현재 선택이 끝나는 줄과 글자 위치는 자동개행이 적용된 상태의 위치이기 때문에
			//자동개행을 안한 상태에서 선택하기가 끝나는 진짜 줄의 위치와 글자위치를 구한다.
			changedRowPos = this->selectedEndYPos;
			changedLetterPos = this->selectedEndXPos;
			originRowPos = 0;
			originLetterPos = 0;
			rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
				&originRowPos);
			this->selectedEndYPos = originRowPos;
			this->selectedEndXPos = originLetterPos;
		}
	}
}

//SetMacroEnd(실행취소 및 다시실행 매크로출력 종료지점 설정)
void BackSpaceKeyActionCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}

void BackSpaceKeyActionCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}

//SetRedone(다시 실행이라고 설정함)
void BackSpaceKeyActionCommand::SetRedone()
{
	this->isRedone = true;
}

//시작하는 줄의 위치구하기
Long BackSpaceKeyActionCommand::GetStartYPos()
{
	return this->startYPos;
}
//시작하는 글자의 위치구하기
Long BackSpaceKeyActionCommand::GetStartXPos()
{
	return this->startXPos;
}
//선택이 끝나는 줄의 위치 구하기
Long BackSpaceKeyActionCommand::GetSelectedEndYPos()
{
	return this->selectedEndYPos;
}
//선택이 끝나는 글자 위치 구하기
Long BackSpaceKeyActionCommand::GetSelectedEndXPos()
{
	return this->selectedEndXPos;
}

//실행취소 종료지점여부 구하기
bool BackSpaceKeyActionCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}

//다시실행 종료지점여부 구하기 
bool BackSpaceKeyActionCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}

//다시실행인지 여부 구하기
bool BackSpaceKeyActionCommand::IsRedone()
{
	return this->isRedone;
}

//변경사항이 있는지 확인 여부
bool BackSpaceKeyActionCommand::IsDirty()
{
	return this->isDirty;
}

//선택영역이 지워졌는지 확인 여부
bool BackSpaceKeyActionCommand::IsSelectedTextsRemoved()
{
	return this->isSelectedTextsRemoved;
}

Glyph* BackSpaceKeyActionCommand::GetGlyph()
{
	return this->glyph;
}

//소멸자 정의
BackSpaceKeyActionCommand::~BackSpaceKeyActionCommand()
{
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}