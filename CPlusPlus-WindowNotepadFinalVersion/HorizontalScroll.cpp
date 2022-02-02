#include "HorizontalScroll.h"
#include "NotepadForm.h"
#include "TextExtent.h"
#include "ScrollController.h"

//디폹트생성자
HorizontalScroll::HorizontalScroll(ScrollController* scrollController)
:Scroll(scrollController)
{
	
}

//매개변수를 5개 가지는 생성자
HorizontalScroll::HorizontalScroll(ScrollController* scrollController, Long currentPos,
	Long min, Long max, Long pageSize)
	:Scroll(scrollController, currentPos, min, max, pageSize)
{

}

//복사생성자
HorizontalScroll::HorizontalScroll(const HorizontalScroll& source)
	:Scroll(source)
{

}

//치환연산자
HorizontalScroll& HorizontalScroll::operator=(const HorizontalScroll& source)
{
	this->scrollController = source.scrollController;
	this->currentPos = source.currentPos;
	this->min = source.min;
	this->max = source.max;
	this->pageSize = source.pageSize;

	return *this;
}

//LinePrevious
Long HorizontalScroll::LinePrevious()
{
	//1. 현재 위치를 한글기준으로 한 칸 이전으로 이동한다.
	Long koreanLetterWidth = this->scrollController->notepadForm->textExtent->GetTextWidth("가");
	this->currentPos -= koreanLetterWidth;
	//2. 현재 위치가 음수이면
	if (this->currentPos < 0)
	{
		//2.1 현재 위치를 0으로 바꿔준다.
		this->currentPos = 0;
	}
	//3. 변경된 현재 위치를 출력한다.
	return this->currentPos;
}

//LineNext
Long HorizontalScroll::LineNext()
{
	//1. 현재 위치를 한글기준으로 한 칸 다음으로 이동한다.
	Long koreanLetterWidth = this->scrollController->notepadForm->textExtent->GetTextWidth("가");
	this->currentPos += koreanLetterWidth;
	//2. 현재 위치가 scrollMaxPos보다 크면
	Long scrollMaxPos = this->max - this->pageSize;
	if (this->currentPos > scrollMaxPos)
	{
		//2.1 현재 위치를 scrollMaxPos로 변경한다.
		this->currentPos = scrollMaxPos;
	}
	//3. 변경된 현재 위치를 출력한다.
	return this->currentPos;
}

//소멸자
HorizontalScroll::~HorizontalScroll()
{

}