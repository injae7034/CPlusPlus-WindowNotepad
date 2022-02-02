#include "HorizontalScroll.h"

//��bƮ������
HorizontalScroll::HorizontalScroll()
:Scroll()
{
	
}

//�Ű������� 5�� ������ ������
HorizontalScroll::HorizontalScroll(ScrollController* scrollController, Long currentPos,
	Long min, Long max, Long pageSize)
	:Scroll(scrollController, currentPos, min, max, pageSize)
{

}

//���������
HorizontalScroll::HorizontalScroll(const HorizontalScroll& source)
	:Scroll(source)
{

}

//ġȯ������
HorizontalScroll& HorizontalScroll::operator=(const HorizontalScroll& source)
{
	this->scrollController = source.scrollController;
	this->currentPos = source.currentPos;
	this->min = source.min;
	this->max = source.max;
	this->pageSize = source.pageSize;

	return *this;
}

//�Ҹ���
HorizontalScroll::~HorizontalScroll()
{

}