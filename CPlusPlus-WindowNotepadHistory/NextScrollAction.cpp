#include "NextScrollAction.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"

//디폴트생성자
NextScrollAction::NextScrollAction(NotepadForm* notepadForm)
	:ScrollAction(notepadForm)
{

}

void NextScrollAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
		//3.1 스크롤을 한글기준으로 한칸 다음으로 이동한다.
		Long position = this->notepadForm->textExtent->GetTextWidth("가");
		//dynamic_cast<ScrollController*>(observer)->scroll[0]->MoveNext(position);
	}
}

void NextScrollAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
		//3.1 스크롤을 한줄 아래로 이동한다.
		Long position = this->notepadForm->textExtent->GetHeight();
		//dynamic_cast<ScrollController*>(observer)->scroll[1]->MoveNext(position);
	}
}

//소멸자
NextScrollAction::~NextScrollAction()
{

}