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
	this->startPasteRowIndex = notepadForm->selectedStartYPos;
	this->startPasteLetterIndex = notepadForm->selectedStartXPos;
	this->rowIndex = notepadForm->note->GetCurrent();
	this->letterIndex = notepadForm->current->GetCurrent();
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
}

//처음 및 다시 실행(선택영역이 항상 있는 상태에서 실행됨)
void OnReplaceButtonClickedCommand::Execute()
{
	//1. RowAutoChange를 생성한다.
	RowAutoChange rowAutoChange(this->notepadForm);
	//2. 선택이 시작되는 줄의 위치와 글자 위치를 구한다.
	Long selectedStartRowPos = notepadForm->selectedStartYPos;//선택이 시작되는 줄
	Long selectedStartLetterPos = notepadForm->selectedStartXPos;//선택이 시작되는 글자
	Long selectedEndRowPos = this->rowIndex;//선택이 끝나는 줄
	Long selectedEndLetterPos = this->letterIndex;//선택이 끝나는 글자

	Long changedRowPos = 0;
	Long changedLetterPos = 0;
	Long originRowPos = 0;
	Long originLetterPos = 0;
	//3. 현재 줄의 위치와 글자 위치를 구한다.
	Long currentRowPos = this->notepadForm->note->GetCurrent();
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	//4. OnReplaceButtonClickedCommand가 다시 실행되면
	if (this->isRedone == true)
	{
		//4.1 현재 줄의 위치와 글자 위치를 다시 조정한다.
		currentRowPos = this->notepadForm->note->Move(this->rowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
		//4.2 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치가 저장되어 있음)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//4.2.1 변경된 화면 크기에 맞는 현재 줄과 글자 위치를 구한다.
			changedRowPos = 0;
			changedLetterPos = 0;
			originRowPos = this->rowIndex;
			originLetterPos = this->letterIndex;
			rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
				&changedRowPos);
			//4.2.2 현재 줄의 위치와 글자 위치를 다시 조정한다.
			currentRowPos = this->notepadForm->note->Move(changedRowPos);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
			//4.2.3 변경된 화면 크기에 맞는 선택이 시작되는 줄과 글자 위치를 구한다.
			changedRowPos = 0;
			changedLetterPos = 0;
			originRowPos = this->startPasteRowIndex;
			originLetterPos = this->startPasteLetterIndex;
			rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
				&changedRowPos);
			//4.4 선택이 시작되는 줄과 글자 위치, 선택이 끝나는 줄과 글자 위치를 저장한다.
			selectedStartRowPos = changedRowPos;//선택이 시작되는 줄
			selectedStartLetterPos = changedLetterPos;//선택이 시작되는 글자
			selectedEndRowPos = currentRowPos;//선택이 끝나는 줄
			selectedEndLetterPos = currentLetterPos;//선택이 끝나는 글자
		}
	}
	//5. 처음 실행이면
	if (this->isRedone == false)
	{
		//5.1 content를 복사하고 지운다.
		this->note = this->notepadForm->note->CopySelectedTextsAndRemove(selectedStartRowPos,
			selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
	}
	//6. 다시 실행이면
	else
	{
		//6.1 content를 지운다.
		this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
			selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
	}
	//7. 연산이 끝났기 때문에 현재 줄의 위치를 다시 조정해준다.(note의연산안에서 현재 줄의 위치와 글자 위치는
	//조정이 되지만 notepadForm의 current(현재줄)는 조정할 수 없어서 notepadForm에서 해준다.)
	currentRowPos = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	currentLetterPos = this->notepadForm->current->GetCurrent();
	//8. 자동 줄 바꿈이 진행중이면
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//8.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
		//OnSize에서 부분자동개행을 하도록 한다. 
		this->notepadForm->SendMessage(WM_SIZE);
	}
	//9. 메모장에서 선택된 texts를 다 지웠기 때문에 메모장에서 선택이 안된 상태로 바꾼다.
	this->notepadForm->isSelecting = false;
	//10. 선택이 끝났기 때문에 캐럿의 x좌표를 0으로 저장한다.
	this->notepadForm->selectedStartXPos = 0;
	//11. 선택이 끝났기 때문에 캐럿의 y좌표를 0으로 저장한다.
	this->notepadForm->selectedStartYPos = 0;
	//12. 복사하기, 잘라내기, 삭제 메뉴를 비활성화 시킨다.
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	//13. 선택영역을 지우고 단어를 삽입하기 전에 시작되는 줄의 위치를 저장한다.
	this->startPasteRowIndex = currentRowPos;
	//14. 선택영역을 지우고 단어를 삽입하기 전에 시작되는 글자의 위치를 저장한다.
	this->startPasteLetterIndex = currentLetterPos;
	//15. 현재 줄의 현재 글자 위치에서 바꿀 단어를 삽입한다.
	this->notepadForm->current->AddWord(this->replacingKeyword);
	//16. 연산이 끝났기 때문에 현재 줄의 위치를 다시 조정해준다.
	//(note의연산안에서 현재 줄의 위치와 글자 위치는
	//조정이 되지만 notepadForm의 current(현재줄)는 조정할 수 없어서 notepadForm에서 해준다.)
	currentRowPos = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	currentLetterPos = this->notepadForm->current->GetCurrent();
	//17. 자동 줄 바꿈이 진행중이면
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//17.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
		//OnSize에서 부분자동개행을 하도록 한다. 
		this->notepadForm->SendMessage(WM_SIZE);
	}
	//18. 처음 실행이면
	if (this->isRedone == false)
	{
		//18.1 메모장 제목에 *를 추가한다.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - 메모장";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//18.2 메모장에 변경사항이 있음을 저장한다.
		this->notepadForm->isDirty = true;
	}
	//19. 글자를 지운 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();
	//19. 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치를 저장해야함)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		Long changedRowPos = this->rowIndex;
		Long changedLetterPos = this->letterIndex;
		Long originRowPos = 0;
		Long originLetterPos = 0;
		//19.1 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
		rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
			&originRowPos);
		//19.2 command에 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
		this->rowIndex = originRowPos;
		this->letterIndex = originLetterPos;
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
	Long changedRowPos = 0;
	Long changedLetterPos = 0;
	Long originRowPos = this->rowIndex;
	Long originLetterPos = this->letterIndex;
	//3. 현재 줄의 위치를 이동시킨다.(캐럿이 다른 곳에 있으면 그 곳에 글자가 지워지기 때문에)
	Long currentRowPos = this->notepadForm->note->Move(this->rowIndex);
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	//4. 현재 글자의 위치를 이동시킨다.
	Long currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
	//5. 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치가 저장되어 있음)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//5.1 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
		rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
			&changedRowPos);
		//5.2 현재 줄의 위치와 글자 위치를 다시 조정한다.
		currentRowPos = this->notepadForm->note->Move(changedRowPos);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
	}
	//6. 선택이 시작되는 줄과 글자 위치, 선택이 끝나는 줄과 글자 위치를 저장한다.
	Long selectedStartRowPos = this->startPasteRowIndex;//선택이 시작되는 줄
	Long selectedStartLetterPos = this->startPasteLetterIndex;//선택이 시작되는 글자
	Long selectedEndRowPos = currentRowPos;//선택이 끝나는 줄
	Long selectedEndLetterPos = currentLetterPos;//선택이 끝나는 글자
	//7. 바꾼 단어를 지운다.
	this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
		selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
	//8. 연산이 끝났기 때문에 현재 줄의 위치를 다시 조정해준다.(note의연산안에서 현재 줄의 위치와 글자 위치는
	//조정이 되지만 notepadForm의 current(현재줄)는 조정할 수 없어서 notepadForm에서 해준다.)
	currentRowPos = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	currentLetterPos = this->notepadForm->current->GetCurrent();
	//16. 자동 줄 바꿈이 진행중이면
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//16.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
		//OnSize에서 부분자동개행을 하도록 한다. 
		this->notepadForm->SendMessage(WM_SIZE);
	}
	//10.1 현재 줄의 위치를 이동시킨다.(캐럿이 다른 곳에 있으면 그 곳에 글자가 지워지기 때문에)
	currentRowPos = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	currentLetterPos = this->notepadForm->current->GetCurrent();
	//9. 메모장에서 선택된 texts를 다시 복구하기 때문에 메모장에서 선택이 된 상태로 바꾼다.
	this->notepadForm->isSelecting = true;
	//10. 선택이 다시 시작했기 때문에 캐럿의 x좌표를 command에 저장된 현재 글자 위치로 저장한다.
	this->notepadForm->selectedStartXPos = currentLetterPos;
	this->startPasteLetterIndex = this->notepadForm->selectedStartXPos;
	//11. 선택이 다시 시작했기 때문에 캐럿의 y좌표를 command에 저장된 현재 줄의 위치로 저장한다.
	this->notepadForm->selectedStartYPos = currentRowPos;
	this->startPasteRowIndex = this->notepadForm->selectedStartYPos;
	//12. 현재 위치에 command가 가지고 있는 content를 삽입한다.
	Long rowIndex = this->notepadForm->note->
		InsertTexts(currentRowPos, currentLetterPos, this->note);
	//13. 메모장의 현재 줄을 저장한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(rowIndex);
	//14. 자동개행이 진행중이면 붙여넣은 줄들을 자동개행시켜준다.
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//14.1 부분자동개행을 한다.
		Long endPastedRowPos = rowAutoChange.DoPartRows(currentRowPos, rowIndex);
		//14.2 붙여넣기가 끝나는 줄로 이동시킨다.
		//붙여넣기가 끝나는 줄은 OnSize에서 부분자동개행을 해서 처리되기 때문에 캐럿의 위치만 조정해주면 됨!
		currentRowPos = this->notepadForm->note->Move(endPastedRowPos);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		this->notepadForm->current->Move(currentLetterPos);
	}
	//15. 선택영역이 다시 생겼기 때문에 복사하기, 잘라내기, 삭제 메뉴를 활성화 시킨다.
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
	//16. 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();
	//17. 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치를 저장해야함)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		Long changedRowPos = this->rowIndex;
		Long changedLetterPos = this->letterIndex;
		Long originRowPos = 0;
		Long originLetterPos = 0;
		//17.1 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
		rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
			&originRowPos);
		//17.2 command에 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
		this->rowIndex = originRowPos;
		this->letterIndex = originLetterPos;
	}
}

//다시실행인지 여부 구하기
bool OnReplaceButtonClickedCommand::IsRedone()
{
	return this->isRedone;
}

//줄의 위치 구하기
Long OnReplaceButtonClickedCommand::GetRowIndex()
{
	return this->rowIndex;
}
//글자 위치 구하기
Long OnReplaceButtonClickedCommand::GetLetterIndex()
{
	return this->letterIndex;
}
//선택이 시작되는 글자의 위치구하기
Long OnReplaceButtonClickedCommand::GetStartPasteRowIndex()
{
	return this->startPasteRowIndex;
}

//선택이 시작되는 줄의 위치구하기
Long OnReplaceButtonClickedCommand::GetStartPasteLetterIndex()
{
	return this->startPasteLetterIndex;
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

//소멸자
OnReplaceButtonClickedCommand::~OnReplaceButtonClickedCommand()
{
	if (this->note != 0)
	{
		delete this->note;
	}
}