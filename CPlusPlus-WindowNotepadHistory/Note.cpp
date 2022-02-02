#include "Note.h"

//디폴트생성자
Note::Note(Long capacity)
	:Composite(capacity)
{

}

//복사생성자
Note::Note(const Note& source)
	:Composite(source)
{

}

//치환연산자
Note& Note::operator=(const Note& source)
{
	//this->Composite::operator=(source);
	Composite::operator=(source);

	return *this;
}

//소멸자
Note::~Note()
{

}

//Prototype Patter Clone
Glyph* Note::Clone()
{
	return new Note(*this);
}

//GetContent
string Note::GetContent()
{
	string content;
	Long index = 0;
	while (index < this->length)
	{
		content.append(this->glyphs.GetAt(index)->GetContent());
		content.push_back('\n');
		index++;
	}

	return content;
}