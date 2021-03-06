#include "OnReplaceButtonClickedCommand.h"
#include "CommandHistory.h"
#include "Row.h"
#include "RowAutoChange.h"
#include "DummyRow.h"
#include "TextExtent.h"
#include "SelectingTexts.h"
#include "ReplacingDialog.h"

//디폴트생성자
OnReplaceButtonClickedCommand::OnReplaceButtonClickedCommand(NotepadForm* notepadForm)
	:Command(notepadForm), 
	replacingKeyword(((ReplacingDialog*)(this->notepadForm->findReplaceDialog))->replacingKeyword)
{
	this->note = 0;//선택된 texts가 있으면 지우기 전에 저장할 공간
	this->startYPos = notepadForm->selectedStartYPos;
	this->startXPos = notepadForm->selectedStartXPos;
	this->selectedEndYPos = notepadForm->note->GetCurrent();
	this->selectedEndXPos = notepadForm->current->GetCurrent();
	this->pastingEndYPos = 0;
	this->pastingEndXPos = 0;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
	this->isSelectedTextsRemoved = true;//무조건 선택영역을 지우기 때문에 default값이 true
}

//처음 및 다시 실행(선택영역이 항상 있는 상태에서 실행됨)
void OnReplaceButtonClickedCommand::Execute()
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

	//2. OnReplaceButtonClickedCommand가 처음 실행되면
	if (this->isRedone == false)
	{
		//2.1 현재 줄의 위치와 글자 위치를 구한다.
		currentRowPos = this->notepadForm->note->GetCurrent();
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//2.2 선택이 시작되는 줄의 위치와 글자 위치를 구한다.
		selectedStartRowPos = this->notepadForm->selectedStartYPos;
		this->startYPos = selectedStartRowPos;
		selectedStartLetterPos = this->notepadForm->selectedStartXPos;
		this->startXPos = selectedStartLetterPos;
		//2.3 선택이 끝나는 줄의 위치와 글자 위치를 구한다.
		selectedEndRowPos = currentRowPos;
		this->selectedEndYPos = selectedEndRowPos;
		selectedEndLetterPos = currentLetterPos;
		this->selectedEndXPos = selectedEndLetterPos;
		//2.4 선택된 단어(찾은 단어)를 command에 복사해서 저장하고 지운다.
		this->note = this->notepadForm->note->CopySelectedTextsAndRemove(selectedStartRowPos,
			selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
		//2.5 메모장 제목에 *를 추가한다.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - 메모장";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//2.6 메모장에 변경사항이 있음을 저장한다.
		this->notepadForm->isDirty = true;
		//2.7 연산이 끝났기 때문에 현재 줄의 위치를 다시 조정해준다.(note의 연산 안에서 현재 줄의 위치와 글자 
		//위치는 조정이 되지만 notepadForm의 current(현재줄)는 조정할 수 없어서 notepadForm에서 해준다.)
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//2.8 선택된 단어를 지웠기 때문에 다시 시작하는 줄의 위치와 글자위치를 갱신한다.
		this->startYPos = currentRowPos;
		this->startXPos = currentLetterPos;
		//2.9 자동 줄 바꿈 메뉴가 체크되어 있으면(선택영역이 삭제 됬기 때문에 자동개행을 다시해줘야함)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//2.9.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
			//OnSize에서 부분자동개행을 하도록 한다. 
			this->notepadForm->SendMessage(WM_SIZE);
			//2.9.2 선택영역이 삭제되었기 때문에 자동개행이 끝나고 나면 줄의 위치와 글자 위치가 바뀔 수
			//있기 때문에 시작하는 줄의 위치를 현재 줄의 위치와 글자위치로 재갱신해줘야한다.
			this->startYPos = this->notepadForm->note->GetCurrent();
			this->startXPos = this->notepadForm->current->GetCurrent();
			//2.9.3 지금 현재 startYPos와 startXPos의 위치는 자동개행이 적용된 상태의 위치이기 때문에
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
	//3. OnReplaceButtonClickedCommand가 다시 실행되면
	else
	{
		//3.1 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치가 저장되어 있음)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//3.1.1 변경된 화면 크기에 맞는 현재 줄과 글자 위치를 구한다.
			changedRowPos = 0;
			changedLetterPos = 0;
			originRowPos = this->selectedEndYPos;
			originLetterPos = this->selectedEndXPos;
			rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
				&changedRowPos);
			//3.1.2 자동개행을 반영해서 현재 줄의 위치와 글자 위치를 다시 조정한다.
			currentRowPos = this->notepadForm->note->Move(changedRowPos);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
			//3.1.3 자동개행을 반영해서 선택이 끝나는 줄의 위치와 글자 위치를 다시 조정한다.
			selectedEndRowPos = currentRowPos;
			selectedEndLetterPos = currentLetterPos;
			//3.1.4 변경된 화면 크기에 맞는 선택이 시작되는 줄과 글자 위치를 구한다.
			changedRowPos = 0;
			changedLetterPos = 0;
			originRowPos = this->startYPos;
			originLetterPos = this->startXPos;
			rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
				&changedRowPos);
			//3.1.5 자동개행을 반영해서 선택이 시작되는 줄과 글자 위치를 다시 조정한다.
			selectedStartRowPos = changedRowPos;//선택이 시작되는 줄
			selectedStartLetterPos = changedLetterPos;//선택이 시작되는 글자
		}
		//3.2 자동개행 진행 중이 아니면
		else
		{
			//3.2.1 현재 줄의 위치와 글자 위치를 다시 조정한다.
			currentRowPos = this->notepadForm->note->Move(this->selectedEndYPos);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->Move(this->selectedEndXPos);
			//3.2.2 선택이 시작되는 줄의 위치와 글자 위치를 다시 조정한다.
			selectedStartRowPos = this->startYPos;
			selectedStartLetterPos = this->startXPos;
			//3.2.3 선택이 끝나는 줄의 위치와 글자 위치를 다시 조정한다.
			selectedEndRowPos = currentRowPos;
			selectedEndLetterPos = currentLetterPos;
		}
		//3.3 선택된 단어(찾은 단어)를 지운다.
		this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
			selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
		//3.4 연산이 끝났기 때문에 현재 줄의 위치를 다시 조정해준다.(note의 연산 안에서 현재 줄의 위치와 글자 
		//위치는 조정이 되지만 notepadForm의 current(현재줄)는 조정할 수 없어서 notepadForm에서 해준다.)
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//3.5 선택영역이 삭제되었기 때문에 자동개행이 끝나고 나면 줄의 위치와 글자 위치가 바뀔 수
		//있기 때문에 시작하는 줄의 위치를 현재 줄의 위치와 글자위치로 재갱신해줘야한다.
		this->startYPos = this->notepadForm->note->GetCurrent();
		this->startXPos = this->notepadForm->current->GetCurrent();
		//3.6 자동 줄 바꿈 메뉴가 체크되어 있으면(선택영역이 삭제 됬기 때문에 자동개행을 다시해줘야함)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//3.6.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
			//OnSize에서 부분자동개행을 하도록 한다. 
			this->notepadForm->SendMessage(WM_SIZE);
			//2.6.2 선택영역이 삭제되었기 때문에 자동개행이 끝나고 나면 줄의 위치와 글자 위치가 바뀔 수
			//있기 때문에 시작하는 줄의 위치를 현재 줄의 위치와 글자위치로 재갱신해줘야한다.
			this->startYPos = this->notepadForm->note->GetCurrent();
			this->startXPos = this->notepadForm->current->GetCurrent();
			//3.6.3 지금 현재 startYPos와 startXPos의 위치는 자동개행이 적용된 상태의 위치이기 때문에
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
	//4. 메모장에서 선택된 단어를 지웠기 때문에 메모장에서 선택이 안된 상태로 바꾼다.
	this->notepadForm->isSelecting = false;
	//5. 선택이 끝났기 때문에 선택이 시작되는 x좌표를 0으로 저장한다.
	this->notepadForm->selectedStartXPos = 0;
	//6. 선택이 끝났기 때문에 선택이 시작되는 y좌표를 0으로 저장한다.
	this->notepadForm->selectedStartYPos = 0;
	//7. 선택된 영역이 없어졌기 때문에 복사하기, 잘라내기, 삭제 메뉴를 비활성화 시킨다.
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	//8. 현재 줄의 현재 글자 위치에서 바꿀 단어를 삽입한다.
	this->notepadForm->current->AddWord(this->replacingKeyword);
	//9. 연산이 끝났기 때문에 현재 글자의 위치를 다시 조정해준다.
	currentLetterPos = this->notepadForm->current->GetCurrent();
	//10. 글자를 추가한 후에 붙여넣기가 끝나는 줄의 위치와 글자위치를 저장한다.
	this->pastingEndYPos = this->notepadForm->note->GetCurrent();
	this->pastingEndXPos = this->notepadForm->current->GetCurrent();
	//11. 자동 줄 바꿈 메뉴가 체크되어 있으면(바꾼 단어가 추가됬기 때문에 자동개행을 다시해줘야함)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//11.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
		//OnSize에서 부분자동개행을 하도록 한다. 
		this->notepadForm->SendMessage(WM_SIZE);
		//11.2 글자를 추가하고 자동개행이 적용된 뒤에 붙여넣기가 끝나는 줄의 위치와 글자위치를 저장한다.
		this->pastingEndYPos = this->notepadForm->note->GetCurrent();
		this->pastingEndXPos = this->notepadForm->current->GetCurrent();
		//11.3 자동개행을 안한 상태의 붙여넣기가 끝나는 진짜 줄의 위치와 글자위치를 구한다.
		changedRowPos = this->pastingEndYPos;
		changedLetterPos = this->pastingEndXPos;
		originRowPos = 0;
		originLetterPos = 0;
		rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
			&originRowPos);
		this->pastingEndYPos = originRowPos;
		this->pastingEndXPos = originLetterPos;
	}
}

