#include "VerticalScroll.h"

//디폴트생성자
VerticalScroll::VerticalScroll()
	:Scroll()
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

//소멸자
VerticalScroll::~VerticalScroll()
{

}