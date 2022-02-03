#include "DeleteKeyAction.h"
#include "Glyph.h"

//디폴트생성자
DeleteKeyAction::DeleteKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void DeleteKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 현재 줄의 위치를 구한다.
	Long currentRowPos = this->notepadForm->note->GetCurrent();
	//2. 현재 글자의 위치를 구한다.
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	//3. 현재 노트의 마지막 줄의 위치를 구한다.
	Long lastRowPos = this->notepadForm->note->GetLength() - 1;
	//4. 현재 줄에서 마지막 글자의 위치를 구한다.
	Long lastLetterPos = this->notepadForm->current->GetLength();
	//제일 마지막 줄이면 줄을 지울 수 없고, 마지막 줄에서 글자 위치가 마지막에 있으면 아무것도 안일어남
	// 현재 줄의 위치가 마지막이 아니고, 현재 글자 위치가 마지막이면 다음 줄을 현재 줄로 편입시킨다.
	//5. 현재 줄의 위치가 노트의 마지막 줄 위치보다 작고, 현재 글자 위치가 마지막이면
	Glyph* currentRow = 0;
	Glyph* nextRow = 0;
	if (currentRowPos < lastRowPos && currentLetterPos == lastLetterPos)
	{
		//5.1 현재 줄을 구한다.
		currentRow = this->notepadForm->note->GetAt(currentRowPos);
		//5.2 현재 줄의 다음 줄을 구한다.
		nextRow = this->notepadForm->note->GetAt(currentRowPos + 1);
		//5.3 다음 줄을 현재 줄에 합친다.
		nextRow->Join(currentRow);
		//5.4 Note에서 다음 줄의 주소를 지운다.
		this->notepadForm->note->Remove(currentRowPos + 1);
		//5.5 현재 줄의 글자 위치가 지금은 마지막이기 때문에 변경해준다.
		Long index = this->notepadForm->current->First();
		while (index < lastLetterPos)
		{
			this->notepadForm->current->Next();
			index++;
			//이렇게하면 index는 절대 overflow가 되지 않기 때문에 반복문을 벗어날 수 없게되고,
			//그럼 결국에 무한반복이 된다.!!
			//index = this->current->Next();
		}
		//5.6 메모장 제목에 *를 추가한다.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - 메모장";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//5.7 메모장에 변경사항이 있음을 저장한다.
		this->notepadForm->IsDirty = true;
	}
	// 현재 글자 위치가 마지막이 아닐 때(현재 줄이 마지막이든 아니든 상관없음)
	//현재 글자의 다음 글자를 지운다.
	//6. 현재 글자 위치가 마지막이 아니면
	else if (currentLetterPos < lastLetterPos)
	{
		//6.1 현재 글자의 다음 글자를 지운다.
		this->notepadForm->current->Remove(currentLetterPos);
		//6.2 메모장 제목에 *를 추가한다.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - 메모장";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//6.3 메모장에 변경사항이 있음을 저장한다.
		this->notepadForm->IsDirty = true;
	}
}

//소멸자
DeleteKeyAction::~DeleteKeyAction()
{

}