#include "Letter.h"

//디폴트생성자
Letter::Letter(bool isSelected)
{
	this->isSelected = isSelected;
}

//선택범위 정하기
void Letter::Select(bool isSelected)
{
	this->isSelected = isSelected;
}

//소멸자
Letter::~Letter()
{

}