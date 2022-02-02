#include "SingleByteLetter.h"
#include "GlyphVisitor.h"

//디폴트생성자
SingleByteLetter::SingleByteLetter()
	:Letter()
{
	//this->isSelceted = false;
	this->script = '\0';//문자초기화는 널문자(\0)로 한다. (빈칸)""은 문자열이라서 string에서 초기화 할 때 사용함.
}

//매개변수 2개 생성자
SingleByteLetter::SingleByteLetter(char script, bool isSelected)
{
	this->script = script;
	this->isSelected = isSelected;
}

//복사생성자
SingleByteLetter::SingleByteLetter(const SingleByteLetter& source)
{
	this->script = source.script;
	this->isSelected = source.isSelected;
}

//치환연산자
SingleByteLetter& SingleByteLetter::operator=(const SingleByteLetter& source)
{
	this->script = source.script;
	this->isSelected = source.isSelected;
	
	return *this;
}

//소멸자
SingleByteLetter::~SingleByteLetter()
{

}

//Prototype Pattern Clone
Glyph* SingleByteLetter::Clone()
{
	return new SingleByteLetter(*this);
}

//GetContent
string SingleByteLetter::GetContent()
{
	string script;
	script += this->script;
	//script += '\0';

	return script;
}

//Visitor Pattern
void SingleByteLetter::Accept(GlyphVisitor* glyphVisitor)
{
	glyphVisitor->VisitSingleByteLetter(this);
}