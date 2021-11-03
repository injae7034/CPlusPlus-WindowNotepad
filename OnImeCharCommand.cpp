#include "OnImeCharCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"
#include "RowAutoChange.h"
#include "SelectingTexts.h"

//디폴트생성자 정의
OnImeCharCommand::OnImeCharCommand(NotepadForm* notepadForm, Glyph* glyph)
	:Command(notepadForm)
{
	this->note = notepadForm->GetRemovedSelectedTexts();
	this->glyph = glyph;
	this->startYPos = notepadForm->note->GetCurrent();
	this->startXPos = notepadForm->current->GetCurrent();
	this->selectedEndYPos = 0;
	this->selectedEndXPos = 0;
	this->pastingEndYPos = 0;
	this->pastingEndXPos = 0;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
	this->isSelectedTextsRemoved = false;//처음에 생성될 때는 선택영역이 안지워졌으므로 false가 디폴트값
}

//실행
void OnImeCharCommand::Execute()
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

	//처음실행이 아니면
	if (this->isRedone == true)
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
	}

	//2. OnImeCharCommand가 다시 실행되면
	if (this->isRedone == true)
	{
		//2.1 한글을 입력할 때 메모장에서 선택된 영역을 지웠으면
		if (this->note != 0)
		{
			//2.1.1 자동개행이 진행 중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치가
			//저장되어 있음)
			if (this->notepadForm->isRowAutoChanging == true)
			{
				//2.1.1.1 변경된 화면 크기에 맞는 현재 줄과 글자 위치를 구한다.
				changedRowPos = 0;
				changedLetterPos = 0;
				originRowPos = this->selectedEndYPos;
				originLetterPos = this->selectedEndXPos;
				rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
					&changedRowPos);
				//2.1.1.2 자동개행을 반영해서 현재 줄의 위치와 글자 위치를 다시 조정한다.
				currentRowPos = this->notepadForm->note->Move(changedRowPos);
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
				//2.1.1.3 자동개행을 반영해서 선택이 끝나는 줄의 위치와 글자 위치를 다시 조정한다.
				selectedEndRowPos = currentRowPos;
				selectedEndLetterPos = currentLetterPos;
				//2.1.1.4 변경된 화면 크기에 맞는 선택이 시작되는 줄과 글자 위치를 구한다.
				changedRowPos = 0;
				changedLetterPos = 0;
				originRowPos = this->startYPos;
				originLetterPos = this->startXPos;
				rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
					&changedRowPos);
				//2.1.1.5 자동개행을 반영해서 선택이 시작되는 줄과 글자 위치를 다시 조정한다.
				selectedStartRowPos = changedRowPos;//선택이 시작되는 줄
				selectedStartLetterPos = changedLetterPos;//선택이 시작되는 글자
			}
			//2.1.2 자동개행 진행 중이 아니면
			else
			{
				//2.1.2.1 현재 줄의 위치와 글자 위치를 다시 조정한다.
				currentRowPos = this->notepadForm->note->Move(this->selectedEndYPos);
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->Move(this->selectedEndXPos);
				//2.1.2.2 선택이 시작되는 줄의 위치와 글자 위치를 다시 조정한다.
				selectedStartRowPos = this->startYPos;
				selectedStartLetterPos = this->startXPos;
				//2.1.2.3 선택이 끝나는 줄의 위치와 글자 위치를 다시 조정한다.
				selectedEndRowPos = currentRowPos;
				selectedEndLetterPos = currentLetterPos;
			}
			//2.1.3 선택된 단어(찾은 단어)를 지운다.
			this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
				selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
			//2.1.4 연산이 끝났기 때문에 현재 줄의 위치를 다시 조정해준다.(note의 연산 안에서 현재 줄의 위치와 글자 
			//위치는 조정이 되지만 notepadForm의 current(현재줄)는 조정할 수 없어서 notepadForm에서 해준다.)
			currentRowPos = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->GetCurrent();
			//2.1.5 선택영역이 삭제되었기 때문에 자동개행이 끝나고 나면 줄의 위치와 글자 위치가 바뀔 수
			//있기 때문에 시작하는 줄의 위치를 현재 줄의 위치와 글자위치로 재갱신해줘야한다.
			this->startYPos = this->notepadForm->note->GetCurrent();
			this->startXPos = this->notepadForm->current->GetCurrent();
			//2.1.6 자동 줄 바꿈 메뉴가 체크되어 있으면(선택영역이 삭제 됬기 때문에 자동개행을 다시해줘야함)
			if (this->notepadForm->isRowAutoChanging == true)
			{
				//2.1.6.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
				//OnSize에서 부분자동개행을 하도록 한다. 
				this->notepadForm->SendMessage(WM_SIZE);
				//2.1.6.2 선택영역이 삭제되었기 때문에 자동개행이 끝나고 나면 줄의 위치와 글자 위치가
				//바뀔 수 있기 때문에 시작하는 줄의 위치를 현재 줄의 위치와 글자위치로 재갱신해줘야한다.
				this->startYPos = this->notepadForm->note->GetCurrent();
				this->startXPos = this->notepadForm->current->GetCurrent();
				//2.1.6.3 지금 현재 startYPos와 startXPos의 위치는 자동개행이 적용된 상태의 위치이기
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
			//2.1.7 메모장에서 선택된 texts를 다 지웠기 때문에 메모장에서 선택이 안된 상태로 바꾼다.
			this->notepadForm->isSelecting = false;
			//2.1.8 선택된 texts를 지웠기 때문에 command가 선택된 영역을 지웠다고 표시한다.
			this->isSelectedTextsRemoved = true;
			//2.1.9 선택이 끝났기 때문에 캐럿의 x좌표를 0으로 저장한다.
			this->notepadForm->selectedStartXPos = 0;
			//2.1.10 선택이 끝났기 때문에 캐럿의 y좌표를 0으로 저장한다.
			this->notepadForm->selectedStartYPos = 0;
			//2.1.11 복사하기, 잘라내기, 삭제 메뉴를 비활성화 시킨다.
			this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		}
		//2.2 한글을 입력할 때 메모장에서 선택영역을 안지웠으면
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
	}
	//3. 처음 실행이면(앞에 조립중인 한글을 지워야하고, 다시 실행이면 앞에 조립중인 한글이 없다!)
	else
	{
		//3.1 현재 줄의 위치와 글자 위치를 구한다.
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//3.2 현재 줄의 캐럿의 가로 위치 바로 앞에 있는 기존 한글을 지운다.
		//그러기 위해서는 캐럿의 현재 가로 위치에 1감소한 값을 넣어주면 된다.
		//미완성된 한글을 지우고 바로 다시 완성된 한글을 넣기 때문에 미완성된 한글을 지울 때는
		//자동개행을 별도로 해줄 필요가 없고, 완성된 한글을 넣고 나서도 자동개행을 따로 해줄 필요가 없다
		//왜냐하면 미완성 글자 위치를 완성글자로 바꿔줬으니까 줄의 길이에는 변동이 없기 때문에!
		this->notepadForm->current->Remove(currentLetterPos - 1);
		//3.3 연산이 끝났기 때문에 현재 글자의 위치를 다시 구한다.
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//3.4 선택된 단어를 지웠기 때문에 다시 시작하는 줄의 위치와 글자위치를 갱신한다.
		this->startYPos = currentRowPos;
		this->startXPos = currentLetterPos;
		//3.5 자동 줄 바꿈 메뉴가 체크되어 있으면(선택영역이 삭제 됬기 때문에 자동개행을 다시해줘야함)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//3.5.1 지금 현재 startYPos와 startXPos의 위치는 자동개행이 적용된 상태의 위치이기
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
		//3.6 한글을 입력할 때 메모장에서 선택된 영역을 지웠으면
		if (this->note != 0)
		{
			//3.6.1 선택된 texts를 지웠기 때문에 command가 선택된 영역을 지웠다고 표시한다.
			this->isSelectedTextsRemoved = true;
		}
	}
	//4. currentRowPos가 현재 줄의 length와 같으면
	if (currentLetterPos == this->notepadForm->current->GetLength())
	{
		//4.1 현재 줄의 마지막 글자 뒤에 새로운 한글을 추가한다.
		currentLetterPos = this->notepadForm->current->Add(this->glyph->Clone());
	}
	//5. currentRowPos가 현재 줄의 length와 다르면
	else
	{
		//5.1 현재 줄의 currentRowPos번째에 새로운 한글을 끼워 쓴다.
		currentLetterPos = this->notepadForm->current->Add(currentLetterPos, 
			this->glyph->Clone());
	}
	//6. 메모장의 현재 글자 위치를 다시 구한다.
	currentLetterPos = this->notepadForm->current->GetCurrent();
	//7. 메모장에 붙여넣었기 때문에 붙여넣기가 끝나는 줄과 글자위치를 갱신한다.
	this->pastingEndYPos = currentRowPos;
	this->pastingEndXPos = currentLetterPos;
	//8. 자동개행이 진행중이면
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//8.1 글자가 추가된 줄을 자동개행시켜준다.
		//this->notepadForm->SendMessage(WM_SIZE);
		//8.2 자동개행 후 메모장의 현재 줄을 저장한다.
		//currentRowPos = this->notepadForm->note->GetCurrent();
		//this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		//currentLetterPos = this->notepadForm->current->GetCurrent();
		//8.3 메모장에 선택영역이 복원되었고, 자동개행인 반영된 선택이 끝나는 줄과 글자위치를 저장한다. 
		//this->pastingEndYPos = currentRowPos;
		//this->pastingEndXPos = currentLetterPos;
		//8.4 지금 현재 선택이 끝나는 줄과 글자 위치는 자동개행이 적용된 상태의 위치이기 때문에
		//자동개행을 안한 상태에서 선택하기가 끝나는 진짜 줄의 위치와 글자위치를 구한다.
		changedRowPos = this->pastingEndYPos;
		changedLetterPos = this->pastingEndXPos;
		originRowPos = 0;
		originLetterPos = 0;
		rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
			&originRowPos);
		this->pastingEndYPos = originRowPos;
		this->pastingEndXPos = originLetterPos;
	}
	//9. 처음 실행이면
	if (this->isRedone == false)
	{
		//9.1 IsComposing을 false로 바꾼다.
		this->notepadForm->isComposing = false;
		//9.2 메모장 제목에 *를 추가한다.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - 메모장";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//9.3 메모장에 변경사항이 있음을 저장한다.
		this->notepadForm->isDirty = true;
	}
}

