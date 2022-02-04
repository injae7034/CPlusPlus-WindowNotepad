#include "CopyCommand.h"
#include "NotepadForm.h"
#include "Note.h"
#include "DummyRow.h"

//디폴트생성자
CopyCommand::CopyCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{

}

//Execute
void CopyCommand::Execute()
{
	//복사하기 Command에 들어왔다는 말은 최소 한글자이상은 복사할게 있어서 들어왔다는 뜻임
	//복사할게 없었으면 애초에 여기에 들어올 수 없음 그러므로 복사하기의 Execute에서는
	//노트를 생성하고 줄을 한 개 생성해서 새로 생성한 노트에 추가까지 하는게 디폴트이다.
	//1. 복사할 텍스트를 담을 새노트를 생성한다.
	Glyph* copyNote = new Note();
	//2. 줄을 생성한다.(복사한 내용이 한 글자이든 수천줄이든 일단 무조건 줄은 하나 생성해야함.)
	//그리고 복사를 시작한 줄이 가짜줄이든 진짜줄이든 상관없이 처음 복사한 줄은 무조건 Row로 생성함. 
	Glyph* copyRow = new Row();
	//3. 새로 생성한 줄을 클립보드의 새로 생성한 노트에 추가한다.
	Long rowIndexOfCopyNote = copyNote->Add(copyRow);
	//4. 메모장에서 현재 줄과 글자의 위치를 구한다.
	Long currentRowPos = this->notepadForm->note->GetCurrent();
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	//5. 선택이 시작되는 줄과 글자, 선택이 끝나는 줄과 글자의 위치를 구한다.
	Long selectedStartRowPos = this->notepadForm->selectedStartYPos;//선택이 시작되는 줄
	Long selectedStartLetterPos = this->notepadForm->selectedStartXPos;//선택이 시작되는 글자
	Long selectedEndRowPos = currentRowPos;//선택이 끝나는 줄
	Long selectedEndLetterPos = currentLetterPos;//선택이 끝나는 글자
	//6. 선택의 방향과 상관없이 오른쪽 방향(다음)으로 시작하는 줄과 글자, 끝나는 줄과 글자위치를 정한다.
	Long startRowIndex = 0;//시작하는 줄의 위치
	Long startLetterIndex = 0;//시작하는 글자 위치
	Long endRowIndex = 0;//끝나는 줄의 위치
	Long endLetterIndex = 0;//끝나는 글자 위치
	Long i = 0;//반복제어변수
	Glyph* startRow = 0;//시작하는 줄의 위치
	Glyph* letter = 0;
	//7. 선택이 시작되는 줄과 선택이 끝나는 줄이 같으면(복사가 한 줄 내에서 이뤄졌으면)
	if (selectedStartRowPos == selectedEndRowPos)
	{
		//7.1.1 선택이 오른쪽으로 진행되었으면
		if (selectedStartLetterPos < selectedEndLetterPos)
		{
			//7.1.1.1 시작하는 글자위치를 선택이 시작되는 글자위치로 정한다.
			startLetterIndex = selectedStartLetterPos;
			//7.1.1.2 끝나는 글자위치를 선택이 끝나는 글자위치로 정한다.
			endLetterIndex = selectedEndLetterPos;
			//7.1.1.3 시작하는 줄의 위치를 선택이 시작하는 줄의 위치로 정한다.
			startRowIndex = selectedStartRowPos;

		}
		//7.1.2 선택이 왼쪽으로 진행되었으면
		else
		{
			//7.1.2.1 시작하는 글자위치를 선택이 끝나는 글자위치로 정한다.
			startLetterIndex = selectedEndLetterPos;
			//7.1.2.2 끝나는 글자위치를 선택이 시작하는 글자위치로 정한다.
			endLetterIndex = selectedStartLetterPos;
			//7.1.2.3 시작하는 줄의 위치를 선택이 끝나는 줄의 위치로 정한다.
			startRowIndex = selectedEndRowPos;
		}
		//7.1.3 시작하는 줄을 구한다.
		startRow = this->notepadForm->note->GetAt(startRowIndex);
		//7.1.4 시작하는 글자위치부터 끝나는 글자까지 복사한다.
		i = startLetterIndex;
		while (i < endLetterIndex)
		{
			//7.1.4.1 시작하는 줄에서 줄에서 글자를 구한다.
			letter = startRow->GetAt(i);
			//7.1.4.2 글자가 선택이 되어있는데 클립보드에 생성된 새로운 노트에 옮길 때는 선택이 안된
			//상태로 복사해야함. 그래야 나중에 붙여넣기할때 붙여넣는 내용들이 선택이 안된 상태로 붙여넣어짐!
			letter->Select(false);
			//7.1.4.3 새로 만든 줄(복사하는 줄)에 글자를 추가한다.
			copyRow->Add(letter->Clone());
			//7.1.4.4 시작하는 글자의 다음 글자로 이동해서 글자를 복사한다.
			i++;
		}
	}
	//8. 선택이 시작하는 줄과 선택이 끝나는 줄이 서로 다르면(선택이 여러줄에 걸쳐져서 되어 있으면)
	else
	{
		//8.1 선택이 오른쪽으로 진행되었으면 
		if (selectedStartRowPos < selectedEndRowPos)
		{
			//8.1.1 시작하는 글자위치를 선택이 시작되는 글자위치로 정한다.
			startLetterIndex = selectedStartLetterPos;
			//8.1.2 끝나는 글자위치를 선택이 끝나는 글자위치로 정한다.
			endLetterIndex = selectedEndLetterPos;
			//8.1.3 시작하는 줄의 위치를 선택이 시작하는 줄의 위치로 정한다.
			startRowIndex = selectedStartRowPos;
			//8.1.4 끝나는 줄의 위치를 선택이 끝나는 줄의 위치로 정한다.
			endRowIndex = selectedEndRowPos;
		}
		//8.2 선택이 왼쪽으로 진행되었으면
		else
		{
			//8.2.1 시작하는 글자위치를 선택이 끝나는 글자위치로 정한다.
			startLetterIndex = selectedEndLetterPos;
			//8.2.2 끝나는 글자위치를 선택이 시작하는 글자위치로 정한다.
			endLetterIndex = selectedStartLetterPos;
			//8.2.3 시작하는 줄의 위치를 선택이 끝나는 줄의 위치로 정한다.
			startRowIndex = selectedEndRowPos;
			//8.2.4 끝나는 줄의 위치를 선택이 시작하는 줄의 위치로 정한다.
			endRowIndex = selectedStartRowPos;
		}
		Glyph* endRow = 0;//끝나는 줄의 위치
		Glyph* row = 0;//줄의 주소를 담을 공간
		Long letterIndex = 0;//글자 위치
		//8.3 시작하는 줄을 구한다.
		startRow = this->notepadForm->note->GetAt(startRowIndex);
		//8.4 시작하는 글자위치부터 시작하는 줄의 마지막 글자까지 복사한다.
		i = startLetterIndex;
		while (i < startRow->GetLength())
		{
			//8.4.1 시작하는 줄에서 글자를 구한다.
			letter = startRow->GetAt(i);
			//8.4.2 글자가 선택이 되어있는데 클립보드에 생성된 새로운 노트에 옮길 때는 선택이 안된
			//상태로 복사해야함. 그래야 나중에 붙여넣기할때 붙여넣는 내용들이 선택이 안된 상태로 붙여넣어짐!
			letter->Select(false);
			//8.4.3 새로 만든 줄(복사하는 줄)에 글자를 추가한다.
			copyRow->Add(letter->Clone());
			//8.4.4 시작하는 글자의 다음 글자로 이동해서 글자를 복사한다.
			i++;
		}
		//8.5 시작하는 줄의 다음줄로 이동한다.
		Long nextRowIndex = startRowIndex + 1;
		Glyph* realRow = copyNote->GetAt(rowIndexOfCopyNote);//진짜 줄의 주소를 담을 공간
		//8.6 다음줄부터 끝나는 줄전까지 반복한다.
		while (nextRowIndex < endRowIndex)
		{
			//8.6.1 메모장의 줄을 구한다.
			row = this->notepadForm->note->GetAt(nextRowIndex);
			//8.6.2 메모장의 줄이 진짜이면
			if (!dynamic_cast<DummyRow*>(row))
			{
				//8.6.2.1 진짜 줄을 생성한다.
				realRow = new Row();
				//8.6.2.2 진짜 줄을 새로 만든 노트에 추가한다.
				rowIndexOfCopyNote = copyNote->Add(realRow);
			}
			//8.6.3 메모장의 진짜 줄의 처음부터 마지막 글자까지 반복한다.
			letterIndex = 0;
			while (letterIndex < row->GetLength())
			{
				//8.6.3.1 메모장의 진짜 줄에서 글자를 구한다.
				letter = row->GetAt(letterIndex);
				//8.6.3.2 글자가 선택이 되어있는데 클립보드에 생성된 새로운 노트에 옮길 때는
				//선택이 안된 상태로 복사해야함. 그래야 나중에 붙여넣기할때 
				//붙여넣는 내용들이 선택이 안된 상태로 붙여넣어짐!
				letter->Select(false);
				//8.6.3.3 새로 만든 노트의 새로 만든 진짜 줄에 글자를 추가한다.
				realRow->Add(letter->Clone());
				//8.6.3.4 다음 글자로 이동해서 글자를 복사한다..
				letterIndex++;
			}
			//8.6.4 다음 줄로 이동한다.
			nextRowIndex++;
		}
		//8.7 메모장에서 끝나는 줄을 구한다.
		endRow = this->notepadForm->note->GetAt(nextRowIndex);
		//8.8 메모장에서 끝나는 줄이 진짜줄이면
		if (!dynamic_cast<DummyRow*>(endRow))
		{
			//8.8.1 진짜 줄을 생성한다.
			realRow = new Row();
			//8.8.2 진짜 줄을 새로 만든 노트에 추가한다.
			rowIndexOfCopyNote = copyNote->Add(realRow);
		}
		//새로운 노트의 진짜 줄에 메모장의 가짜줄의 글자를 복사한다.
		//8.9 글자 위치를 원위치시킨다.
		letterIndex = 0;
		//8.10 메모장의 가짜줄에서 마지막 글자까지 반복한다.
		while (letterIndex < endLetterIndex)
		{
			//8.10.1 메모장의 가짜줄에서 글자를 구한다.
			letter = endRow->GetAt(letterIndex);
			//8.10.2 글자가 선택이 되어있는데 클립보드에 생성된 새로운 노트에 옮길 때는 선택이 안된
			//상태로 복사해야함. 그래야 나중에 붙여넣기할때 붙여넣는 내용들이 선택이 안된 상태로 붙여넣어짐!
			letter->Select(false);
			//8.10.3 새로운 노트의 진짜 줄에 글자를 순차적으로 추가한다.
			realRow->Add(letter->Clone());
			//8.10.4 다음 글자로 이동해서 글자를 복사한다.
			letterIndex++;
		}
	}
	//9. notepadForm의 clipboard에 새로 생성한 Note(복사한 내용)를 추가시켜준다.
	this->notepadForm->clipboard->Add(copyNote);
	//10. 복사한 texts가 생겼기 때문에 붙여넣기 메뉴를 활성화시켜준다.
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_PASTE, MF_BYCOMMAND | MF_ENABLED);
}

//소멸자
CopyCommand::~CopyCommand()
{

}