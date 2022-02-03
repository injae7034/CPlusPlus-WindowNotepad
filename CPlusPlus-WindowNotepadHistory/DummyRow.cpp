#include "DummyRow.h"

//디폴트생성자
DummyRow::DummyRow(Long capacity)
	:Row(capacity)//Row의 기본생성자를 호출하면 거기서 Compostie의 생성자가 호출되서
	//멤버들인(capacity, length, current 등이 초기화된다!)
{

}

//복사생성자
DummyRow::DummyRow(const DummyRow& source)
	:Row(source)
{

}

//치환연산자
DummyRow& DummyRow::operator=(const DummyRow& source)
{
	Row::operator=(source);

	return *this;
}

//소멸자
DummyRow::~DummyRow()
{

}

//Prototype Patter Clone
Glyph* DummyRow::Clone()
{
	return new DummyRow(*this);
}