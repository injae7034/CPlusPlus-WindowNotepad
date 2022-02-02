#include "Scroll.h"
#include "ScrollController.h"

//디폴트생성자
Scroll::Scroll(ScrollController* scrollController)
{
	this->scrollController = scrollController;
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

//First 수평수직스크롤 동일함
Long Scroll::First()
{
	//1. 현재 위치를 처음으로 이동시킨다.
	this->currentPos = this->min;
	//2. 현재 위치를 출력한다.
	return this->currentPos;
}

//Last 수평수직스크롤 동일함
Long Scroll::Last()
{
	//1. 현재 위치를 마지막으로 이동시킨다.
	this->currentPos = this->max;
	//2. 현재 위치를 출력한다.
	return this->currentPos;
}

//LinePrevious 수평수직스크롤 서로 다르므로 HorizontalScroll과 VerticalScroll에서 각자 정의해줌
Long Scroll::LinePrevious()
{
	return -1;
}

//LineNext 수평수직스크롤 서로 다르므로 HorizontalScroll과 VerticalScroll에서 각자 정의해줌
Long Scroll::LineNext()
{
	return -1;
}

//pagePrevious
Long Scroll::PagePrevious()
{
	//1. 현재 위치에 화면크기만큼 빼준다.
	this->currentPos -= this->pageSize;
	//2. 현재 위치가 음수이면
	if (this->currentPos < 0)
	{
		//2.1 현재 위치를 0으로 바꿔준다.
		this->currentPos = 0;
	}
	//3. 현재 위치를 출력한다.
	return this->currentPos;
}

//pageNext
Long Scroll::PageNext()
{
	//1. 현재 위치에 화면크기만큼 더해준다.
	this->currentPos += this->pageSize;
	//2. 현재 위치가 max보다 크면
	Long max = this->max - this->pageSize;
	if (this->currentPos > max)
	{
		//2.1 현재 위치를 max로 변경한다.
		this->currentPos = max;
	}
	//3. 현재 위치를 출력한다.
	return this->currentPos;
}

//MoveP
Long Scroll::Move(UINT nPos)
{
	//1. 현재 위치를 스크롤에서 마우스로 클릭한 위치로 이동시킨다. 
	this->currentPos = nPos;
	//2. 현재 위치를 출력한다.
	return this->currentPos;
}

//소멸자
Scroll::~Scroll()
{

}