#include "OnCharCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"

//디폴트생성자 정의
OnCharCommand::OnCharCommand(NotepadForm* notepadForm, UINT nChar,
	Long rowIndex, Long letterIndex)
	:Command(notepadForm)
{
	this->nChar = nChar;
	this->rowIndex = rowIndex;
	this->letterIndex = letterIndex;
	this->startSplitIndex = 0;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
}

//Execute 정의
void OnCharCommand::Execute()
{
	//선택영역이 있으면 선택영역 삭제
	//1. 메모장에서 선택된 texts가 있으면
	if (this->notepadForm->isSelecting == true)
	{
		//1.1 RemoveCommand로 메세지를 보내서 선택영역을 지운다.
		this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_REMOVE);
	}
	Long currentRowPos = 0;

	//다시 실행이면 캐럿의 위치를 조정
	//2. OnCharCommand가 다시 실행되면
	if (this->isRedone == true)
	{
		//2.1 OnCharCommand의 글자가 개행문자이면
		if (this->nChar == '\n' || this->nChar == '\r')
		{
			//2.1.1 OnCharCommand의 줄의 위치를 한 줄 이전으로 이동한다.(개행이 될 때 줄의 위치가
			//한 줄 증가되었고, 취소되었기 때문에 다시 실행하기 전에 한 줄 이전으로 이동시켜야함)
			this->rowIndex--;
			currentRowPos = this->notepadForm->note->Move(this->rowIndex);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			//2.1.2 개행이 취소되었기 때문에 다시 개행이 되기 전에 칸의 원래 위치로 이동시킨다.
			this->letterIndex = this->notepadForm->current->Move(this->startSplitIndex);
		}
		//2.2 OnCharCommand의 글자가 개행문자가 아니면
		else
		{
			currentRowPos = this->notepadForm->note->Move(this->rowIndex);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			//2.2.1 글자가 써지면 한 칸 이동이 되는데, 글자가 취소되었기 때문에 한 칸 이전으로 이동시킨다.
			this->letterIndex--;
			this->notepadForm->current->Move(this->letterIndex);
		}
	}

	//글자생성해서 노트와 줄에 추가함
	//3. glyphCreator를 생성한다.
	GlyphCreator glyphCreator;
	//4. glyph를 생성한다.
	Glyph* glyph = glyphCreator.Create((char*)&this->nChar);
	Long letterIndex;
	Long rowIndex;
	//5. 입력받은 문자가 개행문자가 아니면
	if (this->nChar != '\n' && this->nChar != '\r')
	{
		//5.1 현재 줄의 글자 위치를 구한다.
		letterIndex = this->notepadForm->current->GetCurrent();
		//5.2 현재 줄의 글자 위치가 현재 줄의 글자개수와 같으면
		if (letterIndex == this->notepadForm->current->GetLength())
		{
			//5.2.1 현재 줄의 마지막 글자 뒤에 새로운 글자를 추가한다.
			letterIndex = this->notepadForm->current->Add(glyph);
		}
		//5.3 현재 줄의 글자 위치가 현재 줄의 글자개수와 다르면
		else
		{
			//5.3.1 현재 줄의 글자 위치에 글자를 끼워서 추가한다.
			letterIndex = this->notepadForm->current->Add(letterIndex, glyph);
		}
	}
	//6. 입력받은 문자가 개행문자이면
	else if (this->nChar == '\n' || this->nChar == '\r')
	{
		//6.1 현재 줄의 위치를 구한다.
		rowIndex = this->notepadForm->note->GetCurrent();
		//6.2 현재 줄의 캐럿의 위치를 구한다.
		letterIndex = this->notepadForm->current->GetCurrent();
		this->startSplitIndex = letterIndex;
		//6.3. 현재 줄에서 현재 글자 다음 위치에 있는 글자들을 떼어내 새로운 줄을 만든다.
		glyph = this->notepadForm->current->Split(letterIndex);
		//6.4 현재 줄의 위치가 노트의 줄의 개수-1 과 같고(현재 줄의 위치가 마지막 줄이면)
		if (rowIndex == this->notepadForm->note->GetLength() - 1)
		{
			//6.4.1 새로운 줄을 마지막 줄 다음에 추가한다.
			rowIndex = this->notepadForm->note->Add(glyph);
		}
		//6.5 그게 아니면
		else
		{
			//6.5.1 새로운 줄을 현재 줄의 다음 위치에 끼워 넣는다.
			rowIndex = this->notepadForm->note->Add(rowIndex + 1, glyph);
		}
		//6.6 현재 줄을 새로 저장한다.
		this->notepadForm->current = this->notepadForm->note->GetAt(rowIndex);
		//6.7 현재 줄의 글자 위치를 처음으로 이동시킨다.
		this->notepadForm->current->First();
		//7.8 자동 줄 바꿈이 진행중이면
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//6.8.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
			//OnSize에서 부분자동개행을 하도록 한다. 
			this->notepadForm->SendMessage(WM_SIZE);
		}
	}

	//변경사항을 갱신함
	//7. 캐럿의 위치와 크기가 변경되었음을 알린다.
	this->notepadForm->Notify();
	//8. isComposing을 false로 바꾼다.
	this->notepadForm->isComposing = false;
	//9. 메모장 제목에 *를 추가한다.
	string name = this->notepadForm->fileName;
	name.insert(0, "*");
	name += " - 메모장";
	this->notepadForm->SetWindowText(CString(name.c_str()));
	//10. 메모장에 변경사항이 있음을 저장한다.
	this->notepadForm->isDirty = true;
	//11. 갱신한다.
	this->notepadForm->Invalidate(TRUE);
	//12. 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();
}

//Unexcute
void OnCharCommand::Unexecute()
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
void OnCharCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void OnCharCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}

//SetRedone(다시 실행이라고 설정함)
void OnCharCommand::SetRedone()
{
	this->isRedone = true;
}

//줄의 위치 구하기
Long OnCharCommand::GetRowIndex()
{
	return this->rowIndex;
}
//글자 위치 구하기
Long OnCharCommand::GetLetterIndex()
{
	return this->letterIndex;
}
//실행취소 종료지점여부 구하기
bool OnCharCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//다시실행 종료지점여부 구하기 
bool OnCharCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}
//다시실행인지 여부 구하기
bool OnCharCommand::IsRedone()
{
	return this->isRedone;
}

//소멸자 정의
OnCharCommand::~OnCharCommand()
{

}