#include "HorizontalScroll.h"

//디폹트생성자
HorizontalScroll::HorizontalScroll()
:Scroll()
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

//소멸자
HorizontalScroll::~HorizontalScroll()
{

}