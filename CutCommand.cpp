#include "CutCommand.h"
#include "NotepadForm.h"
#include "Note.h"
#include "DummyRow.h"
#include "RowAutoChange.h"
#include "SelectingTexts.h"

//디폴트생성자
CutCommand::CutCommand(NotepadForm* notepadForm)
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
	this->isSelectedTextsRemoved = true;//선택영역이 있어야만 cutCommand에 들어올 수 있고,
	//CutCommand가 실행된다면 반드시 선택영역은 지워지기 때문에 default값이 true임!
}

//Execute
void CutCommand::Execute()
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

	//2. CutCommand가 처음 실행되면
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
		this->glyph = this->notepadForm->note->CopySelectedTextsAndRemove(selectedStartRowPos,
			selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
		//2.5 메모장 제목에 *를 추가한다.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - 메모장";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//2.6 메모장에 변경사항이 있음을 저장한다.
		this->notepadForm->isDirty = true;
		//2.7 notepadForm의 clipboard에 command의 새로 생성한 Note(복사한 내용)를 추가시켜준다.
		Long noteIndex = this->notepadForm->clipboard->Add(this->glyph->Clone());
		//내부클립보드에 복사한 내용을 외부클립보드로 옮기기
		//2.8 notepadForm의 clipboard에 새로 생성한 Note(복사한 내용)와 그 길이를 구한다.
		CString content = this->notepadForm->clipboard->
			GetAt(noteIndex)->GetContent().c_str();
		//저장할 문자열의 길이를 구한다. ('\0'까지 포함한 크기)
		int contentLength = content.GetLength() + 1;
		//2.9 외부 클립보드가 열렸으면
		//클립보드는 여러 프로그램들이 공유할 수 있는 기능이기 때문에 어떤 프로그램(윈도우)이 클립보드를
		//사용하는지 지정하고 사용하는 것이 좋다
		if (this->notepadForm->OpenClipboard() != 0)
		{
			//2.9.1 notepadForm의 clipboard에 새로 생성한 Note(복사한 내용)을 클립보드에 옮길 공간을 생성
			//클립보드에 문자열을 복사하기 위해서는 문자열을 저장한 메모리를 클립보드로 전달해야 합니다.
			//따라서 문자열을 저장할 메모리를 먼저 만들어야 하는데, 이 메모리 형식이 핸들을 사용해야 하기
			//때문에 HeapAlloc, malloc 같은 힙 함수는 사용이 불가능합니다.그래서 아래와 핸들 값 형식을
			//사용하는 GlobalAlloc 함수를 사용해서 메모리를 할당해야 하고, 클립보드는 다른 프로그램과
			//공유하는 형식이기 때문에 이 메모리 속성에 GMEM_DDESHARE와 GMEM_MOVEABLE를 추가한다.
			//(이 메모리는 클립보드로 전달되기 때문에 자신의 프로그램에서 해제하면 안된다)
			HGLOBAL hClipboardData = GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE, contentLength);
			//이렇게 할당된 메모리는 핸들 값을 반환하기 때문에 이 메모리에 문자열을 복사하기 위해서는 사용
			//가능한 주소를 얻어야 한다. 따라서 GlobalLock 함수를 사용하여 자신이 이 메모리를 사용하겠다고
			//설정하고 사용 가능한 주소를 얻어야 한다.이때, 주의해야 할 점은 클립보드에서 사용하는
			//문자열이 ASCII 형식으로 구성되어야 하기 때문에 유니코드를 지원(MFC 프로그램들은 기본적으로
			//(유니코드 형식임)하는 프로그램에서는 ASCII 형식으로 문자열을 변경해서 사용해야 한다.
			char* pchData = (char*)GlobalLock(hClipboardData);
			if (pchData != NULL)
			{
				// 할당된 메모리 영역에 삽입할 문자열을 복사한다.
				memcpy(pchData, content, contentLength);
				// 문자열을 복사하기 위해서 Lock 했던 메모리를 해제한다. 
				// 클립보드를 연다. 
				GlobalUnlock(hClipboardData);
				// 클립보드에 저장된 기존 문자열을 삭제한다. 
				//다른 프로그램이 클립보드를 사용하거나 다른 문자열이 클립보드에 저장되어 있을 수 있기
				//때문에 EmptyClipboard 함수를 사용해서 클립보드에 데이터를 저장하기 위한 준비
				EmptyClipboard();
				// 클립보드로 문자열을 복사하고, 클립보드에 저장하고자 하는 정보가 문자열임을 알려준다.
				SetClipboardData(CF_TEXT, hClipboardData);
				// 클립보드를 닫는다.
				CloseClipboard();
			}
		}
	}
	//3. CutCommand가 다시 실행되면
	else
	{
		//3.1 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치가
		//저장되어 있음)
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
	}
	//4. 연산이 끝났기 때문에 현재 줄의 위치를 다시 조정해준다.(note의 연산 안에서 
	//현재 줄의 위치와 글자 위치는 조정이 되지만 notepadForm의 current(현재줄)는 
	//조정할 수 없어서 notepadForm에서 해준다.)
	currentRowPos = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	currentLetterPos = this->notepadForm->current->GetCurrent();
	//5. 선택된 단어를 지웠기 때문에 다시 시작하는 줄의 위치와 글자위치를 갱신한다.
	this->startYPos = currentRowPos;
	this->startXPos = currentLetterPos;
	//6. 자동 줄 바꿈 메뉴가 체크되어 있으면(선택영역이 삭제 됬기 때문에 자동개행을 다시해줘야함)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//6.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
		//OnSize에서 부분자동개행을 하도록 한다. 
		this->notepadForm->SendMessage(WM_SIZE);
		//6.2 선택영역이 삭제되었기 때문에 자동개행이 끝나고 나면 줄의 위치와 글자 위치가
		//바뀔 수있기 때문에 시작하는 줄의 위치를 현재 줄의 위치와 글자위치로 재갱신해줘야한다.
		this->startYPos = this->notepadForm->note->GetCurrent();
		this->startXPos = this->notepadForm->current->GetCurrent();
		//6.3 지금 현재 startYPos와 startXPos의 위치는 자동개행이 적용된 상태의 위치이기
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
	//7. 메모장에서 선택된 단어를 지웠기 때문에 메모장에서 선택이 안된 상태로 바꾼다.
	this->notepadForm->isSelecting = false;
	//8. 선택이 끝났기 때문에 선택이 시작되는 x좌표를 0으로 저장한다.
	this->notepadForm->selectedStartXPos = 0;
	//9. 선택이 끝났기 때문에 선택이 시작되는 y좌표를 0으로 저장한다.
	this->notepadForm->selectedStartYPos = 0;
	//10. 선택된 영역이 없어졌기 때문에 복사하기, 잘라내기, 삭제 메뉴를 비활성화 시킨다.
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	//11. 복사한 texts가 생겼기 때문에 붙여넣기 메뉴를 활성화시켜준다.
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_PASTE, MF_BYCOMMAND | MF_ENABLED);
}

