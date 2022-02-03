#include "Letter.h"
#include "TextOutVisitor.h"

//����Ʈ������
Letter::Letter()
{
	this->isSelceted = false;
}

//���ù��� ���ϱ�
void Letter::Select(bool isSelected)
{
	this->isSelceted = isSelected;
}

void Letter::Accept(TextOutVisitor* textOutVisitor)
{
	textOutVisitor->VisitLetterTextOut(this);
}

//�Ҹ���
Letter::~Letter()
{

}