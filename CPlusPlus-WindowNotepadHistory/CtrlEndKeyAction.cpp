#include "CtrlEndKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"

//디폴트생성자
CtrlEndKeyAction::CtrlEndKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 메모장의 제일 마지막으로 캐럿의 세로 위치를 이동시킨다.
	Long lastIndex = this->notepadForm->note->Last();
	//2. 메모장의 현재 줄을 변경한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(lastIndex);
	//3. 현재 줄의 맨 마지막으로 캐럿의 가로 위치를 이동시킨다.
	this->notepadForm->current->Last();
	//4.3.3 현재 줄까지 총 세로 길이를 구한다.(캐럿의 세로 위치를 구한다.)
	Long caretYPos = this->notepadForm->textExtent->GetHeight() * (lastIndex + 1);
	//4. 수직스크롤을 마지막으로 이동시켜준다.
	this->notepadForm->scrollController->scroll[1]->Move(caretYPos);
	//5. 수평스크롤바의 수평스크롤을 이동시킨다.
	this->notepadForm->SetScrollPos(SB_VERT,
		this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
	//6. 현재 화면의 가로 길이를 구한다.
	CRect rect;
	this->notepadForm->GetClientRect(&rect);
	Long pageWidth = rect.Width();
	//7. 현재 줄의 캐럿의 가로 위치를 구한다.
	Long caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
		current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
	//8. 수평스크롤이 이동해야할 범위를 구한다.
	Long currentScrollPos = caretXPos - pageWidth;
	//9. 캐럿이 이동한 글자의 너비만큼 수평스크롤을 이동시켜준다.
	this->notepadForm->scrollController->scroll[0]->Move(currentScrollPos + 2);
	//10. 수평스크롤바의 수평스크롤을 이동시킨다.
	this->notepadForm->SetScrollPos(SB_HORZ,
		this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
}

//소멸자
CtrlEndKeyAction::~CtrlEndKeyAction()
{

}