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

//First ���������ũ�� ������
Long Scroll::First()
{
	//1. ���� ��ġ�� ó������ �̵���Ų��.
	this->currentPos = this->min;
	//2. ���� ��ġ�� ����Ѵ�.
	return this->currentPos;
}

//Last ���������ũ�� ������
Long Scroll::Last()
{
	//1. ���� ��ġ�� ���������� �̵���Ų��.
	this->currentPos = this->max;
	//2. ���� ��ġ�� ����Ѵ�.
	return this->currentPos;
}

//LinePrevious ���������ũ�� ���� �ٸ��Ƿ� HorizontalScroll�� VerticalScroll���� ���� ��������
Long Scroll::LinePrevious()
{
	return -1;
}

//LineNext ���������ũ�� ���� �ٸ��Ƿ� HorizontalScroll�� VerticalScroll���� ���� ��������
Long Scroll::LineNext()
{
	return -1;
}

//pagePrevious
Long Scroll::PagePrevious()
{
	//1. ���� ��ġ�� ȭ��ũ�⸸ŭ ���ش�.
	this->currentPos -= this->pageSize;
	//2. ���� ��ġ�� �����̸�
	if (this->currentPos < 0)
	{
		//2.1 ���� ��ġ�� 0���� �ٲ��ش�.
		this->currentPos = 0;
	}
	//3. ���� ��ġ�� ����Ѵ�.
	return this->currentPos;
}

//pageNext
Long Scroll::PageNext()
{
	//1. ���� ��ġ�� ȭ��ũ�⸸ŭ �����ش�.
	this->currentPos += this->pageSize;
	//2. ���� ��ġ�� max���� ũ��
	Long max = this->max - this->pageSize;
	if (this->currentPos > max)
	{
		//2.1 ���� ��ġ�� max�� �����Ѵ�.
		this->currentPos = max;
	}
	//3. ���� ��ġ�� ����Ѵ�.
	return this->currentPos;
}

//MoveP
Long Scroll::Move(UINT nPos)
{
	//1. ���� ��ġ�� ��ũ�ѿ��� ���콺�� Ŭ���� ��ġ�� �̵���Ų��. 
	this->currentPos = nPos;
	//2. ���� ��ġ�� ����Ѵ�.
	return this->currentPos;
}

//�Ҹ���
Scroll::~Scroll()
{

}