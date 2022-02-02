#include "Scroll.h"
#include "ScrollController.h"

//����Ʈ������
Scroll::Scroll(ScrollController* scrollController)
{
	this->scrollController = scrollController;
	this->currentPos = 0;
	this->min = 0;
	this->max = 0;
	this->pageSize = 0;
}

//�Ű������� 5�� ������ ������
Scroll::Scroll(ScrollController* scrollController, Long currentPos, Long min, Long max,
	Long pageSize)
{
	this->scrollController = scrollController;
	this->currentPos = currentPos;
	this->min = min;
	this->max = max;
	this->pageSize = pageSize;
}

//���������
Scroll::Scroll(const Scroll& source)
{
	this->scrollController = source.scrollController;
	this->currentPos = source.currentPos;
	this->min = source.min;
	this->max = source.max;
	this->pageSize = source.pageSize;
}

//ġȯ������
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
	//1. ���� ��ġ�� ó������ �̵���Ų��.
	this->currentPos = this->min;
	//2. ���� ��ġ�� ����Ѵ�.
	return this->currentPos;
}

//Last
Long Scroll::Last()
{
	//1. ���� ��ġ�� ���������� �̵���Ų��.
	this->currentPos = this->max;
	//2. ���� ��ġ�� ����Ѵ�.
	return this->currentPos;
}

//LinePrevious
Long Scroll::LinePrevious()
{
	return -1;
}

//LineNext
Long Scroll::LineNext()
{
	return -1;
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

//�Ҹ���
Scroll::~Scroll()
{

}