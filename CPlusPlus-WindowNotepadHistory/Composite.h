#ifndef _COMPOSITE_H
#define _COMPOSITE_H

#include "Glyph.h"
#include "Array.h"

typedef signed long int Long;

//Composite을 Row기준으로 작성하고 Note에서 Note기준으로 오버라이딩(재정의)해서 이용함. 
class Composite :public Glyph
{
public:
	Composite(Long capacity = 256);//디폴트생성자
	Composite(const Composite& source);//복사생성자
	Composite& operator=(const Composite& source);//치환연산자
	//가상함수:순수가상함수는 내용이 없기때문에 반드시 상속받은 클래스들이 순수가상함수를 재정의해줘야만 함!
	//하지만 가상함수는 내용이 있기 때문에 상속받은 클래스들에 의해 재정의되어도 되고 안되어도됨.
	//상속받은 함수가 가상함수를 재정의하면 재정의한 함수를 사용하게 되고,
	//상속받은 함수가 가상함수를 재정의하지않으면 추상클래스(부모클래스)의 가상함수를 그대로 사용하게 됨.
	virtual Long Add(Glyph* glyph);//가상함수
	virtual Long Add(Long index, Glyph* glyph);//(Insert)가상함수
	virtual Long Remove(Long index);//가상함수
	virtual Glyph* Split(Long index);
	//상속받는 클래스가 부모클래스의 함수를 재정의하기 위해서는 함수 앞에 virtual을 붙여서 가상함수를 만들어야함!
	Glyph* GetAt(Long index); 
	virtual ~Composite() = 0;//추상클래스 소멸자 순수가상함수 상속받은 클래스에 의해 반드시 재정의되야함!
	//추상클래스(구체적 내용X)이므로 Clone을 선언X, Concrete Prototype, 
	//구체적인 내용이 있는 자식들(Row, Note) 헤더파일에서 Clone메소드를 선언해주고 cpp에서 정의해주면 됨.
	//캐럿의 이동과 관련된 가상함수 여기서 Row기준으로 작성하고 Note는 자기기준에 맞게 재정의해서 사용함!
	virtual Long First();
	virtual Long Last();
	virtual Long Previous();
	virtual Long Next();
	virtual Long NextWord();
	virtual Long PreviousWord();
	//인라인함수
	Long GetCapacity() const;
	Long GetLength() const;
	Long GetCurrent() const;
protected://상속받은 subclass들이 superclass의 데이터멤버에 접근하기 위해서 protected로 해줘야함.
	Array<Glyph*> glyphs;
	Long capacity;
	Long length;
	Long current;//다음 글자가 쓰여질 칸의 위치: index + 1(캐럿의 가로 위치)(Row기준)
	//Note 기준으로 current는 index이고 캐럿의 세로 위치인데 Note에서 재정의해주면 됨.
};

//인라인함수정의
inline Long Composite::GetCapacity() const
{
	return this->capacity;
}
inline Long Composite::GetLength() const
{
	return this->length;
}
inline Long Composite::GetCurrent() const
{
	return this->current;
}

#endif // !_COMPOSITE_H

