#ifndef _COMPOSITE_H
#define _COMPOSITE_H

#include "Glyph.h"
#include "Array.h"

typedef signed long int Long;

class Composite :public Glyph
{
public:
	Composite(Long capacity = 256);//디폴트생성자
	Composite(const Composite& source);//복사생성자
	Composite& operator=(const Composite& source);//치환연산자
	Long Add(Glyph* glyph);
	Long Remove(Long index);
	Glyph* GetAt(Long index);
	virtual ~Composite() = 0;//추상클래스 소멸자
	//추상클래스(구체적 내용X)이므로 Clone을 선언X, Concrete Prototype,
	//즉 구체적인 내용이 있는 자식들(Row, Note) 헤더파일에서 Clone메소드를 선언해주고 cpp에서 정의해주면 됨.
	//인라인함수
	Long GetCapacity() const;
	Long GetLength() const;
protected://상속받은 subclass들이 superclass의 데이터멤버에 접근하기 위해서 protected로 해줘야함.
	Array<Glyph*> glyphs;
	Long capacity;
	Long length;
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

#endif // !_COMPOSITE_H