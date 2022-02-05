#include "OnCompositionCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"

//디폴트생성자 정의
OnCompositionCommand::OnCompositionCommand(NotepadForm* notepadForm, WPARAM wParam,
	Long rowIndex, Long letterIndex)
	:Command(notepadForm)
{
	this->wParam = wParam;
	this->rowIndex = rowIndex;
	this->letterIndex = letterIndex;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
}

//실행
void OnCompositionCommand::Execute()
{
	//1. 메모장에서 선택된 texts가 있으면
	if (this->notepadForm->isSelecting == true)
	{
		//1.1 RemoveCommand로 메세지를 보내서 선택영역을 지운다.
		this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_REMOVE);
	}
	Long currentRowPos = 0;
	//다시 실행이면 캐럿의 위치를 조정
	//2. OnCompositionCommand가 다시 실행되면
	if (this->isRedone == true)
	{
		currentRowPos = this->notepadForm->note->Move(this->rowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		//2.2.1 글자가 써지면 한 칸 이동이 되는데, 글자가 취소되었기 때문에 한 칸 이전으로 이동시킨다.
		this->letterIndex--;
		this->notepadForm->current->Move(this->letterIndex);
	}
	//2. glyphCreator를 생성한다.
	GlyphCreator glyphCreator;
	WORD word = LOWORD(this->wParam);
	//3. 키보드로부터 입력받을 정보를 바탕으로 새로운 한글을 저장한다.
	char koreanLetter[3];
	koreanLetter[0] = HIBYTE(word);
	koreanLetter[1] = LOBYTE(word);
	koreanLetter[2] = '\0';
	//4. 현재 줄의 캐럿의 가로 위치를 구한다.
	Long index = this->notepadForm->current->GetCurrent();
	//5. IsComposing값이 '참'이면(한글이 조립중인 상태이면)
	if (this->notepadForm->isComposing == true)
	{
		//5.1 현재 줄의 캐럿의 가로 위치 바로 앞에 있는 기존 한글을 지운다.
		//그러기 위해서는 캐럿의 현재 가로 위치에 1감소한 값을 넣어주면 된다.
		//기존 한글을 지워야 새로 입력 받은 한글을 대체할 수 있다.
		this->notepadForm->current->Remove(index - 1);
		//5.2 갱신된 current의 위치를 index에 저장한다.
		index = this->notepadForm->current->GetCurrent();
	}
	//6. 현재위치의 한글을 지웠기 때문에 한글이 조립중이 아님으로 상태를 변경한다.
	this->notepadForm->isComposing = false;
	//7. 새로운 한글이 입력되었으면(한글 조립중에 글자를 다 지워버리면 '\0'문자로
	//OnComposition에 입력된다 백스페이스키가 입력되면 기존 한글이 지워지고 '\0'가 들어 오게 된다.)
	if (koreanLetter[0] != '\0')
	{
		//7.1 doubleByteLetter를 생성한다.
		Glyph* doubleByteLetter = glyphCreator.Create((char*)koreanLetter);
		//7.2 index가 현재 줄의 length와 같으면
		if (index == this->notepadForm->current->GetLength())
		{
			//7.2.1 현재 줄의 마지막 글자 뒤에 새로운 한글을 추가한다.
			index = this->notepadForm->current->Add(doubleByteLetter);
		}
		//7.3 index가 현재 줄의 length와 다르면
		else
		{
			//7.3.1 현재 줄의 index번째에 새로운 한글을 끼워 쓴다.
			index = this->notepadForm->current->Add(index, doubleByteLetter);
		}
		//7.4 한글을 현재 위치에 추가했기때문에 한글이 조립중인 상태로 변경한다.
		this->notepadForm->isComposing = true;
	}
	//8. 한글 조립중에 백스페이스키룰 눌러서 조립 중인 한글을 지워버리면
	else
	{
		//BackSpace와 Delete키와 별도로 한글조립중에 지우는 경우도 OnSize로 보내줘야 한다.
		//8.1 자동 줄 바꿈이 진행중이면
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//8.1.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
			//OnSize에서 부분자동개행을 하도록 한다. 
			this->notepadForm->SendMessage(WM_SIZE);
		}
	}
	//9. 캐럿의 위치와 크기가 변경되었음을 알린다.
	this->notepadForm->Notify();
	//10. 메모장 제목에 *를 추가한다.
	string name = this->notepadForm->fileName;
	name.insert(0, "*");
	name += " - 메모장";
	this->notepadForm->SetWindowText(CString(name.c_str()));
	//11. 메모장에 변경사항이 있음을 저장한다.
	this->notepadForm->isDirty = true;
	//12. 갱신한다.
	this->notepadForm->Invalidate(TRUE);
	//12. 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();
}

