#include "PasteCommand.h"
#include "NotepadForm.h"
#include "Note.h"
#include "DummyRow.h"

//디폴트생성자
PasteCommand::PasteCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{

}

//Execute
void PasteCommand::Execute()
{
	//1. 메모장의 현재 줄과 글자의 위치를 구한다.
	Long currentRowPos = this->notepadForm->note->GetCurrent();
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	//2. 메모장의 클립보드에서 현재 노트(복사한 내용)를 호출한다.
	Glyph* copyNote = this->notepadForm->clipboard->
		GetAt(this->notepadForm->clipboard->GetCurrent());
	//3. 메모장의 현재 줄을 구한다.
	Glyph* currentRow = this->notepadForm->note->GetAt(currentRowPos);
	//4. 메모장의 현재 줄에서 현재 글자위치 다음부터 split시킨다.
	Glyph* splitedRow = currentRow->Split(currentLetterPos);
	//5. 클립보드의 현재 노트의 첫번째 줄을 구한다.
	Long firstCopyRowPos = 0;
	Glyph* firstCopyRow = copyNote->GetAt(firstCopyRowPos);
	Glyph* letter = 0;//글자를 담을 공간
	//5. 클립보드의 현재 노트의 첫번째 줄의 개수보다 작은동안 반복한다.
	Long letterIndex = 0;
	while (letterIndex < firstCopyRow->GetLength())
	{
		//5.1 클립보드의 현재 노트의 첫번째 줄의 글자를 구한다.
		letter = firstCopyRow->GetAt(letterIndex);
		//5.2 글자를 메모장의 현재 줄에 추가한다.(깊은 복사)
		currentRow->Add(letter->Clone());
		//5.3 다음 글자로 이동한다.
		letterIndex++;
	}
	//6. 클립보드의 현재 노트의 첫번째 줄에서 다음 줄로 이동한다.
	Long nextCopyRowPos = firstCopyRowPos + 1;
	Glyph* copyRow = firstCopyRow;//첫번째 줄이 디폴트
	Long i = currentRowPos;
	//7. 클립보드의 현재 노트(복사한 내용)의 마지막줄까지 반복한다.
	while (nextCopyRowPos < copyNote->GetLength())
	{
		//7.1 복사한 줄을 구한다.
		copyRow = copyNote->GetAt(nextCopyRowPos);
		//7.2 메모장의 현재 줄의 다음 줄로 이동한다.
		i++;
		//7.3 구한 줄을 메모장의 현재 줄의 위치 다음부터 끼워넣는다.(깊은 복사)
		i = this->notepadForm->note->Add(i, copyRow->Clone());
		//7.4 복사한 노트에서 다음 줄로 이동한다.
		nextCopyRowPos++;
	}
	//8. 메모장에서 현재 줄을 구한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(i);
	//9. 메모장에서 현재 글자위치를 구한다.
	currentLetterPos = this->notepadForm->current->GetCurrent();
	//10. 메모장에서 현재 줄에 아까 split했던 글자들을 추가한다.
	letterIndex = 0;
	while (letterIndex < splitedRow->GetLength())
	{
		//10.1 글자를 구한다.
		letter = splitedRow->GetAt(letterIndex);
		//10.2 마지막 줄에 글자를 깊은 복사를 한다.
		this->notepadForm->current->Add(letter->Clone());
		//10.3 다음 글자로 이동한다.
		letterIndex++;
	}
	//11. 메모장에서 현재 글자위치를 조정한다.
	this->notepadForm->current->Move(currentLetterPos);
	//12. 메모장 제목에 *를 추가한다.
	string name = this->notepadForm->fileName;
	name.insert(0, "*");
	name += " - 메모장";
	this->notepadForm->SetWindowText(CString(name.c_str()));
	//13. 메모장에 변경사항이 있음을 저장한다.
	this->notepadForm->isDirty = true;
	//14. 변경사항을 갱신한다.
	this->notepadForm->Notify();
	this->notepadForm->Invalidate(TRUE);
}

//소멸자
PasteCommand::~PasteCommand()
{

}