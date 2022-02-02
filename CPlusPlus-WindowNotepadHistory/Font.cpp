#include "Font.h"

//�⺻������
Font::Font()
	:logFont()
{
	this->color = RGB(0, 0, 0);
}


//�Ű����� 2�� ������
Font::Font(LOGFONT logFont, COLORREF color)
	:logFont(logFont)
{
	this->color = color;
}

//���������
Font::Font(const Font& source)
	:logFont(source.logFont)
{
	this->color = source.color;
}

//ġȯ������
Font& Font::operator=(const Font& source)
{
	this->logFont = source.logFont;
	this->color = source.color;

	return *this;
}

//�Ҹ���
Font::~Font()
{

}