#include "VerticalScroll.h"
#include "ScrollController.h"
#include "TextExtent.h"

//디폴트생성자
VerticalScroll::VerticalScroll(ScrollController* scrollController)
	:Scroll(scrollController)
{
	
}

//매개변수를 5개 가지는 생성자
VerticalScroll::VerticalScroll(ScrollController* scrollController, Long currentPos, Long min,
	Long max, Long pageSize)
	:Scroll(scrollController, currentPos, min, max, pageSize)
{

}

//복사생성자
VerticalScroll::VerticalScroll(const VerticalScroll& source)
	:Scroll(source)
{

}

//치환연산자
VerticalScroll& VerticalScroll::operator=(const VerticalScroll& source)
{
	this->scrollController = source.scrollController;
	this->currentPos = source.currentPos;
	this->min = source.min;
	this->max = source.max;
	this->pageSize = source.pageSize;

	return *this;
}

//LinePrevious
Long VerticalScroll::LinePrevious()
{
	//1. 현재 위치를 한 줄 위로 이동시킨다.
	this->currentPos -= this->scrollController->notepadForm->textExtent->GetHeight();
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
Long VerticalScroll::LineNext()
{
	//1. 현재 위치를 한 줄 아래로 이동시킨다.
	this->currentPos += this->scrollController->notepadForm->textExtent->GetHeight();
	//2. 현재 위치가 max보다 크면
	Long max = this->max - this->pageSize;
	if (this->currentPos > max)
	{
		//2.1 현재 위치를 max로 변경한다.
		this->currentPos = max;
	}
	//3. 변경된 현재 위치를 출력한다.
	return this->currentPos;
}

//소멸자
VerticalScroll::~VerticalScroll()
{

}