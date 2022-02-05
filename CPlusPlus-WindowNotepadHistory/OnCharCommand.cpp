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
}

//Execute 정의
void OnCharCommand::Execute()
{
	//1. 메모장에서 선택된 texts가 있으면
	if (this->notepadForm->isSelecting == true)
	{
		//1.1 RemoveCommand로 메세지를 보내서 선택영역을 지운다.
		this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_REMOVE);
	}

	//현재 줄의 위치와 글자위치를 이동시킨다.
	Long currentRowPos = this->notepadForm->note->Move(this->rowIndex);
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	this->notepadForm->current->Move(this->letterIndex);

	//2. glyphCreator를 생성한다.
	GlyphCreator glyphCreator;
	//this->nChar = this->notepadForm->GetNChar();
	//3. glyph를 생성한다.
	Glyph* glyph = glyphCreator.Create((char*)&this->nChar);
	Long letterIndex;
	Long rowIndex;
	//4. 입력받은 문자가 개행문자가 아니면
	if (this->nChar != '\n' && this->nChar != '\r')
	{
		//4.1 현재 줄의 캐럿의 가로 위치를 구한다.
		letterIndex = this->notepadForm->current->GetCurrent();
		//4.2 FileSaveCommand가 현재 줄의 length와 같으면
		if (letterIndex == this->notepadForm->current->GetLength())
		{
			//4.2.1 현재 줄의 마지막 글자 뒤에 새로운 글자를 추가한다.
			letterIndex = this->notepadForm->current->Add(glyph);
		}
		//4.3 index가 현재 줄의 length와 다르면
		else
		{
			//4.3.1 현재 줄의 index번째에 새로운 글자를 끼워 쓴다.
			letterIndex = this->notepadForm->current->Add(letterIndex, glyph);
		}
	}
	//5. 입력받은 문자가 개행문자이면
	else if (this->nChar == '\n' || this->nChar == '\r')
	{
		//5.1 현재 줄의 위치를 구한다.
		rowIndex = this->notepadForm->note->GetCurrent();
		//5.2 현재 줄의 캐럿의 위치를 구한다.
		letterIndex = this->notepadForm->current->GetCurrent();
		//5.3. 현재 줄에서 현재 캐럿 다음 위치에 있는 글자들을 떼어낸다.
		glyph = this->notepadForm->current->Split(letterIndex);
		//5.4 rowIndex가 노트의 줄의 개수-1 과 같고(현재 줄의 위치가 마지막 줄이면)
		if (rowIndex == this->notepadForm->note->GetLength() - 1)
		{
			//5.4.1 새로운 줄을 마지막 줄 다음에 추가한다.
			rowIndex = this->notepadForm->note->Add(glyph);
		}
		//5.5 그게 아니면
		else
		{
			//5.5.1 새로운 줄을 현재 줄의 다음 위치에 끼워넣는다.
			rowIndex = this->notepadForm->note->Add(rowIndex + 1, glyph);
		}
		//5.4 현재 줄의 위치를 새로 저장한다.
		this->notepadForm->current = this->notepadForm->note->GetAt(rowIndex);
		//5.5 현재 줄의 캐럿의 위치를 처음으로 이동시킨다.
		this->notepadForm->current->First();
		//5.6 자동 줄 바꿈이 진행중이면
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//5.6.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
			//OnSize에서 부분자동개행을 하도록 한다. 
			this->notepadForm->SendMessage(WM_SIZE);
		}
	}
	//6. 캐럿의 위치와 크기가 변경되었음을 알린다.
	this->notepadForm->Notify();
	//7. isComposing을 false로 바꾼다.
	this->notepadForm->isComposing = false;
	//8. 메모장 제목에 *를 추가한다.
	string name = this->notepadForm->fileName;
	name.insert(0, "*");
	name += " - 메모장";
	this->notepadForm->SetWindowText(CString(name.c_str()));
	//9. 메모장에 변경사항이 있음을 저장한다.
	this->notepadForm->isDirty = true;
	//10. 갱신한다.
	this->notepadForm->Invalidate(TRUE);
	//11. 현재 글자 위치를 저장한다.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->letterIndex = this->notepadForm->current->GetCurrent() - 1;
}

//Unexcute
void OnCharCommand::Unexecute()
{
	//1. 현재 줄의 위치를 이동시킨다.
	Long currentRowPos = this->notepadForm->note->Move(this->rowIndex);
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	//2. 현재 글자의 위치를 이동시킨다.
	Long currentLetterPos = this->notepadForm->current->Move(this->letterIndex + 1);
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
			//3.1.5 Note에서 현재 줄의 주소를 지운다.
			this->notepadForm->note->Remove(currentRowPos);
			//3.1.6 줄이 지워졌기 때문에 rowPosBeforeUndo를 한 줄 앞당긴다.
			currentRowPos = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			//3.1.7 현재 줄의 글자 위치가 지금은 마지막이기 때문에 변경해준다.
			//이전 줄의 마지막 현재 줄의 처음 사이에 위치하도록 조정한다.
			Long index = this->notepadForm->current->First();
			while (index < letterPos)
			{
				this->notepadForm->current->Next();
				index++;
				//이렇게하면 index는 절대 overflow가 되지 않기 때문에 반복문을 벗어날 수 없게되고,
				//그럼 결국에 무한반복이 된다.!!
				//index = this->current->Next();
			}
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

//소멸자 정의
OnCharCommand::~OnCharCommand()
{

}