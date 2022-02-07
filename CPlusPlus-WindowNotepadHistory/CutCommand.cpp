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
	this->rowIndex = notepadForm->note->GetCurrent();
	this->letterIndex = notepadForm->current->GetCurrent();
	this->glyph = 0;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
	this->selectedStartXPos = 0;
	this->selectedStartYPos = 0;
	this->isSelectedTextsRemoved = false;//처음에 생성될 때는 선택영역이 안지워졌으므로 false가 디폴트값
}

//Execute
void CutCommand::Execute()
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
	//3. RemoveCommand가 다시 실행되면
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
	//4. 다시 실행이면
	if (this->isRedone == true)
	{
		this->notepadForm->selectedStartYPos = this->selectedStartYPos;
		this->notepadForm->selectedStartXPos = this->selectedStartXPos;
	}
	//5. 선택이 시작되는 줄과 글자 위치, 선택이 끝나는 줄과 글자 위치를 저장한다.
	Long selectedStartRowPos = this->notepadForm->selectedStartYPos;//선택이 시작되는 줄
	Long selectedStartLetterPos = this->notepadForm->selectedStartXPos;//선택이 시작되는 글자
	Long selectedEndRowPos = currentRowPos;//선택이 끝나는 줄
	Long selectedEndLetterPos = currentLetterPos;//선택이 끝나는 글자
	//6. 처음 실행이면
	if (this->isRedone == false)
	{
		//6.1 content를 복사하고 지운다.
		this->glyph = this->notepadForm->note->CopySelectedTextsAndRemove(selectedStartRowPos,
			selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
		//6.2 메모장 제목에 *를 추가한다.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - 메모장";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//6.3 메모장에 변경사항이 있음을 저장한다.
		this->notepadForm->isDirty = true;
		//6.4 notepadForm의 clipboard에 command의 새로 생성한 Note(복사한 내용)를 추가시켜준다.
		Long noteIndex = this->notepadForm->clipboard->Add(this->glyph->Clone());
		//내부클립보드에 복사한 내용을 외부클립보드로 옮기기
		//6.5 notepadForm의 clipboard에 새로 생성한 Note(복사한 내용)와 그 길이를 구한다.
		CString content = this->notepadForm->clipboard->
			GetAt(noteIndex)->GetContent().c_str();
		//저장할 문자열의 길이를 구한다. ('\0'까지 포함한 크기)
		int contentLength = content.GetLength() + 1;
		//6.6 외부 클립보드가 열렸으면
		//클립보드는 여러 프로그램들이 공유할 수 있는 기능이기 때문에 어떤 프로그램(윈도우)이 클립보드를
		//사용하는지 지정하고 사용하는 것이 좋다
		if (this->notepadForm->OpenClipboard() != 0)
		{
			//6.6.1 notepadForm의 clipboard에 새로 생성한 Note(복사한 내용)을 클립보드에 옮길 공간을 생성
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
	//7. 다시 실행이면
	else
	{
		//7.1 content를 지운다.
		this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
			selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
	}
	//8. 연산이 끝났기 때문에 현재 줄의 위치를 다시 조정해준다.(note의연산안에서 현재 줄의 위치와 글자 위치는
	//조정이 되지만 notepadForm의 current(현재줄)는 조정할 수 없어서 notepadForm에서 해준다.)
	this->notepadForm->current = this->notepadForm->note->
		GetAt(this->notepadForm->note->GetCurrent());
	//9. 복사한 texts가 생겼기 때문에 붙여넣기 메뉴를 활성화시켜준다.
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_PASTE, MF_BYCOMMAND | MF_ENABLED);
	//10. 자동 줄 바꿈 메뉴가 체크되어 있으면
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//10.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
		//OnSize에서 부분자동개행을 하도록 한다. 
		this->notepadForm->SendMessage(WM_SIZE);
	}
	//11. 메모장에서 선택된 texts를 다 지웠기 때문에 메모장에서 선택이 안된 상태로 바꾼다.
	this->notepadForm->isSelecting = false;
	//12. 선택이 끝났기 때문에 캐럿의 x좌표를 0으로 저장한다.
	this->notepadForm->selectedStartXPos = 0;
	//13. 선택이 끝났기 때문에 캐럿의 y좌표를 0으로 저장한다.
	this->notepadForm->selectedStartYPos = 0;
	//14. 복사하기, 잘라내기, 삭제 메뉴를 비활성화 시킨다.
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	//15. 글자를 지운 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();
	//16. 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치를 저장해야함)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		Long changedRowPos = this->rowIndex;
		Long changedLetterPos = this->letterIndex;
		Long originRowPos = 0;
		Long originLetterPos = 0;
		//16.1 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
		rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
			&originRowPos);
		//16.2 command에 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
		this->rowIndex = originRowPos;
		this->letterIndex = originLetterPos;
	}
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
		//4.1 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
		rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
			&changedRowPos);
		//4.2 현재 줄의 위치와 글자 위치를 다시 조정한다.
		currentRowPos = this->notepadForm->note->Move(changedRowPos);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
	}
	//6. 메모장에서 선택된 texts를 다시 복구하기 때문에 메모장에서 선택이 된 상태로 바꾼다.
	this->notepadForm->isSelecting = true;
	//7. 선택이 다시 시작했기 때문에 캐럿의 x좌표를 command에 저장된 현재 글자 위치로 저장한다.
	this->notepadForm->selectedStartXPos = currentLetterPos;
	this->selectedStartXPos = this->notepadForm->selectedStartXPos;
	//8. 선택이 다시 시작했기 때문에 캐럿의 y좌표를 command에 저장된 현재 줄의 위치로 저장한다.
	this->notepadForm->selectedStartYPos = currentRowPos;
	this->selectedStartYPos = this->notepadForm->selectedStartYPos;
	//9. 현재 위치에 command가 가지고 있는 content를 삽입한다.
	Long rowIndex = this->notepadForm->note->
		InsertTexts(currentRowPos, currentLetterPos, this->glyph);
	//10. 메모장의 현재 줄을 저장한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(rowIndex);
	//11. 자동개행이 진행중이면 붙여넣은 줄들을 자동개행시켜준다.
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//11.1 자동개행클래스를 생성한다.
		RowAutoChange rowAutoChange(this->notepadForm);
		//11.2 부분자동개행을 한다.
		Long endPastedRowPos = rowAutoChange.DoPartRows(currentRowPos, rowIndex);
		//11.3 붙여넣기가 끝나는 줄로 이동시킨다.
		//붙여넣기가 끝나는 줄은 OnSize에서 부분자동개행을 해서 처리되기 때문에 캐럿의 위치만 조정해주면 됨!
		currentRowPos = this->notepadForm->note->Move(endPastedRowPos);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		this->notepadForm->current->Move(currentLetterPos);
	}
	//12. 선택영역이 다시 생겼기 때문에 복사하기, 잘라내기, 삭제 메뉴를 활성화 시킨다.
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
	//13. 글자가 다시 추가되었기 때문에 현재 줄의 위치와 글자위치를 다시 저장한다.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();
	//14. 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치를 저장해야함)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		Long changedRowPos = this->rowIndex;
		Long changedLetterPos = this->letterIndex;
		Long originRowPos = 0;
		Long originLetterPos = 0;
		//14.1 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
		rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
			&originRowPos);
		//14.2 command에 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
		this->rowIndex = originRowPos;
		this->letterIndex = originLetterPos;
	}
}

//다시실행인지 여부 구하기
bool CutCommand::IsRedone()
{
	return this->isRedone;
}

//줄의 위치 구하기
Long CutCommand::GetRowIndex()
{
	return this->rowIndex;
}
//글자 위치 구하기
Long CutCommand::GetLetterIndex()
{
	return this->letterIndex;
}

//선택이 시작되는 글자의 위치구하기
Long CutCommand::GetSelectedStartXPos()
{
	return this->selectedStartXPos;
}

//선택이 시작되는 줄의 위치구하기
Long CutCommand::GetSelectedStartYPos()
{
	return this->selectedStartYPos;
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

//소멸자
CutCommand::~CutCommand()
{
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}