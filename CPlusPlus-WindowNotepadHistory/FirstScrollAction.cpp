#include "FirstScrollAction.h"
#include "ScrollController.h"
#include "Scroll.h"

//디폴트생성자
FirstScrollAction::FirstScrollAction(NotepadForm* notepadForm)
	:ScrollAction(notepadForm)
{

}

void FirstScrollAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. 옵저버 리스트 배열에서 해당위치의 옵저버를 구한다.
	Long i = 0;
	Observer* observer = this->notepadForm->GetAt(i);
	//2. i가 length보다 작은 동안 옵저버가 캐럿매니저가 아닌동안 반복한다.
	while (i < this->notepadForm->GetLength()
		&& dynamic_cast<ScrollController*>(observer) != observer)
	{
		//2.1 옵저버 리스트에서 옵저버를 구한다.
		observer = this->notepadForm->GetAt(i);
		//2.2 i를 증가시킨다.
		i++;
	}
	//3. ScrollController이면
	if (dynamic_cast<ScrollController*>(observer))
	{
		//3.1 스크롤을 처음으로 이동한다.
		dynamic_cast<ScrollController*>(observer)->scroll[0]->First();
	}
}

void FirstScrollAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. 옵저버 리스트 배열에서 해당위치의 옵저버를 구한다.
	Long i = 0;
	Observer* observer = this->notepadForm->GetAt(i);
	//2. i가 length보다 작은 동안 옵저버가 스크롤매니저가 아닌동안 반복한다.
	while (i < this->notepadForm->GetLength()
		&& dynamic_cast<ScrollController*>(observer) != observer)
	{
		//2.1 옵저버 리스트에서 옵저버를 구한다.
		observer = this->notepadForm->GetAt(i);
		//2.2 i를 증가시킨다.
		i++;
	}
	//3. ScrollController이면
	if (dynamic_cast<ScrollController*>(observer))
	{
		//3.1 스크롤을 맨 위로 이동한다.
		dynamic_cast<ScrollController*>(observer)->scroll[1]->First();
	}
}

//소멸자
FirstScrollAction::~FirstScrollAction()
{

}