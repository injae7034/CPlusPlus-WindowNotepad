#include "RightArrowKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"
#include "SelectText.h"

//디폴트생성자
RightArrowKeyAction::RightArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void RightArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 다음으로 이동하기 전에 글자의 위치를 저장한다.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//2. 다음으로 이동하기 전에 줄의 위치를 저장한다.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//3. 다음으로 이동하고 현재 글자의 위치를 반환받는다.
	Long currentLetterIndex = this->notepadForm->current->Next();
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//4. 캐럿의 이동이 없었으면(캐럿이 줄의 마지막에 위치하고 있었기 때문에)
	if (previousLetterIndex == currentLetterIndex)
	{
		//4.1 다음 줄로 이동시킨다.
		currentRowIndex = this->notepadForm->note->Next();
		//4.2 줄의 이동이 있었으면(이동하기 전의 줄이 노트의 마지막 줄이 아니면)
		if (previousRowIndex != currentRowIndex)
		{
			//4.2.1 현재 줄을 다음으로 이동한 줄로 변경한다.
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
			//4.2.2 캐럿의 현재 위치를 처음으로 보낸다.(현재 줄이 다음 줄로 옮겨졌기 때문에
			//캐럿은 이전 줄의 마지막 위치에서 다음 줄의 처음 위치로 이동한다.)
			currentLetterIndex = this->notepadForm->current->First();
		}
	}
	//5. 화면에서 선택여부를 확인하기 위해 SelectText를 생성한다.
	SelectText selectText(this->notepadForm);
	//6. Shift키가 눌러졌는지 안눌러졌는지 상태를 체크해서 저장한다.
	Long ctrlPressedCheck = GetKeyState(VK_SHIFT);
	//7. Shift키가 눌러져있으면
	if (ctrlPressedCheck & 0x8000)
	{
		//7.1 텍스트를 선택한다.
		selectText.Do(previousRowIndex, previousLetterIndex, currentRowIndex,
			currentLetterIndex);
	}
	//8. Shift키가 안눌러져있으면
	else
	{
		//8.1 텍스트를 선택해제한다.
		selectText.Undo();
	}
}

//소멸자
RightArrowKeyAction::~RightArrowKeyAction()
{

}

