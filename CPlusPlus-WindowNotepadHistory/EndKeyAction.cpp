#include "EndKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"

//디폴트생성자
EndKeyAction::EndKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void EndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 현재 화면의 가로 길이를 구한다.
	CRect rect;
	this->notepadForm->GetClientRect(&rect);
	Long pageWidth = rect.Width();
	//2. 현재 줄의 맨 마지막으로 캐럿의 가로 위치를 이동시킨다.
	this->notepadForm->current->Last();
	//3. 현재 줄의 캐럿의 가로 위치를 구한다.
	Long caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
		current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
	//4. 스크롤이 이동해야할 범위를 구한다.
	Long currentScrollPos = caretXPos - pageWidth;
	//5. 캐럿이 이동한 글자의 너비만큼 수평스크롤을 이동시켜준다.
	this->notepadForm->scrollController->scroll[0]->Move(currentScrollPos);
	//6. 수평스크롤바의 수평스크롤을 이동시킨다.
	this->notepadForm->SetScrollPos(SB_HORZ,
		this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
}

//소멸자
EndKeyAction::~EndKeyAction()
{

}