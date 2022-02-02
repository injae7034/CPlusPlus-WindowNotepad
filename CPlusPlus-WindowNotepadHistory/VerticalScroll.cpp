#include "VerticalScroll.h"
#include "ScrollController.h"
#include "TextExtent.h"
#include "Glyph.h"

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
	//2. ���� ��ġ�� scrollMaxPos���� ũ��
	Long scrollMaxPos = this->max - this->pageSize;
	if (this->currentPos > scrollMaxPos)
	{
		//2.1 ���� ��ġ�� scrollMaxPos�� �����Ѵ�.
		this->currentPos = scrollMaxPos;
	}
	//3. ����� ���� ��ġ�� ����Ѵ�.
	return this->currentPos;
}

//PagePrevious
Long VerticalScroll::PagePrevious()
{
	Long distance = 0;//������������ �̵��� �Ÿ�
	Long i = 0;//���� ����

	//1. ���� �۲��� ���̸� ���Ѵ�.
	Long textHeight = this->scrollController->notepadForm->textExtent->GetHeight();
	//2. �̵��� �Ÿ��� ���� ȭ���� ���� ���̺��� �۰ų� ��������
	//�׸��� i�� �޸����� ���� �������� �۰ų� �������� �ݺ��Ѵ�.
	while (distance <= this->pageSize &&
		i <= this->scrollController->notepadForm->note->GetLength())
	{
		//2.1 ���� ������ ������Ų��.
		i++;
		//2.2 ���� ������ ���� ��� ���̸� ���ؼ� �̵��� �Ÿ��� ���Ѵ�.
		distance = textHeight * i;
	}
	//2. �̵��� �Ÿ��� ����ȭ���� ���α��̺��� ũ�� �޸����� ���� �������� �۰ų� ������
	if (distance > this->pageSize &&
		i <= this->scrollController->notepadForm->note->GetLength())
	{
		//2.1 �̵��� �Ÿ��� ���������ش�.
		i--;
		distance = textHeight * i;
	}
	//3. i�� ���� �������� ũ��
	else if (i > this->scrollController->notepadForm->note->GetLength())
	{
		//3.1 �̵��� �Ÿ��� ���������ش�.
		distance = 0;
	}
	//4. ���� ��ġ�� �̵��� �Ÿ���ŭ ���ش�.
	this->currentPos -= distance;
	//5. ���� ��ġ�� 0���� ������
	if (this->currentPos < 0)
	{
		//5.1 ���� ��ġ�� 0���� �����Ѵ�.
		this->currentPos = 0;
	}
	//6. ���� ��ġ�� ����Ѵ�.
	return this->currentPos;
}

//PageNext
Long VerticalScroll::PageNext()
{
	Long distance = 0;//������������ �̵��� �Ÿ�
	Long i = 0;//���� ����

	//1. ���� �۲��� ���̸� ���Ѵ�.
	Long textHeight = this->scrollController->notepadForm->textExtent->GetHeight();
	//2. �̵��� �Ÿ��� ���� ȭ���� ���� ���̺��� �۰ų� ��������
	//�׸��� i�� �޸����� ���� �������� �۰ų� �������� �ݺ��Ѵ�.
	while (distance <= this->pageSize &&
		i <= this->scrollController->notepadForm->note->GetLength())
	{
		//2.1 ���� ������ ������Ų��.
		i++;
		//2.2 ���� ������ ���� ��� ���̸� ���ؼ� �̵��� �Ÿ��� ���Ѵ�.
		distance = textHeight * i;
	}
	//2. �̵��� �Ÿ��� ����ȭ���� ���α��̺��� ũ�� �޸����� ���� �������� �۰ų� ������
	if (distance > this->pageSize &&
		i <= this->scrollController->notepadForm->note->GetLength())
	{
		//2.1 �̵��� �Ÿ��� ���������ش�.
		i--;
		distance = textHeight * i;
	}
	//3. i�� ���� �������� ũ��
	else if (i > this->scrollController->notepadForm->note->GetLength())
	{
		//3.1 �̵��� �Ÿ��� ���������ش�.
		distance = 0;
	}
	//4. ���� ��ġ�� �̵��� �Ÿ���ŭ �����ش�.
	this->currentPos += distance;
	//5. ���� ��ġ�� scrollMaxPos���� ũ��
	Long scrollMaxPos = this->max - this->pageSize;
	if (this->currentPos > scrollMaxPos)
	{
		//5.1 ���� ��ġ�� scrollMaxPos�� �����Ѵ�.
		this->currentPos = scrollMaxPos;
	}
	//6. ���� ��ġ�� ����Ѵ�.
	return this->currentPos;
}

//�Ҹ���
VerticalScroll::~VerticalScroll()
{

}