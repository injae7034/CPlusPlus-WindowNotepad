#include "CtrlRightArrowKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"

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
	//2. 노트에서 단어단위로 오른쪽으로 이동한다.
	Long currentCaretindex = this->notepadForm->note->NextWord();
	//3. 이동한 후에 줄의 위치를 구한다.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//4. 현재 줄을 이동한 후의 줄로 변경한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	//5. 줄에서 캐럿을 단어단위로 이동한다.
	Long i = this->notepadForm->current->First();
	while (i < currentCaretindex)
	{
		//5.1 캐럿을 다음으로 이동시킨다.
		i = this->notepadForm->current->Next();
	}
}

//소멸자
CtrlRightArrowKeyAction::~CtrlRightArrowKeyAction()
{

}