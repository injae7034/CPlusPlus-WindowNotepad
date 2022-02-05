#include "CtrlDeleteKeyActionCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"
#include "Row.h"
#include "DummyRow.h"

//디폴트생성자 정의
CtrlDeleteKeyActionCommand::CtrlDeleteKeyActionCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{
	this->rowIndex = notepadForm->note->GetCurrent();
	this->letterIndex = notepadForm->current->GetCurrent();
	this->glyph = 0;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
	this->isDirty = false;//처음에 생성될 때는 변경사항이 없으므로 false가 디폴트값임.
}

//실행
void CtrlDeleteKeyActionCommand::Execute()
{
	//1. 현재 줄의 위치와 글자 위치를 구한다.
	Long currentRowPos = this->notepadForm->note->GetCurrent();
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	//2. OnCharCommand가 다시 실행되면
	if (this->isRedone == true)
	{
		//2.1 현재 줄의 위치와 글자위치를 재조정해준다.
		currentRowPos = this->notepadForm->note->Move(this->rowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
	}
	//3. 메모장에서 선택된 texts가 없으면
	if (this->notepadForm->isSelecting == false)
	{
		//3.1 현재 노트의 마지막 줄의 위치를 구한다.
		Long lastRowPos = this->notepadForm->note->GetLength() - 1;
		//3.2 현재 줄에서 마지막 글자의 위치를 구한다.
		Long lastLetterPos = this->notepadForm->current->GetLength();
		//제일 마지막 줄이면 줄을 지울 수 없고, 마지막 줄에서 글자 위치가 마지막에 있으면 아무것도 안일어남
		// 현재 줄의 위치가 마지막이 아니고, 현재 글자 위치가 마지막이면 다음 줄을 현재 줄로 편입시킨다.
		//3.3 현재 줄의 위치가 노트의 마지막 줄 위치보다 작고, 현재 글자 위치가 마지막이면
		if (currentRowPos < lastRowPos && currentLetterPos == lastLetterPos)
		{
			//3.3.1 현재 줄을 구한다.
			Glyph* currentRow = this->notepadForm->note->GetAt(currentRowPos);
			//3.3.2 현재 줄의 다음 줄을 구한다.
			Glyph* nextRow = this->notepadForm->note->GetAt(currentRowPos + 1);
			//3.3.3 기존에 저장된 줄이 있으면 할당해제한다.
			if (this->glyph != 0)
			{
				delete this->glyph;
			}
			//3.3.4 현재 줄의 다음 줄을 깊은 복사해서 저장한다.
			this->glyph = nextRow->Clone();
			//3.3.5 다음 줄을 현재 줄에 합친다.
			nextRow->Join(currentRow);
			//3.3.6 Note에서 다음 줄의 주소를 지운다.
			this->notepadForm->note->Remove(currentRowPos + 1);
			//3.3.7 현재 줄의 글자 위치가 지금은 마지막이기 때문에 lastLetterPos로 옮겨준다.
			currentLetterPos = this->notepadForm->current->Move(lastLetterPos);
			//3.3.8 Command에 변경 사항이 있음을 표시한다.
			this->isDirty = true;
		}
		// 현재 글자 위치가 마지막이 아닐 때(현재 줄이 마지막이든 아니든 상관없음)
		//현재 글자의 다음 글자를 지운다.
		//3.4 현재 글자 위치가 마지막이 아니면
		else if (currentLetterPos < lastLetterPos)
		{
			//3.4.1 기존에 저장된 줄이 있으면 할당해제한다.
			if (this->glyph != 0)
			{
				delete this->glyph;
			}
			//DummyRow를 생성해서 글자를 담고 그걸 복사해서 this->glyph에 주소를 옮기고,
			//글자들을 단어단위로 지우기전에 옮겨서 저장한다.
			//3.4.2 DummyRow를 생성한다.
			this->glyph = new DummyRow();
			//3.4.3 오른쪽 방향으로 단어단위로 이동한 뒤의 글자위치를 구한다.
			Long letterPosAfterMoving = this->notepadForm->current->NextWord();
			//3.4.4 오른쪽 방향으로 단어단위로 이동한 뒤의 글자위치가 현재 글자 위치보다 작은동안 반복한다.
			Glyph* letter = 0;
			while (currentLetterPos < letterPosAfterMoving)
			{
				//3.4.4.1 글자를 지우기 전에 글자를 구한다.
				letter = this->notepadForm->current->GetAt(currentLetterPos);
				//3.4.4.2 글자를 깊은 복사해서 DummyRow에 저장한다.
				this->glyph->Add(letter->Clone());
				//3.4.4.3 글자를 지운다.
				this->notepadForm->current->Remove(currentLetterPos);
				letterPosAfterMoving--;
			}
			//3.4.5 자동 줄 바꿈 메뉴가 체크되어 있으면
			if (this->notepadForm->isRowAutoChanging == true)
			{
				//3.4.5.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
				//OnSize에서 부분자동개행을 하도록 한다. 
				this->notepadForm->SendMessage(WM_SIZE);
			}
			//3.1.10 Command에 변경 사항이 있음을 표시한다.
			this->isDirty = true;
		}
		//3.5 Command에 변경 사항이 있으면
		if (this->isDirty == true)
		{
			//3.5.1 메모장 제목에 *를 추가한다.
			string name = this->notepadForm->fileName;
			name.insert(0, "*");
			name += " - 메모장";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//3.5.2 메모장에 변경사항이 있음을 저장한다.
			this->notepadForm->isDirty = true;
			//3.5.3 글자를 지운 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
			this->rowIndex = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
			this->letterIndex = this->notepadForm->current->GetCurrent();
		}
	}
	//4. 메모장에서 선택된 texts가 있으면
	else
	{
		//4.1 RemoveCommand로 메세지를 보내서 선택영역을 지운다.
		this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_REMOVE);
	}
}

