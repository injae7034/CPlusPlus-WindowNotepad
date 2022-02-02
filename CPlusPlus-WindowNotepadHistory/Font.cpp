#include "Font.h"

//기본생성자
Font::Font()
	:style(style)
{
	this->size = 0;
}


//매개변수 2개 생성자
Font::Font(Long size, string style)
	:style(style)
{
	this->size = size;
}

//복사생성자
Font::Font(const Font& source)
	:style(source.style)
{
	this->size = source.size;
}

//치환연산자
Font& Font::operator=(const Font& source)
{
	this->style = source.style;
	this->size = source.size;

	return *this;
}

//소멸자
Font::~Font()
{

}