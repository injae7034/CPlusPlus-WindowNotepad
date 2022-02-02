#include "DoubleByteLetter.h"

//디폴트생성자
DoubleByteLetter::DoubleByteLetter()
{
	//문자 배열 초기화는 문자 배열 각요소에 널문자(\0)를 대입함으로써 한다. 문자 배열은 문자열이랑 다르다.
	this->script[0] = '\0';
	this->script[1] = '\0';
}

//매개변수 1개 생성자
DoubleByteLetter::DoubleByteLetter(char(*script))// char(*script)는 배열포인터이고 char* script는 그냥 주소를 저장한거임
//개념상 배열포인터(배열의 시작주소)를 입력하는것이 맞다
{
	//문자배열은 매개변수로 이용할 때 시작주소(char*)를 입력한다.
	this->script[0] = script[0];
	this->script[1] = script[1];
}

//복사생성자
DoubleByteLetter::DoubleByteLetter(const DoubleByteLetter& source)
{
	//문자배열은 문자열처럼 한번에 대입이 불가능함. 각 배열요소에 따로 대입을 해줘야함!
	this->script[0] = source.script[0];
	this->script[1] = source.script[1];
}

//치환연산자
DoubleByteLetter& DoubleByteLetter::operator=(const DoubleByteLetter& source)
{
	this->script[0] = source.script[0];
	this->script[1] = source.script[1];

	return *this;
}

//소멸자
DoubleByteLetter::~DoubleByteLetter()
{

}

//Prototype Pattern Clone
Glyph* DoubleByteLetter::Clone()
{
	return new DoubleByteLetter(*this);
}

//GetContent
string DoubleByteLetter::GetContent()
{
	string script;
	script += this->script[0];
	script += this->script[1];
	//script += '\0';

	return script;
}