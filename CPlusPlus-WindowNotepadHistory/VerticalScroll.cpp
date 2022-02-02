#include "VerticalScroll.h"
#include "ScrollController.h"
#include "TextExtent.h"

//����Ʈ������
VerticalScroll::VerticalScroll(ScrollController* scrollController)
	:Scroll(scrollController)
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

//LinePrevious
Long VerticalScroll::LinePrevious()
{
	//1. ���� ��ġ�� �� �� ���� �̵���Ų��.
	this->currentPos -= this->scrollController->notepadForm->textExtent->GetHeight();
	//2. ���� ��ġ�� �����̸�
	if (this->currentPos < 0)
	{
		//2.1 ���� ��ġ�� 0���� �ٲ��ش�.
		this->currentPos = 0;
	}
	//3. ����� ���� ��ġ�� ����Ѵ�.
	return this->currentPos;
}

//LineNext
Long VerticalScroll::LineNext()
{
	//1. ���� ��ġ�� �� �� �Ʒ��� �̵���Ų��.
	this->currentPos += this->scrollController->notepadForm->textExtent->GetHeight();
	//2. ���� ��ġ�� max���� ũ��
	Long max = this->max - this->pageSize;
	if (this->currentPos > max)
	{
		//2.1 ���� ��ġ�� max�� �����Ѵ�.
		this->currentPos = max;
	}
	//3. ����� ���� ��ġ�� ����Ѵ�.
	return this->currentPos;
}

//�Ҹ���
VerticalScroll::~VerticalScroll()
{

}