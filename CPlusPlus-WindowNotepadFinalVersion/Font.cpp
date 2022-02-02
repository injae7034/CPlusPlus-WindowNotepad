#include "Font.h"

//�⺻������
Font::Font()
	:logFont()
{
	this->color = RGB(0, 0, 0);
	this->size = 0;
}


//�Ű����� 2�� ������
Font::Font(LOGFONT logFont, Long size, COLORREF color)
	:logFont(logFont)
{
	this->size = size;
	this->color = color;
}

//���������
Font::Font(const Font& source)
	:logFont(source.logFont)
{
	this->size = source.size;
	this->color = source.color;
}

//ġȯ������
Font& Font::operator=(const Font& source)
{
	this->logFont = source.logFont;
	this->size = source.size;
	this->color = source.color;

	return *this;
}

//�Ҹ���
Font::~Font()
{

}