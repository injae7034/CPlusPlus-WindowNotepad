#include "RightArrowKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"

//디폴트생성자
RightArrowKeyAction::RightArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void RightArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 다음으로 이동하기 전에 캐럿의 위치를 저장한다.
	Long previousCaretIndex = this->notepadForm->current->GetCurrent();
	//2. 다음으로 이동하고 현재 캐럿의 위치를 반환받는다.
	Long currentCaretIndex = this->notepadForm->current->Next();
	//3. 다음으로 이동하기 전 캐럿의 위치와 다음으로 이동한 후의 캐럿의 위치가 같으면(캐럿의 이동이 없었으면)
	//이동이 없었다는 말은 캐럿이 줄의 마지막 글자 뒤에 있어서 더이상 이동할 곳이 없다는 뜻이다.
	if (previousCaretIndex == currentCaretIndex)
	{
		//3.1 다음으로 이동하기 전에 줄의 위치를 저장한다.
		Long previousRowIndex = this->notepadForm->note->GetCurrent();
		//3.2 현재 줄의 위치를 다음 줄로 이동시킨다.
		Long currentRowIndex = this->notepadForm->note->Next();
		//3.3 다음으로 이동하기 전에 줄의 위치와 다음으로 이동한 후에 줄의 위치가 서로 다르면
		//(현재 줄의 이동이 있었으면)
		//줄의 이동이 있었다는 말은 이동하기 전의 줄이 마지막 줄이 아니기 때문에 이동이 가능함.
		//현재 줄이 마지막 줄이었으면 다음 줄로 이동 후에도 현재 줄과 위치가 같기 때문에 이동한게아님.
		//줄의 마지막에서 다음줄 처음으로 이동
		if (previousRowIndex != currentRowIndex)
		{
			//3.3.1 현재 줄을 다음으로 이동한 줄로 변경한다.
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
			//3.3.2 캐럿의 현재 위치를 처음으로 보낸다.(현재 줄이 다음 줄로 옮겨졌기 때문에
			//캐럿은 이전 줄의 마지막 위치에서 다음 줄의 처음 위치로 이동한다.)
			this->notepadForm->current->First();
		}
	}
}

//소멸자
RightArrowKeyAction::~RightArrowKeyAction()
{

}

