#include "RowAutoChangeCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "TextExtent.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "DummyRow.h"
#include "RowAutoChange.h"

//디폴트생성자
RowAutoChangeCommand::RowAutoChangeCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{
	
}

//Execute
void RowAutoChangeCommand::Execute()
{
	//1. RowAutoChange를 생성한다.(힙에 할당하면 나중에 따로 할당해제를 해줘야함
	//그러나 주소없이 스택에 할당하면 이 함수 스택이 종료되면 자동으로 같이 사라짐.)
	//여기서는 이 스택에서만 RowAutoChange의 연산을 쓰기 위한것이기 때문에 스택에 할당하는게 효율적임!
	RowAutoChange rowAutoChange(this->notepadForm);
	//2. 자동 줄 바꿈 메뉴가 체크되었는지 확인한다.
	UINT state = this->notepadForm->GetMenu()->
		GetMenuState(IDM_ROW_AUTOCHANGE, MF_BYCOMMAND);
	//3. 자동 줄 바꿈 메뉴가 체크되어 있지 않으면
	if (state == MF_UNCHECKED)
	{
		//3.1 자동 줄 바꿈 메뉴를 체크해준다.
		this->notepadForm->GetMenu()->
			CheckMenuItem(IDM_ROW_AUTOCHANGE, MF_CHECKED | MF_BYCOMMAND);
		//3.2 자동개행을 실행한다.
		rowAutoChange.Do();
	}
	//3. 자동 줄 바꿈 메뉴가 체크되어 있으면
	else
	{
		//3.1 자동 줄 바꿈 메뉴에 있는 체크를 없앤다.	
		this->notepadForm->GetMenu()->
			CheckMenuItem(IDM_ROW_AUTOCHANGE, MF_UNCHECKED | MF_BYCOMMAND);
		//3.2 자동개행을 취소한다.
		rowAutoChange.Undo();
	}
	//4. 캐럿을 제일 처음으로 보낸다.
	this->notepadForm->note->First();
	this->notepadForm->current = this->notepadForm->note->
		GetAt(this->notepadForm->note->GetCurrent());
	this->notepadForm->current->First();
	//5. 캐럿의 위치가 변경되었음을 알린다.
	this->notepadForm->Notify();
	//6. 변경사항을 갱신한다.
	this->notepadForm->Invalidate(TRUE);
}


//소멸자
RowAutoChangeCommand::~RowAutoChangeCommand()
{

}