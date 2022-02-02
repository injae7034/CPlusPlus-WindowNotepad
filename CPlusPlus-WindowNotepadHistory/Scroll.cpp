#include "Scroll.h"
#include "ScrollController.h"

//디폴트생성자
Scroll::Scroll()
{
	this->scrollController = 0;
	this->currentPos = 0;
	this->min = 0;
	this->max = 0;
	this->pageSize = 0;
}

//매개변수를 5개 가지는 생성자
Scroll::Scroll(ScrollController* scrollController, Long currentPos, Long min, Long max,
	Long pageSize)
{
	this->scrollController = scrollController;
	this->currentPos = currentPos;
	this->min = min;
	this->max = max;
	this->pageSize = pageSize;
}

//복사생성자
Scroll::Scroll(const Scroll& source)
{
	this->scrollController = source.scrollController;
	this->currentPos = source.currentPos;
	this->min = source.min;
	this->max = source.max;
	this->pageSize = source.pageSize;
}

//치환연산자
Scroll& Scroll::operator=(const Scroll& source)
{
	this->scrollController = source.scrollController;
	this->currentPos = source.currentPos;
	this->min = source.min;
	this->max = source.max;
	this->pageSize = source.pageSize;

	return *this;
}

//First
Long Scroll::First()
{
	//1. 현재 위치를 처음으로 이동시킨다.
	this->currentPos = this->min;
	//2. 현재 위치를 출력한다.
	return this->currentPos;
}

//Last
Long Scroll::Last()
{
	//1. 현재 위치를 마지막으로 이동시킨다.
	this->currentPos = this->max;
	//2. 현재 위치를 출력한다.
	return this->currentPos;
}

//LinePrevious
Long Scroll::LinePrevious()
{
	//1. 현재 위치를 한글기준으로 한글자 왼쪽으로 또는 한줄 위로 이동시킨다.
	return this->currentPos;
}

//LineNext
Long Scroll::LineNext()
{
	return this->currentPos;
}

//pagePrevious
Long Scroll::pagePrevious()
{
	return this->currentPos;;
}

//pageNext
Long Scroll::pageNext()
{
	return this->currentPos;
}

//Move
Long Scroll::Move(Long currentPos)
{
	return this->currentPos;
}

//소멸자
Scroll::~Scroll()
{

}