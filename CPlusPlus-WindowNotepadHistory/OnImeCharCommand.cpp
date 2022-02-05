#include "OnImeCharCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"

//디폴트생성자 정의
OnImeCharCommand::OnImeCharCommand(NotepadForm* notepadForm, Glyph* glyph)
	:Command(notepadForm)
{
	this->glyph = glyph;
	this->rowIndex = notepadForm->note->GetCurrent();
	this->letterIndex = notepadForm->current->GetCurrent();
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
}

//실행
void OnImeCharCommand::Execute()
{
	//1. 현재 줄의 글자 위치를 구한다.
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	//다시 실행이면 캐럿의 위치를 조정
	//2. OnCompositionCommand가 다시 실행되면
	if (this->isRedone == true)
	{
		Long currentRowPos = this->notepadForm->note->Move(this->rowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
	}
	//3. 처음 실행이면(앞에 조립중인 한글을 지워야하고, 다시 실행이면 앞에 조립중인 한글이 없다!)
	else
	{
		//3.1 현재 줄의 캐럿의 가로 위치 바로 앞에 있는 기존 한글을 지운다.
		// 그러기 위해서는 캐럿의 현재 가로 위치에 1감소한 값을 넣어주면 된다.
		this->notepadForm->current->Remove(currentLetterPos - 1);
		//3.2 갱신된 current의 위치를 index에 저장한다.
		currentLetterPos = this->notepadForm->current->GetCurrent();
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
	//6. IsComposing을 false로 바꾼다.
	this->notepadForm->isComposing = false;
	//7. 메모장 제목에 *를 추가한다.
	string name = this->notepadForm->fileName;
	name.insert(0, "*");
	name += " - 메모장";
	this->notepadForm->SetWindowText(CString(name.c_str()));
	//8. 메모장에 변경사항이 있음을 저장한다.
	this->notepadForm->isDirty = true;
	//9. 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();
}

//실행취소
void OnImeCharCommand::Unexecute()
{
	//1. 현재 줄의 위치를 이동시킨다.(캐럿이 다른 곳에 있으면 그 곳에 글자가 지워지기 때문에)
	Long currentRowPos = this->notepadForm->note->Move(this->rowIndex);
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	//2. 현재 글자의 위치를 이동시킨다.
	Long currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
	//3. 현재 글자를 지운다.
	this->notepadForm->current->Remove(currentLetterPos - 1);
	//4. 자동 줄 바꿈이 진행중이면
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//4.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
		//OnSize에서 부분자동개행을 하도록 한다. 
		this->notepadForm->SendMessage(WM_SIZE);
	}
	//5. 메모장 제목에 *를 추가한다.
	string name = this->notepadForm->fileName;
	name.insert(0, "*");
	name += " - 메모장";
	this->notepadForm->SetWindowText(CString(name.c_str()));
	//6. 메모장에 변경사항이 있음을 저장한다.
	this->notepadForm->isDirty = true;
	//7. 글자를 지운 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();
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

//줄의 위치 구하기
Long OnImeCharCommand::GetRowIndex()
{
	return this->rowIndex;
}
//글자 위치 구하기
Long OnImeCharCommand::GetLetterIndex()
{
	return this->letterIndex;
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

//소멸자 정의
OnImeCharCommand::~OnImeCharCommand()
{
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}