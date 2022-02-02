#include "CtrlLeftArrowKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"

//디폴트생성자
CtrlLeftArrowKeyAction::CtrlLeftArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlLeftArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	Long caretXPos = 0;
	Long caretYPos = 0;
	Long HScrollPos = 0;
	Long YScrollPos = 0;
	Long partScrollHeight = 0;
	Long distance = 0;
	//1. 현재 화면의 크기를 구한다.
	CRect rect;
	this->notepadForm->GetClientRect(&rect);
	Long pageWidth = rect.Width();
	Long pageHeight = rect.Height();
	//2. 이동하기 전에 현재 줄의 위치를 구한다.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//3. Note의 PreviousWord를 호출한다.
	Long currentCaretindex = this->notepadForm->note->PreviousWord();
	//4. 이동한 후에 현재 줄의 위치를 구한다.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//5. 현재줄을 변경한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(this->notepadForm->note->GetCurrent());
	//6. 현재줄의 캐럿의 가로 위치를 변경한다.
	Long i = this->notepadForm->current->First();
	while (i < currentCaretindex)
	{
		//6.1 캐럿을 다음으로 이동시킨다.
		i = this->notepadForm->current->Next();
	}
	//7. 이동한 뒤에 현재 캐럿의 세로 위치가 그대로이면(줄이 바뀌지X, 한 줄 내에서 이동함)
	//줄이 바뀌지 않았기 때문에 수직스크롤을 신경쓸 필요가 없고, 수평스크롤의 이동만 신경쓰면 됨)
	if (previousRowIndex == currentRowIndex)
	{
		//7.1 현재 캐럿의 가로 위치를 구한다.
		caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
			current->GetPartOfContent(currentCaretindex));
		//7.2 수평스크롤의 현재 위치를 구한다.
		HScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
		//7.3 현재 캐럿의 가로 위치가 수평스크롤의 현재위치보다 작으면
		if (caretXPos < HScrollPos)
		{
			//5.3.1 수평스크롤의 이동할 범위를 구한다.
			//<-버튼을 누를 때마다 가로스크롤의 현재 위치에서 누적해 현재 화면의 5분의 1 크기만큼 이동한다. 
			//distance = caretXPos - pageWidth + (pageWidth / 5);

			//그리고 지금 현재 스크롤이 캐럿이 없는 곳에 있을때 이동시키면 원래는 스크롤이 캐럿이
			//있는 화면으로 이동을 해야하는데 이동이 없어서 그것도 반영해야함 모든 키액션에서
			//이게 어떻게 해서 정상적으로 돌아가는지 확인해보기
			//distance값이 음수가 되면 Move의 매개변수 자료형은 UINT라서 입력하는 순간 이상한 값이
			//들어가게 되고 그 이상한 값이 scroll의 currentPos에 들어가게 되면 다시 음수로 저장된다
			//왜냐하면 scroll의 currentPos는 singned이기 때문에 하지만 스크롤에서 최소 이동은 0이고
			//음수로 이동이 불가능하기 때문에 결국 음수값을 넘겨주면 나중에 ScrollController에서 
			//GetScrollPos를 하면 음수값을 얻는게 불가능하기때문에 실패해서 0을 반환하게 되고 그래서
			//결국에 스크롤의 현재 위치가 0으로 이동하게 된다. 그래서 음수가 나와도 돌아가긴하는데
			//애초에 distance에 음수가 안들어가게 하는게 좋을거같아서 if로 음수가 되는 경우를 막자
			//수평스크롤이 왼쪽으로 이동할 범위를 구한다.
			distance = caretXPos - (pageWidth / 5);
			//distance가 0보다 작으면
			if (distance < 0)
			{
				//distance는 스크롤의 이동범위이기때문에 음수가 될 수 없고 최소값이 0이므로
				//0을 대입해준다.
				distance = 0;
			}
			

			//distance = HScrollPos - pageWidth - (pageWidth / 5);
			//distance = HScrollPos - caretXPos - (pageWidth / 5);
			//5.3.2 캐럿이 이동한 글자의 너비만큼 수평스크롤을 이동시켜준다.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//5.3.3 수평스크롤바의 수평스크롤을 이동시킨다.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
	}
	//8. 이동한 뒤에 현재 캐럿의 세로 위치가 바뀌었으면(줄이 바뀜. 현재 줄에서 이전 줄로 이동함.)
	//줄에서 줄로 이동하므로 수직스크롤을 신경써야하며 줄바뀌고 가로이동도 있기 때문에 수평스크롤도 신경써야함)
	else
	{
		//8.1 이전 줄까지의 총 세로 길이를 구한다.(캐럿의 y좌표 시작점을 구한다.)
		caretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
		//8.2 수직스크롤의 현재 위치를 구한다.
		YScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
		//8.3 이전 줄까지의 총 세로 길이가 수직스크롤위 현재 위치보다 작거나 같으면
		if (caretYPos < YScrollPos)
		{
			//8.3.1 수직스크롤을 caretYPos로 이동시킨다.
			this->notepadForm->scrollController->scroll[1]->Move(caretYPos);
			//8.3.2 수직스크롤바의 수직스크롤을 이동시킨다.
			this->notepadForm->SetScrollPos(SB_VERT,
				this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
		}
		//8.4 현재 캐럿의 가로 위치(x좌표)를 구한다.
		caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
			current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
		//8.5 수평스크롤의 현재 위치를 구한다.
		HScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
		//8.6 현재화면의 가로길이와 수평스크롤의 현재위치의 합을 구한다.
		partScrollHeight = pageHeight + HScrollPos;
		//8.6 현재 캐럿의 가로 위치가  현재화면의 가로길이와 수평스크롤의 현재위치의 합보다 크면
		if (caretXPos > partScrollHeight)
		{
			//8.6.1 캐럿이 이동한 글자의 너비만큼 수평스크롤을 이동시켜준다.
			caretXPos -= pageWidth;//수평스크롤의 현재위치가 누적되어있음!
			this->notepadForm->scrollController->scroll[0]->Move(caretXPos + 2);
			//8.6.2 수평스크롤바의 수평스크롤을 이동시킨다.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
	}

}

//소멸자
CtrlLeftArrowKeyAction::~CtrlLeftArrowKeyAction()
{

}