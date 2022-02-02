#include "Row.h"

//디폴트생성자
Row::Row(Long capacity)
	:Composite(capacity)
{
	
}

//복사생성자
Row::Row(const Row& source)
	:Composite(source)
{

}

//치환연산자
Row& Row::operator=(const Row& source)
{
	Composite::operator=(source);

	return *this;
}

//소멸자
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