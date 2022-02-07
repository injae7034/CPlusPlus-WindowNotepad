#include "Font.h"

//기본생성자
Font::Font()
	:logFont()
{
	this->color = RGB(0, 0, 0);
	this->size = 0;
}


//매개변수 2개 생성자
Font::Font(LOGFONT logFont, Long size, COLORREF color)
	:logFont(logFont)
{
	this->size = size;
	this->color = color;
}

//복사생성자
Font::Font(const Font& source)
	:logFont(source.logFont)
{
	this->size = source.size;
	this->color = source.color;
}

//치환연산자
Font& Font::operator=(const Font& source)
{
	this->logFont = source.logFont;
	this->size = source.size;
	this->color = source.color;

	return *this;
}

//소멸자
Font::~Font()
{

}