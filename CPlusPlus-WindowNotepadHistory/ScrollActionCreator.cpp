#include "ScrollActionCreator.h"
#include "ScrollAction.h"
#include "FirstScrollAction.h"
#include "LastScrollAction.h"
#include "PreviousScrollAction.h"
#include "NextScrollAction.h"

//nSBCode == SB_LINEUP || nSBCode == SB_LINELEFT 일 경우  SB_LINEPREVIOUS로 구분한다.
//#define SB_LINEPREVIOUS 0

//디폴트생성자
ScrollActionCreator::ScrollActionCreator(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
}

//팩토리메소드패턴
ScrollAction* ScrollActionCreator::Create(UINT nSBCode)
{
	ScrollAction* scrollAction = 0;
	//if(nSBCode == SB_LINEPREVIOUS)
	//1. 스크롤을 제일 위로 움직이거나 제일 왼쪽으로 움직이면
	if (nSBCode == SB_TOP || nSBCode == SB_LEFT)
	{
		scrollAction = new FirstScrollAction(this->notepadForm);
	}
	//2. 스크롤을 제일 아래로 움직이거나 제일 오른쪽으로 움직이면
	else if (nSBCode == SB_BOTTOM || nSBCode == SB_RIGHT)
	{
		scrollAction = new LastScrollAction(this->notepadForm);
	}
	//3. 스크롤을 한 줄 위로 올리거나 한 칸 왼쪽으로 움직이면
	else if (nSBCode == SB_LINEUP || nSBCode == SB_LINELEFT)
	{
		scrollAction = new PreviousScrollAction(this->notepadForm);
	}
	//4. 스크롤을 한 줄 아래로 내리거나 한 칸 오른쪽으로 움직이면
	else if (nSBCode == SB_LINEDOWN || nSBCode == SB_LINERIGHT)
	{
		scrollAction = new LastScrollAction(this->notepadForm);
		//scrollAction = new NextScrollAction(this->notepadForm);
	}
	//scrollAction = new FirstScrollAction(this->notepadForm);

	return scrollAction;
}

//소멸자
ScrollActionCreator::~ScrollActionCreator()
{

}