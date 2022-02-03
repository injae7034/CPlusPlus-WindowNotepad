#include "BackSpaceKeyAction.h"
#include "Glyph.h"

//디폴트생성자
BackSpaceKeyAction::BackSpaceKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void BackSpaceKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 현재 줄의 위치를 구한다.
	Long currentRowPos = this->notepadForm->note->GetCurrent();
	//2. 현재 글자의 위치를 구한다.
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	//제일 첫 줄이면 줄을 지울 수 없고, 제일 첫 줄에서 글자 위치가 제일 처음에 있으면 아무것도 안일어남
	// 현재 줄의 위치가 제일 처음이 아니고, 현재 글자 위치가 제일 처음이면 현재 줄을 다음 줄로 편입시킨다.
	//3. 현재 줄의 위치가 0보다 크고, 현재 글자 위치가 0이면
	Glyph* currentRow = 0;
	Glyph* previousRow = 0;
	if (currentRowPos > 0 && currentLetterPos == 0)
	{
		//3.1 현재 줄을 구한다.
		currentRow = this->notepadForm->note->GetAt(currentRowPos);
		//3.2 현재 줄의 이전 줄을 구한다.
		previousRow = this->notepadForm->note->GetAt(currentRowPos - 1);
		//3.3 현재 줄의 이전 줄의 마지막 글자 위치를 구한다.
		Long letterPos = previousRow->GetLength();
		//3.3 현재 줄을 이전 줄에 합친다.
		currentRow->Join(previousRow);
		//3.4 Note에서 현재 줄의 주소를 지운다.
		this->notepadForm->note->Remove(currentRowPos);
		//3.5 현재 줄이 지워졌기 때문에 현재 줄을 변경한다.
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		//3.6 현재 줄의 글자 위치가 지금은 마지막이기 때문에 변경해준다.
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
		//3.7 메모장 제목에 *를 추가한다.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - 메모장";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//3.8 메모장에 변경사항이 있음을 저장한다.
		this->notepadForm->IsDirty = true;
	}
	// 현재 글자 위치가 처음이 아닐 때(현재 줄이 처음이든 아니든 상관없음) 현재 글자를 지운다.
	//4. 현재 글자 위치가 처음이 아니면
	else if (currentLetterPos > 0)
	{
		//4.1 현재 글자를 지운다.
		this->notepadForm->current->Remove(currentLetterPos - 1);
		//4.2 메모장 제목에 *를 추가한다.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - 메모장";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//4.3 메모장에 변경사항이 있음을 저장한다.
		this->notepadForm->IsDirty = true;
	}
}


//소멸자
BackSpaceKeyAction::~BackSpaceKeyAction()
{

}