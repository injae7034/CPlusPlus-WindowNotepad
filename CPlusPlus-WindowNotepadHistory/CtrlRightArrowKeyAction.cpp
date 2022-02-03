#include "CtrlRightArrowKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"
#include "SelectText.h"

//디폴트생성자
CtrlRightArrowKeyAction::CtrlRightArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlRightArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 이동하기 전에 줄의 위치를 구한다.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//2. 이동하기 전에 글자의 위치를 구한다.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//3. 노트에서 단어단위로 오른쪽으로 이동한다.
	Long currentLetterIndex = this->notepadForm->note->NextWord();
	//4. 이동한 후에 줄의 위치를 구한다.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//5. 현재 줄을 이동한 후의 줄로 변경한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	//6. 줄에서 캐럿을 단어단위로 이동한다.
	this->notepadForm->current->Move(currentLetterIndex);
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
#if 0
	if (currentLetterIndex > 0)
	{
		//6.1 Shift키가 눌러졌는지 안눌러졌는지 상태를 체크해서 저장한다.
		Long ctrlPressedCheck = GetKeyState(VK_SHIFT);
		
		//6.2 Shift키가 눌러져있으면
		if (ctrlPressedCheck & 0x8000)
		{
			
		}
	}
#endif
}

//소멸자
CtrlRightArrowKeyAction::~CtrlRightArrowKeyAction()
{

}