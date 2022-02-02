#include "ScrollActionCreator.h"
#include "ScrollAction.h"
#include "FirstScrollAction.h"
#include "LastScrollAction.h"
#include "PreviousScrollAction.h"
#include "NextScrollAction.h"
#include "PagePreviousScrollAction.h"
#include "PageNextScrollAction.h"
#include "ThumbPositionScrollAction.h"

//디폴트생성자
ScrollActionCreator::ScrollActionCreator(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
}

//팩토리메소드패턴
ScrollAction* ScrollActionCreator::Create(UINT nSBCode)
{
	ScrollAction* scrollAction = 0;
	//1. 스크롤을 제일 위(SB_TOP)로 움직이거나 제일 왼쪽(SB_LEFT)으로 움직이면
	//SB_TOP과 SB_LEFT의 값이 6으로 서로 같음.
	if (nSBCode == 6)
	{
		scrollAction = new FirstScrollAction(this->notepadForm);
	}
	//2. 스크롤을 제일 아래(SB_BOTTOM)로 움직이거나 제일 오른쪽(SB_RIGHT)으로 움직이면
	//SB_BOTTOM과 SB_RIGHT의 값이 7로 서로 같음.
	else if (nSBCode == 7)
	{
		scrollAction = new LastScrollAction(this->notepadForm);
	}
	//3. 스크롤을 한 줄 위(SB_LINEUP)로 올리거나 한 칸 왼쪽(SB_LINELEFT)으로 움직이면
	//SB_LINEUP과 SB_LINELEFT의 값이 0으로 서로 같음.
	else if (nSBCode == 0)
	{
		scrollAction = new PreviousScrollAction(this->notepadForm);
	}
	//4. 스크롤을 한 줄 아래(SB_LINEDOWN)로 내리거나 한 칸 오른쪽(SB_LINERIGHT)으로 움직이면
	//SB_LINEDOWN과 SB_LINERIGHT의 값이 1로 서로 같음.
	else if (nSBCode == 1)
	{
		scrollAction = new NextScrollAction(this->notepadForm);
	}
	//5. 스크롤을 한 페이지 위(SB_PAGEUP)로 올리거나 한 페이지 왼쪽(SB_PAGELEFT)으로 움직이면
	//SB_PAGEUP과 SB_PAGELEFT의 값이 2로 서로 같음.
	else if (nSBCode == 2)
	{
		scrollAction = new PagePreviousScrollAction(this->notepadForm);
	}
	//6. 스크롤을 한 페이지 아래(SB_PAGEDOWN)로 내리거나 한 페이지 오른쪽(SB_PAGERIGHT)으로 움직이면
	//SB_PAGEDOWN과 SB_PAGERIGHT의 값이 3으로 서로 같음.
	else if (nSBCode == 3)
	{
		scrollAction = new PageNextScrollAction(this->notepadForm);
	}
	//7. 스크롤을 현재 마우스가 클릭한 위치로 움직이면
	//SB_THUMBPOSITION을 쓰면 안됨! 그럼 스크롤 이동할 때 끊김이 심하다!
	else if (nSBCode == SB_THUMBTRACK)
	{
		scrollAction = new ThumbPositionScrollAction(this->notepadForm);
	}

	return scrollAction;
}

//소멸자
ScrollActionCreator::~ScrollActionCreator()
{

}