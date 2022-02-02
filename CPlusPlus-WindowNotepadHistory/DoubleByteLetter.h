#ifndef _DOUBLEBYTELETTER_H
#define _DOUBLEBYTELETTER_H

#include "Letter.h"
#include "Glyph.h"

class DoubleByteLetter :public Letter
{
public:
	DoubleByteLetter();//디폴트생성자
	DoubleByteLetter(char(*script));//매개변수1개생성자
	DoubleByteLetter(const DoubleByteLetter& source);//복사생성자
	DoubleByteLetter& operator=(const DoubleByteLetter& source);//치환연산자
	virtual ~DoubleByteLetter();//소멸자
	virtual Glyph* Clone();//Prototype Pattern Clone
	virtual string GetContent();
	//인라인함수
	char* GetScript() const;

private:
	char script[2];
};

//인라인함수 정의
//char*(*(포인터)주소로 배열에 접근함)문자배열의 주소를 반환하면 됨 배열 자체를 반환하는게 말이 안되고 비효율적임. 배열 주소를 알면 접근할 수 있기 때문에
inline char* DoubleByteLetter::GetScript() const
{
	return const_cast<char*>(this->script);
}

#endif // !_SINGLEBYTELETTER_H