//실행취소
void OnImeCharCommand::Unexecute()
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
		//3.1 자동개행을 반영해서 붙여넣기가 끝나는(현재) 줄과 글자 위치를 구한다.
		changedRowPos = 0;
		changedLetterPos = 0;
		originRowPos = this->pastingEndYPos;
		originLetterPos = this->pastingEndXPos;
		rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
			&changedRowPos);
		//3.2 자동개행을 반영해서 현재 줄의 위치와 글자 위치를 다시 조정한다.
		currentRowPos = this->notepadForm->note->Move(changedRowPos);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
		//3.3 자동개행을 반영해서 붙여넣기가 끝나는 줄의 위치와 글자 위치를 다시 조정한다.
		selectedEndRowPos = currentRowPos;
		selectedEndLetterPos = currentLetterPos;
		//3.4 자동개행을 반영해서 붙여넣기가 시작되는 줄과 글자 위치를 구한다.
		changedRowPos = 0;
		changedLetterPos = 0;
		originRowPos = this->startYPos;
		originLetterPos = this->startXPos;
		rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
			&changedRowPos);
		//3.5 자동개행을 반영해서 붙여넣기가 시작되는 줄과 글자 위치를 다시 조정한다.
		selectedStartRowPos = changedRowPos;//선택이 시작되는 줄
		selectedStartLetterPos = changedLetterPos;//선택이 시작되는 글자
	}
	//4. 자동개행 진행 중이 아니면
	else
	{
		//4.1 현재 줄의 위치와 글자 위치를 다시 조정한다.
		currentRowPos = this->notepadForm->note->Move(this->pastingEndYPos);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(this->pastingEndXPos);
		//4.2 붙여넣기가 시작되는 줄의 위치와 글자 위치를 다시 조정한다.
		selectedStartRowPos = this->startYPos;
		selectedStartLetterPos = this->startXPos;
		//4.3 붙여넣기가 끝나는 줄의 위치와 글자 위치를 다시 조정한다.
		selectedEndRowPos = currentRowPos;
		selectedEndLetterPos = currentLetterPos;
	}
	//5. 현재 글자를 지운다.
	this->notepadForm->current->Remove(currentLetterPos - 1);
	//8. 연산이 끝났기 때문에 현재 줄의 위치를 다시 조정해준다.(note의연산안에서 현재 줄의 위치와 글자 위치는
	//조정이 되지만 notepadForm의 current(현재줄)는 조정할 수 없어서 notepadForm에서 해준다.)
	currentRowPos = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	currentLetterPos = this->notepadForm->current->GetCurrent();
	//9. 선택영역이 삭제되었기 때문에 자동개행이 끝나고 나면 줄의 위치와 글자 위치가 바뀔 수
	//있기 때문에 시작하는 줄의 위치를 현재 줄의 위치와 글자위치로 재갱신해줘야한다.
	this->startYPos = this->notepadForm->note->GetCurrent();
	this->startXPos = this->notepadForm->current->GetCurrent();
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
	//7. 한글을 입력할 때 메모장에서 선택된 영역을 지웠으면
	if (this->note != 0)
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
			InsertTexts(currentRowPos, currentLetterPos, this->note);
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
void OnImeCharCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void OnImeCharCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}


