#ifndef _LETTER_H
#define _LETTER_H

#include "Glyph.h"
typedef signed long int Long;

class Letter :public Glyph
{
public:
	Letter(bool isisSelected = false);//디폴트생성자
	virtual void Select(bool isSelected);
	virtual ~Letter() = 0;//추상클래스 소멸자
	//추상클래스(구체적 내용X)이므로 Clone을 선언X, Concrete Prototype, 즉 구체적인 내용이 있는 자식들(SingleByterLetter, DoubleByteLetter) 헤더파일에서 Clone메소드를 선언해주고 cpp에서 정의해주면 됨.
	//인라인함수정의
	virtual bool IsSelected() const;
protected:
	bool isSelected;
};

//인라인함수 정의
inline bool Letter::IsSelected() const
{
	return this->isSelected;
}

#endif // !_LETTER_H
