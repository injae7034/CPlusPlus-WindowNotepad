#include "Row.h"

//디폴트생성자
Row::Row(Long capacity)
	:Composite(capacity)//Composite의 기본생성자를 호출하면 거기서 멤버들인(capacity, length,
	//current 등이 초기화된다.!!)
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
	string content;//글자들을 저장할 변수(줄)
	string letter;//글자를 저장할 변수
	Long index = 0;
	//1. 한줄의 글자개수만큼 반복한다.
	while (index < this->length)
	{
		//1.1 letter에 한글자(Row의 배열요소 1개)를 저장한다.
		letter = this->glyphs.GetAt(index)->GetContent();
		//1.2 letter가 탭문자이면
		if (letter == "\t")
		{
			//1.2.1 공란을 8개로 바꾼다.
			letter = "        ";
		}
		//1.3 줄에 글자를 추가한다.
		content += letter;
		index++;
	}
	//2. 줄을 반환한다.
	return content;
	//3. 끝내다.
}

//GetPartOfContent
string Row::GetPartOfContent(Long current)
{
	string content = "";//글자들을 저장할 변수(줄)
	string letter;//글자를 저장할 변수
	Long index = 0;
	//1. 현재 줄의 캐럿의 가로 위치보다 작은동안 반복한다.
	while (index < current)
	{
		//1.1 letter에 한글자(Row의 배열요소 1개)를 저장한다.
		letter = this->glyphs.GetAt(index)->GetContent();
		//1.2 letter가 탭문자이면
		if (letter == "\t")
		{
			//1.2.1 공란을 8개로 바꾼다.
			letter = "        ";
		}
		//1.3 줄에 글자를 추가한다.
		content += letter;
		index++;
	}
	//2. 줄을 반환한다.
	return content;
	//3. 끝내다.
}