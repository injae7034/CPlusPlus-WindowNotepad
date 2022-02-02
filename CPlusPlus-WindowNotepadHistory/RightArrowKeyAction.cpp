#include "RightArrowKeyAction.h"
#include "Glyph.h"

//디폴트생성자
RightArrowKeyAction::RightArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void RightArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 다음으로 이동하기 전에 캐럿의 현재 가로위치를 저장한다.
	Long previousIndex = this->notepadForm->current->GetCurrent();
	//2. 다음으로 이동하고 현재 캐럿의 가로위치를 반환받는다.
	Long currentIndex = this->notepadForm->current->Next();
	//3. 캐럿의 이전 가로위치와 캐럿의 현재 가로위치가 같으면
	if (previousIndex == currentIndex)
	{
		//3.1 다음으로 이동하기 전에 캐럿의 현재 세로위치를 저장한다.
		Long previousRowIndex = this->notepadForm->note->GetCurrent();
		//3.2 캐럿의 현재 세로 위치를 다음 줄로 이동시킨다.
		Long currentRowIndex = this->notepadForm->note->Next();
		//3.3 캐럿의 이전 세로 위치와 캐럿의 현재 세로 위치가 서로 다르면
		if (previousRowIndex != currentRowIndex)
		{
			//3.3.1 캐럿의 현재 줄을 변경한다.
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
			//3.3.2 캐럿의 현재 가로 위치를 변경한다.
			this->notepadForm->current->First();
		}
	}
}

//소멸자
RightArrowKeyAction::~RightArrowKeyAction()
{

}

