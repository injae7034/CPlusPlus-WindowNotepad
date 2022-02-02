#include "Font.h"

//기본생성자
Font::Font()
	:logFont()
{
	this->color = RGB(0, 0, 0);
}


//매개변수 2개 생성자
Font::Font(LOGFONT logFont, COLORREF color)
	:logFont(logFont)
{
	this->color = color;
}

//복사생성자
Font::Font(const Font& source)
	:logFont(source.logFont)
{
	this->color = source.color;
}

//치환연산자
Font& Font::operator=(const Font& source)
{
	this->logFont = source.logFont;
	this->color = source.color;

	return *this;
}

//소멸자
Font::~Font()
{

}