//SetRedone(다시 실행이라고 설정함)
void OnImeCharCommand::SetRedone()
{
	this->isRedone = true;
}

//시작하는 줄의 위치구하기
Long OnImeCharCommand::GetStartYPos()
{
	return this->startYPos;
}
//시작하는 글자의 위치구하기
Long OnImeCharCommand::GetStartXPos()
{
	return this->startXPos;
}
//선택이 끝나는 줄의 위치 구하기
Long OnImeCharCommand::GetSelectedEndYPos()
{
	return this->selectedEndYPos;
}
//선택이 끝나는 글자 위치 구하기
Long OnImeCharCommand::GetSelectedEndXPos()
{
	return this->selectedEndXPos;
}
//붙여넣기가 끝나는 줄의 위치구하기
Long OnImeCharCommand::GetPastingEndYPos()
{
	return this->pastingEndYPos;
}
//붙여넣기가 끝나는 글자의 위치구하기
Long OnImeCharCommand::GetPastingEndXPos()
{
	return this->pastingEndXPos;
}
//실행취소 종료지점여부 구하기
bool OnImeCharCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//다시실행 종료지점여부 구하기 
bool OnImeCharCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}
//다시실행인지 여부 구하기
bool OnImeCharCommand::IsRedone()
{
	return this->isRedone;
}
//선택영역이 지워졌는지 확인 여부
bool OnImeCharCommand::IsSelectedTextsRemoved()
{
	return this->isSelectedTextsRemoved;
}


Glyph* OnImeCharCommand::GetGlyph()
{
	return this->glyph;
}

Glyph* OnImeCharCommand::GetNote()
{
	return this->note;
}

//소멸자 정의
OnImeCharCommand::~OnImeCharCommand()
{
	if (this->note != 0)
	{
		delete this->note;
	}
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}