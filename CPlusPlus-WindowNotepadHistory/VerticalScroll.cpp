#include "VerticalScroll.h"
#include "ScrollController.h"
#include "TextExtent.h"
#include "Glyph.h"

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

//PagePrevious
Long VerticalScroll::PagePrevious()
{
	Long distance = 0;//페이지단위로 이동할 거리
	Long i = 0;//줄의 개수

	//1. 현재 글꼴의 높이를 구한다.
	Long textHeight = this->scrollController->notepadForm->textExtent->GetHeight();
	//2. 이동할 거리가 현재 화면의 세로 길이보다 작거나 같은동안
	//그리고 i가 메모장의 줄의 개수보다 작거나 같은동안 반복한다.
	while (distance <= this->pageSize &&
		i <= this->scrollController->notepadForm->note->GetLength())
	{
		//2.1 줄의 개수를 누적시킨다.
		i++;
		//2.2 줄의 개수에 줄의 평균 높이를 곱해서 이동할 거리를 구한다.
		distance = textHeight * i;
	}
	//2. 이동할 거리가 현재화면의 세로길이보다 크고 메모장의 줄의 개수보다 작거나 같으면
	if (distance > this->pageSize &&
		i <= this->scrollController->notepadForm->note->GetLength())
	{
		//2.1 이동할 거리를 재조정해준다.
		i--;
		distance = textHeight * i;
	}
	//3. i가 줄의 개수보다 크면
	else if (i > this->scrollController->notepadForm->note->GetLength())
	{
		//3.1 이동할 거리를 재조정해준다.
		distance = 0;
	}
	//4. 현재 위치에 이동할 거리만큼 빼준다.
	this->currentPos -= distance;
	//5. 현재 위치가 0보다 작으면
	if (this->currentPos < 0)
	{
		//5.1 현재 위치를 0으로 변경한다.
		this->currentPos = 0;
	}
	//6. 현재 위치를 출력한다.
	return this->currentPos;
}

//PageNext
Long VerticalScroll::PageNext()
{
	Long distance = 0;//페이지단위로 이동할 거리
	Long i = 0;//줄의 개수

	//1. 현재 글꼴의 높이를 구한다.
	Long textHeight = this->scrollController->notepadForm->textExtent->GetHeight();
	//2. 이동할 거리가 현재 화면의 세로 길이보다 작거나 같은동안
	//그리고 i가 메모장의 줄의 개수보다 작거나 같은동안 반복한다.
	while (distance <= this->pageSize &&
		i <= this->scrollController->notepadForm->note->GetLength())
	{
		//2.1 줄의 개수를 누적시킨다.
		i++;
		//2.2 줄의 개수에 줄의 평균 높이를 곱해서 이동할 거리를 구한다.
		distance = textHeight * i;
	}
	//2. 이동할 거리가 현재화면의 세로길이보다 크고 메모장의 줄의 개수보다 작거나 같으면
	if (distance > this->pageSize &&
		i <= this->scrollController->notepadForm->note->GetLength())
	{
		//2.1 이동할 거리를 재조정해준다.
		i--;
		distance = textHeight * i;
	}
	//3. i가 줄의 개수보다 크면
	else if (i > this->scrollController->notepadForm->note->GetLength())
	{
		//3.1 이동할 거리를 재조정해준다.
		distance = 0;
	}
	//4. 현재 위치에 이동할 거리만큼 더해준다.
	this->currentPos += distance;
	//5. 현재 위치가 scrollMaxPos보다 크면
	Long scrollMaxPos = this->max - this->pageSize;
	if (this->currentPos > scrollMaxPos)
	{
		//5.1 현재 위치를 scrollMaxPos로 변경한다.
		this->currentPos = scrollMaxPos;
	}
	//6. 현재 위치를 출력한다.
	return this->currentPos;
}

//소멸자
VerticalScroll::~VerticalScroll()
{

}