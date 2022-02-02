#include "Font.h"

//�⺻������
Font::Font()
	:style(style)
{
	this->size = 0;
}


//�Ű����� 2�� ������
Font::Font(Long size, string style)
	:style(style)
{
	this->size = size;
}

//���������
Font::Font(const Font& source)
	:style(source.style)
{
	this->size = source.size;
}

//ġȯ������
Font& Font::operator=(const Font& source)
{
	this->style = source.style;
	this->size = source.size;

	return *this;
}

//�Ҹ���
Font::~Font()
{

}