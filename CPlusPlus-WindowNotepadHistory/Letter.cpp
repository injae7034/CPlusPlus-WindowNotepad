#include "Letter.h"

//디폴트생성자
Letter::Letter()
{
	this->isSelceted = false;
}

//선택범위 정하기
void Letter::Select(bool isSelected)
{
	this->isSelceted = isSelected;
}

//소멸자
Letter::~Letter()
{

}