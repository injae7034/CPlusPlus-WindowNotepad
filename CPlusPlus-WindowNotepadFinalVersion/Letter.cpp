#include "Letter.h"

//����Ʈ������
Letter::Letter(bool isSelected)
{
	this->isSelected = isSelected;
}

//���ù��� ���ϱ�
void Letter::Select(bool isSelected)
{
	this->isSelected = isSelected;
}

//�Ҹ���
Letter::~Letter()
{

}