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
	//1. 현재 화면의 크기를 구한다.
	CRect rect;
	this->notepadForm->GetClientRect(&rect);
	Long pageWidth = rect.Width();
	Long pageHeight = rect.Height();
	Long currentScrollPos = 0;
	Long caretYPos = 0;
	Long caretXPos = 0;
	Long partScrollWidth = 0;
	Long partScrollHeight = 0;
	Long distance = 0;
	//2. 다음으로 이동하기 전에 캐럿의 현재 가로위치를 저장한다.
	Long previousIndex = this->notepadForm->current->GetCurrent();
	//3. 다음으로 이동하고 현재 캐럿의 가로위치를 반환받는다.
	Long currentIndex = this->notepadForm->current->Next();
	//4. 캐럿의 이전 가로위치와 캐럿의 현재 가로위치가 같으면(캐럿의 이동이 없었으면)
	//이동이 없었다는 말은 캐럿이 현재 줄의 마지막 글자뒤에 있어서 더이상 이동할 곳이 없다는 뜻이다.
	if (previousIndex == currentIndex)
	{
		//4.1 다음으로 이동하기 전에 캐럿의 현재 세로위치(현재 줄의 위치)를 저장한다.
		Long previousRowIndex = this->notepadForm->note->GetCurrent();
		//4.2 캐럿의 현재 세로 위치(현재 줄의 위치)를 다음 줄로 이동시킨다.
		Long currentRowIndex = this->notepadForm->note->Next();
		//4.3 캐럿의 이전 세로 위치와 캐럿의 현재 세로 위치가 서로 다르면(현재 줄의 이동이 있었으면)
		//줄의 이동이 있었다는 말은 이동하기 전의 줄이 마지막 줄이 아니기 때문에 이동이 가능함.
		//현재 줄이 마지막 줄이었으면 다음 줄로 이동 후에도 현재 줄과 위치가 같기 때문에 이동한게아님.
		if (previousRowIndex != currentRowIndex)
		{
			//4.3.1 캐럿의 현재 줄을 변경한다.
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
			//4.3.2 캐럿의 현재 가로 위치를 변경한다.(현재 줄이 다음 줄로 옮겨졌기 때문에
			//캐럿은 이전 줄의 마지막 위치에서 다음 줄의 처음 위치로 이동한다.)
			this->notepadForm->current->First();
			//4.3.3 현재 줄까지 총 세로 길이를 구한다.(캐럿의 세로 위치를 구한다.)
			caretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);
			//4.3.4 수직스크롤의 현재 위치를 구한다.
			currentScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
			//4.3.5 현재화면의 세로길이와 수직스크롤의 현재위치의 합을 구한다.
			partScrollHeight = pageHeight + currentScrollPos;
			//4.3.6 현재 줄까지의 총 세로 길이가 현재화면의 세로길이와 수직스크롤의 현재위치의 합보다 크면
			if (caretYPos > partScrollHeight)
			{

				//4.3.6.1 수직스크롤을 한줄 아래로 이동시킨다.
				//this->notepadForm->scrollController->scroll[1]->LineNext();

				//4.3.6.1 수직스크롤이 이동할 범위를 정한다. 
				//caretYPos -= pageHeight;//이렇게해야 정확히 한 줄씩 이동함.
				distance = caretYPos - pageHeight;//distance값을 구할 때 누적 된 값을 구해야함!
				//distance = caretYPos - partScrollHeight;
				//partScrollHeight는 현재화면의 세로길이와 현재수직스크롤의 위치를 합한 값인데
				//이 값을 빼주면 distance는 항상 일정한 값이 되어 누적이 없음.
				//Move연산은 distance만큼 더해서(누적시켜서) 이동시키는게 아니라
				//그냥 입력받은 distance로 이동하는 것이기 때문에 애초에 distance값을 구할 때
				//누적된 값을 구해야함. 그래서 현재 캐럿의 세로 위치에서 현재 화면크기 값만 빼주면
				//현재수직스크롤의현재위치값은 남아있어서 한줄 높이가 누적된 distance를 구할 수 있다.
				this->notepadForm->scrollController->scroll[1]->Move(distance);
				//4.3.6.2 수직스크롤바의 수직스크롤을 이동시킨다.
				this->notepadForm->SetScrollPos(SB_VERT,
					this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
			}
			//4.3.7 수평스크롤을 처음으로 이동시킨다.
			this->notepadForm->scrollController->scroll[0]->First();
			//4.3.8 수평스크롤바의 수평스크롤을 이동시킨다.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
	}
	//5. 캐럿의 이전 가로위치와 캐럿의 현재 가로위치가 다르면(캐럿의 이동이 있었으면)
	//캐럿이 이동했다는 것은 캐럿이 이동하기 전에 줄의 마지막 위치에 있지 않았다는 뜻임
	//따라서 캐럿은 Next연산을 통해 정상적으로 한 칸 이동했기 때문에 수평스크롤만 이동할지 말지 확인하면 됨
	else
	{
		//5.1 현재 캐럿의 위치를 구한다.
		caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
			current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
		//5.2 수평스크롤의 현재 위치를 구한다. 
		currentScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
		//5.3 현재화면의 가로길이와 수평스크롤의 현재 위치를 더한다.
		partScrollWidth = pageWidth + currentScrollPos;
		//5.4 현재 캐럿의 위치가 현재화면의 가로길이와 수평스크롤의 현재 위치를 더한 값보다 크면
		if (caretXPos > partScrollWidth)
		{
			
			//5.4.1 수평스크롤이 이동할 범위를 정한다.
			//caretXPos -= pageWidth;
			//스크롤이 글자 한 칸씩 이동하면 이동이 너무 짧아서 이동할 때 사용자 입장에서 너무 답답함! 
			
			//5.4.1 수평스크롤이 이동할 범위를 정한다.
			//->버튼을 누를 때마다 가로스크롤의 현재 위치에서 누적해 현재 화면의 5분의 1 크기만큼 이동한다. 
			distance = (pageWidth / 5) + currentScrollPos;
			//5.4.2 캐럿이 이동한 글자의 너비만큼 수평스크롤을 이동시켜준다.
			this->notepadForm->scrollController->scroll[0]->Move(distance);
			//5.4.3 수평스크롤바의 수평스크롤을 이동시킨다.
			this->notepadForm->SetScrollPos(SB_HORZ,
				this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
		}
	}
}

//소멸자
RightArrowKeyAction::~RightArrowKeyAction()
{

}