//실행취소
void OnReplaceButtonClickedCommand::Unexecute()
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
	//5. Execute에서 붙여넣은 바꾼 단어를 지운다.
	this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
		selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
	//6. 연산이 끝났기 때문에 현재 줄의 위치를 다시 조정해준다.(note의연산안에서 현재 줄의 위치와 글자 위치는
	//조정이 되지만 notepadForm의 current(현재줄)는 조정할 수 없어서 notepadForm에서 해준다.)
	currentRowPos = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	currentLetterPos = this->notepadForm->current->GetCurrent();
	//7. 선택영역이 삭제되었기 때문에 자동개행이 끝나고 나면 줄의 위치와 글자 위치가 바뀔 수
	//있기 때문에 시작하는 줄의 위치를 현재 줄의 위치와 글자위치로 재갱신해줘야한다.
	this->startYPos = this->notepadForm->note->GetCurrent();
	this->startXPos = this->notepadForm->current->GetCurrent();
	//8. 자동 줄 바꿈 메뉴가 체크되어 있으면
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//8.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
		//OnSize에서 부분자동개행을 하도록 한다. 
		this->notepadForm->SendMessage(WM_SIZE);
		//8.2 선택영역이 삭제되었기 때문에 자동개행이 끝나고 나면 줄의 위치와 글자 위치가 바뀔 수
		//있기 때문에 시작하는 줄의 위치를 현재 줄의 위치와 글자위치로 재갱신해줘야한다.
		this->startYPos = this->notepadForm->note->GetCurrent();
		this->startXPos = this->notepadForm->current->GetCurrent();
		//8.3 지금 현재 startYPos와 startXPos의 위치는 자동개행이 적용된 상태의 위치이기 때문에
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
	//9. 바꾼단어를 지웠고 이제 선택영역을 다시 복원하기 때문에 현재 줄과 글자의 위치를
	//선택영역을 시작 줄과 글자 위치로 설정한다.
	currentRowPos = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	currentLetterPos = this->notepadForm->current->GetCurrent();
	this->notepadForm->selectedStartYPos = currentRowPos;
	this->notepadForm->selectedStartXPos = currentLetterPos;
	//10. 메모장에서 선택된 texts를 다시 복구하기 때문에 메모장에서 선택이 된 상태로 바꾼다.
	this->notepadForm->isSelecting = true;
	//11. 선택영역이 다시 생겼기 때문에 복사하기, 잘라내기, 삭제 메뉴를 활성화 시킨다.
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
	//12. 바꾼 단어를 삽입하기 전에 줄의 위치를 저장한다.
	Long previousRowIndex = currentRowPos;
	//13. 바꾼 단어를 지운 위치에 아까 지웠던 선택영역 content를 삽입하고 현재 줄의 위치를 반환받는다.
	currentRowPos = this->notepadForm->note->
		InsertTexts(currentRowPos, currentLetterPos, this->note);
	//14. 메모장의 현재 줄을 저장한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	currentLetterPos = this->notepadForm->current->GetCurrent();
	//15. 메모장에 선택영역이 복원되었기 때문에 선택이 끝나는 줄과 글자위치를 갱신한다.
	this->selectedEndYPos = currentRowPos;
	this->selectedEndXPos = currentLetterPos;
	//16. 자동개행이 진행중이면 붙여넣은 줄들을 자동개행시켜준다.
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//16.1 부분자동개행을 한다.(마지막 줄은 제외하고 자동개행함)
		//바꾼 단어를 삽입하기 전에 현재 줄의 위치부터 바꾼 단어를 삽입한 후에 현재 줄의 위치까지 자동개행
		Long endPastedRowPos = rowAutoChange.DoPartRows(previousRowIndex, currentRowPos);
		//16.2 붙여넣기가 끝나는 줄로 이동시킨다.
		//붙여넣기가 끝나는 줄은 OnSize에서 부분자동개행을 해서 처리되기 때문에 캐럿의 위치만 조정해주면 됨!
		currentRowPos = this->notepadForm->note->Move(endPastedRowPos);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		this->notepadForm->current->Move(currentLetterPos);
		//16.3 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
			//OnSize에서 부분자동개행을 하도록 한다. (마지막 줄을 자동개행시킴)
		this->notepadForm->SendMessage(WM_SIZE);
		//16.4 메모장의 현재 줄을 저장한다.
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//16.5 메모장에 선택영역이 복원되었고, 자동개행인 반영된 선택이 끝나는 줄과 글자위치를 저장한다. 
		this->selectedEndYPos = currentRowPos;
		this->selectedEndXPos = currentLetterPos;
		//16.6 지금 현재 선택이 끝나는 줄과 글자 위치는 자동개행이 적용된 상태의 위치이기 때문에
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

