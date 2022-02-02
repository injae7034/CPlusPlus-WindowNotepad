#ifndef _LETTER_H
#define _LETTER_H

#include "Glyph.h"
typedef signed long int Long;

class Letter :public Glyph
{
public:
	Letter(Long capacity = 256);//디폴트생성자
	//Letter(const Letter& source);//복사생성자 필요없음
	//Letter& operator=(const Letter& source);//치환연산자 필요없음
	virtual ~Letter() = 0;//추상클래스 소멸자
	//추상클래스(구체적 내용X)이므로 Clone을 선언X, Concrete Prototype, 즉 구체적인 내용이 있는 자식들(SingleByterLetter, DoubleByteLetter) 헤더파일에서 Clone메소드를 선언해주고 cpp에서 정의해주면 됨.
	//인라인함수정의
	//GetScript는 반환값이 char이나 char*로 다르기때문에 여기서 인라인함수정의를 하지X
	//논리함수 필요없음
	//bool IsEqual(const Letter& other);
	//bool IsNotEqual(const Letter& other);
	//연산자함수 필요없음
	//bool operator==(const Letter& other);
	//bool operator!=(const Letter& other);
};

#endif // !_LETTER_H
