#include "RowAutoChangeCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "TextExtent.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "DummyRow.h"
#include "RowAutoChange.h"
#include "SelectingTexts.h"

//디폴트생성자
RowAutoChangeCommand::RowAutoChangeCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{
	
}

//Execute
void RowAutoChangeCommand::Execute()
{
	//1. 선택이 진행되고 있는 중이었으면
	if (this->notepadForm->isSelecting == true)
	{
		//1.1. 선택된 텍스트를 선택해제한다.(선택을 끝낸다.)
		this->notepadForm->selectingTexts->Undo();
		//1.2 선택이 끝난 상태로 바꾼다.
		this->notepadForm->isSelecting = false;
		//1.3 선택이 끝났기 때문에 캐럿의 x좌표를 0으로 저장한다.
		this->notepadForm->selectedStartXPos = 0;
		//1.4 선택이 끝났기 때문에 캐럿의 y좌표를 0으로 저장한다.
		this->notepadForm->selectedStartYPos = 0;
	}
	//2. 선택한 texts들을 다 선택취소했으면
	if (this->notepadForm->isSelecting == false)
	{
		//2.1 복사하기, 잘라내기, 삭제 메뉴를 비활성화 시킨다.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
	//3. RowAutoChange를 생성한다.(힙에 할당하면 나중에 따로 할당해제를 해줘야함
	//그러나 주소없이 스택에 할당하면 이 함수 스택이 종료되면 자동으로 같이 사라짐.)
	//여기서는 이 스택에서만 RowAutoChange의 연산을 쓰기 위한것이기 때문에 스택에 할당하는게 효율적임!
	RowAutoChange rowAutoChange(this->notepadForm);
	//4. 자동 줄 바꿈 메뉴가 체크되었는지 확인한다.
	UINT state = this->notepadForm->GetMenu()->
		GetMenuState(IDM_ROW_AUTOCHANGE, MF_BYCOMMAND);
	//5. 자동 줄 바꿈 메뉴가 체크되어 있지 않으면
	if (state == MF_UNCHECKED)
	{
		//5.1 자동 줄 바꿈 메뉴를 체크해준다.
		this->notepadForm->GetMenu()->
			CheckMenuItem(IDM_ROW_AUTOCHANGE, MF_CHECKED | MF_BYCOMMAND);
		//5.2 자동개행을 실행한다.
		rowAutoChange.DoAllRows();
		this->notepadForm->isRowAutoChanging = true;
	}
	//6. 자동 줄 바꿈 메뉴가 체크되어 있으면
	else
	{
		//6.1 자동 줄 바꿈 메뉴에 있는 체크를 없앤다.	
		this->notepadForm->GetMenu()->
			CheckMenuItem(IDM_ROW_AUTOCHANGE, MF_UNCHECKED | MF_BYCOMMAND);
		//6.2 자동개행을 취소한다.
		rowAutoChange.UndoAllRows();
		this->notepadForm->isRowAutoChanging = false;
	}
	//7. 캐럿을 제일 처음으로 보낸다.
	this->notepadForm->note->First();
	this->notepadForm->current = this->notepadForm->note->
		GetAt(this->notepadForm->note->GetCurrent());
	this->notepadForm->current->First();
}


//소멸자
RowAutoChangeCommand::~RowAutoChangeCommand()
{

}