//다시실행인지 여부 구하기
bool OnReplaceButtonClickedCommand::IsRedone()
{
	return this->isRedone;
}

//시작하는 줄의 위치구하기
Long OnReplaceButtonClickedCommand::GetStartYPos()
{
	return this->startYPos;
}
//시작하는 글자의 위치구하기
Long OnReplaceButtonClickedCommand::GetStartXPos()
{
	return this->startXPos;
}
//선택이 끝나는 줄의 위치 구하기
Long OnReplaceButtonClickedCommand::GetSelectedEndYPos()
{
	return this->selectedEndYPos;
}
//선택이 끝나는 글자 위치 구하기
Long OnReplaceButtonClickedCommand::GetSelectedEndXPos()
{
	return this->selectedEndXPos;
}
//붙여넣기가 끝나는 줄의 위치구하기
Long OnReplaceButtonClickedCommand::GetPastingEndYPos()
{
	return this->pastingEndYPos;
}
//붙여넣기가 끝나는 글자의 위치구하기
Long OnReplaceButtonClickedCommand::GetPastingEndXPos()
{
	return this->pastingEndXPos;
}

//SetMacroEnd(실행취소 및 다시실행 매크로출력 종료지점 설정)
void OnReplaceButtonClickedCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void OnReplaceButtonClickedCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}

//실행취소 종료지점여부 구하기
bool OnReplaceButtonClickedCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//다시실행 종료지점여부 구하기 
bool OnReplaceButtonClickedCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}

//SetRedone(다시 실행이라고 설정함)
void OnReplaceButtonClickedCommand::SetRedone()
{
	this->isRedone = true;
}

//선택영역이 지워졌는지 확인 여부
bool OnReplaceButtonClickedCommand::IsSelectedTextsRemoved()
{
	return this->isSelectedTextsRemoved;
}

//소멸자
OnReplaceButtonClickedCommand::~OnReplaceButtonClickedCommand()
{
	if (this->note != 0)
	{
		delete this->note;
	}
}