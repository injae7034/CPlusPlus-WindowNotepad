#include "Letter.h"

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

//�Ҹ���
Letter::~Letter()
{

}