//실행취소
void OnCompositionCommand::Unexecute()
{
	//1. 현재 줄의 위치를 이동시킨다.(캐럿이 다른 곳에 있으면 그 곳에 글자가 지워지기 때문에)
	Long currentRowPos = this->notepadForm->note->Move(this->rowIndex);
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	//2. 현재 글자의 위치를 이동시킨다.
	Long currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
	//3. 메모장에서 선택된 texts가 없으면
	if (this->notepadForm->isSelecting == false)
	{
		//제일 첫 줄이면 줄을 지울 수 없고, 제일 첫 줄에서 글자 위치가 제일 처음에 있으면
		//아무것도 안일어남. 현재 줄의 위치가 제일 처음이 아니고, 현재 글자 위치가 제일 처음이면
		//현재 줄을 이전 줄에 Join시킨다.
		//3.1 현재 줄의 위치가 0보다 크고, 현재 글자 위치가 0이면
		Glyph* currentRow = 0;
		Glyph* previousRow = 0;
		if (currentRowPos > 0 && currentLetterPos == 0)
		{
			//3.1.1 현재 줄을 구한다.
			currentRow = this->notepadForm->note->GetAt(currentRowPos);
			//3.1.2 현재 줄의 이전 줄을 구한다.
			previousRow = this->notepadForm->note->GetAt(currentRowPos - 1);
			//3.1.3 현재 줄의 이전 줄의 마지막 글자 위치를 구한다.
			Long letterPos = previousRow->GetLength();
			//3.1.4 현재 줄을 이전 줄에 합친다.
			currentRow->Join(previousRow);
			//3.1.5 Note에서 현재 줄의 주소를 지운다.(내용은 지우면 안됨)
			this->notepadForm->note->Remove(currentRowPos);
			//3.1.6 줄이 지워졌기 때문에 줄의 위치를 다시 구한다.
			currentRowPos = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			//3.1.7 현재 줄의 글자 위치가 지금은 마지막이기 때문에 변경해준다.
			//이전 줄의 마지막 위치로 이동시킨다.
			Long index = this->notepadForm->current->Move(letterPos);
			//3.1.8 메모장 제목에 *를 추가한다.
			string name = this->notepadForm->fileName;
			name.insert(0, "*");
			name += " - 메모장";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//3.1.9 메모장에 변경사항이 있음을 저장한다.
			this->notepadForm->isDirty = true;
		}
		// 현재 글자 위치가 처음이 아닐 때(현재 줄이 처음이든 아니든 상관없음) 현재 글자를 지운다.
		//3.2 현재 글자 위치가 처음이 아니면
		else if (currentLetterPos > 0)
		{
			//3.2.1 현재 글자를 지운다.
			this->notepadForm->current->Remove(currentLetterPos - 1);
			//3.2.2 메모장 제목에 *를 추가한다.
			string name = this->notepadForm->fileName;
			name.insert(0, "*");
			name += " - 메모장";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//3.2.3 메모장에 변경사항이 있음을 저장한다.
			this->notepadForm->isDirty = true;
			//3.2.4 자동 줄 바꿈 메뉴가 체크되었는지 확인한다.
			UINT state = this->notepadForm->GetMenu()->
				GetMenuState(IDM_ROW_AUTOCHANGE, MF_BYCOMMAND);
			//3.2.5 자동 줄 바꿈 메뉴가 체크되어 있으면
			if (state == MF_CHECKED)
			{
				//3.2.5.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
				//OnSize에서 부분자동개행을 하도록 한다. 
				this->notepadForm->SendMessage(WM_SIZE);
			}
		}
	}
}

//SetMacroEnd(실행취소 및 다시실행 매크로출력 종료지점 설정)
void OnCompositionCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void OnCompositionCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}


//SetRedone(다시 실행이라고 설정함)
void OnCompositionCommand::SetRedone()
{
	this->isRedone = true;
}

//줄의 위치 구하기
Long OnCompositionCommand::GetRowIndex()
{
	return this->rowIndex;
}
//글자 위치 구하기
Long OnCompositionCommand::GetLetterIndex()
{
	return this->letterIndex;
}
//실행취소 종료지점여부 구하기
bool OnCompositionCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//다시실행 종료지점여부 구하기 
bool OnCompositionCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}
//다시실행인지 여부 구하기
bool OnCompositionCommand::IsRedone()
{
	return this->isRedone;
}

//소멸자 정의
OnCompositionCommand::~OnCompositionCommand()
{

}