#include "Note.h"

//����Ʈ������
Note::Note(Long capacity)
	:Composite(capacity)
{

}

//���������
Note::Note(const Note& source)
	:Composite(source)
{

}

//ġȯ������
Note& Note::operator=(const Note& source)
{
	//this->Composite::operator=(source);
	Composite::operator=(source);

	return *this;
}

//�Ҹ���
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