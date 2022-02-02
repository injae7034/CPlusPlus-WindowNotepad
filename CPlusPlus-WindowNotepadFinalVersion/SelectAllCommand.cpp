#include "SelectAllCommand.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "NotepadForm.h"

//디폴트생성자
SelectAllCommand::SelectAllCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{

}

//Execute
void SelectAllCommand::Execute()
{
	//1. 메모장의 노트에서 줄의 개수를 구한다.
	Long rowCountOfNote = this->notepadForm->note->GetLength();
	//2. 메모장의 노트의 마지막 줄의 글자 개수를 구한다.
	Long letterCountOfFirstRow = this->notepadForm->note->GetAt(rowCountOfNote - 1)->GetLength();
	//3. 메모장의 노트에 줄의 개수가 하나있는데 그 줄의 글자가 하나도 없는 경우가 아니라면
	//글자가 하나도 없고 줄만 2개이상 있는 경우부터는 선택이 가능하기 떄문에 복사, 잘라내기, 삭제가 가능하다.
	if (rowCountOfNote != 1 || letterCountOfFirstRow != 0)
	{
		//3.1 이전에 있던 글자 선택을 해제한다.
		this->notepadForm->selectingTexts->Undo();
		//3.2 마지막 줄로 이동하기 전에 줄의 위치를 저장한다.
		Long previousRowIndex = 0;
		//3.3 마지막으로 글자로 이동하기 전에 글자의 위치를 저장한다.
		Long previousLetterIndex = 0;
		//3.4 선택이 진행되고 있는 중으로 상태를 바꾼다.
		this->notepadForm->isSelecting = true;
		//3.5 선택이 시작되는 캐럿의 x좌표를 저장한다.
		this->notepadForm->selectedStartXPos = previousLetterIndex;
		//3.6 선택이 시작되는 캐럿의 y좌표를 저장한다.
		this->notepadForm->selectedStartYPos = previousRowIndex;
		//3.7 메모장의 제일 마지막 줄로 이동한다.
		Long currentRowIndex = this->notepadForm->note->Last();
		//3.8 메모장의 현재 줄을 변경한다.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
		//3.9 현재 줄의 마지막 글자로 이동한다.
		Long currentLetterIndex = this->notepadForm->current->Last();
		//3.10 전체 글자를 선택한다.
		this->notepadForm->selectingTexts->DoNext(previousRowIndex, previousLetterIndex,
			currentRowIndex, currentLetterIndex);
		//3.11 메모장 texts 전체가 선택되었기 때문에 복사하기와 잘라내기, 삭제를 활성화 시켜준다.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
	}
}

//소멸자
SelectAllCommand::~SelectAllCommand()
{

}