//실행취소
void CutCommand::Unexecute()
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
	//5. 바꾼단어를 지웠고 이제 선택영역을 다시 복원하기 때문에 현재 줄과 글자의 위치를
	//선택영역을 시작 줄과 글자 위치로 설정한다.
	this->notepadForm->selectedStartYPos = currentRowPos;
	this->notepadForm->selectedStartXPos = currentLetterPos;
	//6. 메모장에서 선택된 texts를 다시 복구하기 때문에 메모장에서 선택이 된 상태로 바꾼다.
	this->notepadForm->isSelecting = true;
	//7. 선택영역이 다시 생겼기 때문에 복사하기, 잘라내기, 삭제 메뉴를 활성화 시킨다.
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
	//8. 선택영역을 다시 복원하기 전에 줄의 위치를 저장한다.
	Long previousRowIndex = currentRowPos;
	//9. 선택영역을 다시 복원하고 현재 줄의 위치를 반환받는다.
	currentRowPos = this->notepadForm->note->
		InsertTexts(currentRowPos, currentLetterPos, this->glyph);
	//10. 메모장의 현재 줄을 저장한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	currentLetterPos = this->notepadForm->current->GetCurrent();
	//11. 메모장에 선택영역이 복원되었기 때문에 선택이 끝나는 줄과 글자위치를 갱신한다.
	this->selectedEndYPos = currentRowPos;
	this->selectedEndXPos = currentLetterPos;
	//12. 자동개행이 진행중이면 붙여넣은 줄들을 자동개행시켜준다.
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//12.1 부분자동개행을 한다.(마지막 줄은 제외하고 자동개행함)
		Long endPastedRowPos = rowAutoChange.DoPartRows(previousRowIndex, currentRowPos);
		//12.2 선택영역이 끝나는 줄로 이동시킨다.
		//선택영역이 끝나는 줄은 OnSize에서 부분자동개행을 해서 처리되기 때문에 캐럿의 위치만 조정해주면 됨!
		currentRowPos = this->notepadForm->note->Move(endPastedRowPos);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		this->notepadForm->current->Move(currentLetterPos);
		//12.3 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
		//OnSize에서 부분자동개행을 하도록 한다. (마지막 줄을 자동개행시킴)
		this->notepadForm->SendMessage(WM_SIZE);
		//12.4 메모장의 현재 줄을 저장한다.
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//12.5 메모장에 선택영역이 복원되었고, 자동개행인 반영된 선택이 끝나는 줄과 글자위치를 저장한다. 
		this->selectedEndYPos = currentRowPos;
		this->selectedEndXPos = currentLetterPos;
		//12.6 지금 현재 선택이 끝나는 줄과 글자 위치는 자동개행이 적용된 상태의 위치이기 때문에
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
bool CutCommand::IsRedone()
{
	return this->isRedone;
}

//시작하는 줄의 위치구하기
Long CutCommand::GetStartYPos()
{
	return this->startYPos;
}
//시작하는 글자의 위치구하기
Long CutCommand::GetStartXPos()
{
	return this->startXPos;
}
//선택이 끝나는 줄의 위치 구하기
Long CutCommand::GetSelectedEndYPos()
{
	return this->selectedEndYPos;
}
//선택이 끝나는 글자 위치 구하기
Long CutCommand::GetSelectedEndXPos()
{
	return this->selectedEndXPos;
}

//SetMacroEnd(실행취소 및 다시실행 매크로출력 종료지점 설정)
void CutCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void CutCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}

//실행취소 종료지점여부 구하기
bool CutCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//다시실행 종료지점여부 구하기 
bool CutCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}

//SetRedone(다시 실행이라고 설정함)
void CutCommand::SetRedone()
{
	this->isRedone = true;
}

//선택영역이 지워졌는지 확인 여부
bool CutCommand::IsSelectedTextsRemoved()
{
	return this->isSelectedTextsRemoved;
}

Glyph* CutCommand::GetGlyph()
{
	return this->glyph;
}

//소멸자
CutCommand::~CutCommand()
{
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}