#include "HorizontalScroll.h"
#include "NotepadForm.h"
#include "TextExtent.h"
#include "ScrollController.h"

//��bƮ������
HorizontalScroll::HorizontalScroll(ScrollController* scrollController)
:Scroll(scrollController)
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

//LinePrevious
Long HorizontalScroll::LinePrevious()
{
	//1. ���� ��ġ�� �ѱ۱������� �� ĭ �������� �̵��Ѵ�.
	Long koreanLetterWidth = this->scrollController->notepadForm->textExtent->GetTextWidth("��");
	this->currentPos -= koreanLetterWidth;
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
Long HorizontalScroll::LineNext()
{
	//1. ���� ��ġ�� �ѱ۱������� �� ĭ �������� �̵��Ѵ�.
	Long koreanLetterWidth = this->scrollController->notepadForm->textExtent->GetTextWidth("��");
	this->currentPos += koreanLetterWidth;
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

//�Ҹ���
HorizontalScroll::~HorizontalScroll()
{

}