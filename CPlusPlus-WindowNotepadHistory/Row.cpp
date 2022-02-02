#include "Row.h"

//����Ʈ������
Row::Row(Long capacity)
	:Composite(capacity)
{
	
}

//���������
Row::Row(const Row& source)
	:Composite(source)
{

}

//ġȯ������
Row& Row::operator=(const Row& source)
{
	Composite::operator=(source);

	return *this;
}

//�Ҹ���
Row::~Row()
{

}

//Prototype Patter Clone
Glyph* Row::Clone()
{
	return new Row(*this);
}

//GetContent
string Row::GetContent()
{
	string content;
	Long index = 0;
	while (index < this->length)
	{
		content.append(this->glyphs.GetAt(index)->GetContent());
		index++;
	}

	return content;
}