//실행취소
void CtrlDeleteKeyActionCommand::Unexecute()
{
	//1. 현재 줄의 위치를 이동시킨다.(캐럿이 다른 곳에 있으면 그 곳에 글자가 지워지기 때문에)
	Long currentRowPos = this->notepadForm->note->Move(this->rowIndex);
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	//2. 현재 글자의 위치를 이동시킨다.
	Long currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
	//3. 지울 때 저장한 glyph가 줄(개행문자)이 아니면
	if (dynamic_cast<DummyRow*>(this->glyph))
	{
		//3.1 현재 줄의 글자 위치가 현재 줄의 글자개수와 같으면
		if (currentLetterPos == this->notepadForm->current->GetLength())
		{
			//3.1.1 현재 줄을 구한다.
			Glyph* currentRow = this->notepadForm->current;
			//3.1.2 dummyRow를 깊은 복사한다.
			Glyph* dummyRow = this->glyph->Clone();
			//3.1.3 dummyRow를 현재 줄에 합친다.
			dummyRow->Join(currentRow);
			//3.1.4 dummyRow를 할당해제한다.
			if (dummyRow != 0)
			{
				delete dummyRow;
			}
			//3.1.5 글자 위치를 현재 줄에서 제일 앞으로 이동시킨다.
			currentRow->Move(currentLetterPos);
		}
		//3.2 현재 줄의 글자 위치가 현재 줄의 글자개수와 다르면
		else
		{
			//3.2.1 dummyRow의 개수만큼 반복한다.
			Glyph* letter = 0;
			Long i = this->glyph->GetLength() - 1;
			while (i >= 0)
			{
				//3.2.1.1 글자를 구한다.
				letter = this->glyph->GetAt(i);
				//3.2.1.2 현재 줄의 글자 위치에 dummyRow에서 깊은 복사를 한 letter를 끼워 넣는다.
				this->notepadForm->current->Add(currentLetterPos, letter->Clone());
				//3.2.1.3 i를 감소시킨다.
				i--;
			}
			//3.2.2 글자 위치를 재조정해준다.
			currentLetterPos = this->notepadForm->current->Move(currentLetterPos);
		}
	}
	//4. 지울 때 저장한 glyph가 줄(개행문자)이면
	else
	{
		//4.1 기존에 저장된 줄이 있으면 할당해제한다.
		if (this->glyph != 0)
		{
			delete this->glyph;
		}
		//4.2. 현재 줄에서 현재 글자 다음 위치에 있는 글자들을 떼어내 새로운 줄을 만든다.
		this->glyph = this->notepadForm->current->Split(currentLetterPos);
		//4.3 현재 줄의 위치가 노트의 줄의 개수-1 과 같고(현재 줄의 위치가 마지막 줄이면)
		if (currentRowPos == this->notepadForm->note->GetLength() - 1)
		{
			//4.3.1 새로운 줄을 마지막 줄 다음에 추가한다.
			currentRowPos = this->notepadForm->note->Add(this->glyph->Clone());
		}
		//4.4 그게 아니면
		else
		{
			//4.4.1 새로운 줄을 현재 줄의 다음 위치에 끼워 넣는다.
			currentRowPos = this->notepadForm->note->
				Add(currentRowPos + 1, this->glyph->Clone());
		}
		//4.5 현재 줄을 새로 저장한다.(새로 생성된 줄이 아니라 분리한 줄을 현재 줄로 한다.)
		currentRowPos = this->notepadForm->note->Move(currentRowPos - 1);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		//4.6 현재 줄의 글자 위치를 마지막으로 이동시킨다.
		this->notepadForm->current->Last();
		//4.7 자동 줄 바꿈이 진행중이면
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//4.7.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
			//OnSize에서 부분자동개행을 하도록 한다. 
			this->notepadForm->SendMessage(WM_SIZE);
		}
	}
	//변경사항을 갱신함
	//5. isComposing을 false로 바꾼다.
	this->notepadForm->isComposing = false;
	//6. 메모장 제목에 *를 추가한다.
	string name = this->notepadForm->fileName;
	name.insert(0, "*");
	name += " - 메모장";
	this->notepadForm->SetWindowText(CString(name.c_str()));
	//7. 메모장에 변경사항이 있음을 저장한다.
	this->notepadForm->isDirty = true;
	//8. 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();
}

//SetMacroEnd(실행취소 및 다시실행 매크로출력 종료지점 설정)
void CtrlDeleteKeyActionCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void CtrlDeleteKeyActionCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}


//SetRedone(다시 실행이라고 설정함)
void CtrlDeleteKeyActionCommand::SetRedone()
{
	this->isRedone = true;
}

//줄의 위치 구하기
Long CtrlDeleteKeyActionCommand::GetRowIndex()
{
	return this->rowIndex;
}
//글자 위치 구하기
Long CtrlDeleteKeyActionCommand::GetLetterIndex()
{
	return this->letterIndex;
}
//실행취소 종료지점여부 구하기
bool CtrlDeleteKeyActionCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//다시실행 종료지점여부 구하기 
bool CtrlDeleteKeyActionCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}
//다시실행인지 여부 구하기
bool CtrlDeleteKeyActionCommand::IsRedone()
{
	return this->isRedone;
}
//변경사항이 있는지 확인 여부
bool CtrlDeleteKeyActionCommand::IsDirty()
{
	return this->isDirty;
}
//소멸자 정의
CtrlDeleteKeyActionCommand::~CtrlDeleteKeyActionCommand()
{
	//1. DummyRow나 Row를 할당해제한다.
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}