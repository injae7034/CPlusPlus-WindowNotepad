#include "VerticalScroll.h"

//����Ʈ������
VerticalScroll::VerticalScroll()
	:Scroll()
{
	
}

//�Ű������� 5�� ������ ������
VerticalScroll::VerticalScroll(ScrollController* scrollController, Long currentPos, Long min,
	Long max, Long pageSize)
	:Scroll(scrollController, currentPos, min, max, pageSize)
{

}

//���������
VerticalScroll::VerticalScroll(const VerticalScroll& source)
	:Scroll(source)
{

}

//ġȯ������
VerticalScroll& VerticalScroll::operator=(const VerticalScroll& source)
{
	this->scrollController = source.scrollController;
	this->currentPos = source.currentPos;
	this->min = source.min;
	this->max = source.max;
	this->pageSize = source.pageSize;

	return *this;
}

//�Ҹ���
VerticalScroll::~VerticalScroll()
{

}