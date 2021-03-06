#ifndef _SINGLEBYTELETTER_H
#define _SINGLEBYTELETTER_H

#include "Letter.h"
#include "Glyph.h"

class SingleByteLetter :public Letter
{
public:
	SingleByteLetter();//디폴트생성자
	SingleByteLetter(char script, bool isSelected);//매개변수2개생성자
	SingleByteLetter(const SingleByteLetter& source);//복사생성자
	SingleByteLetter& operator=(const SingleByteLetter& source);//치환연산자
	virtual ~SingleByteLetter();//소멸자
	virtual Glyph* Clone();//Prototype Pattern Clone
	virtual string GetContent();
	virtual void Accept(GlyphVisitor* glyphVisitor);
	//인라인함수
	char GetScript() const;

private:
	char script;
};

//인라인함수 정의
inline char SingleByteLetter::GetScript() const
{
	return this->script;
}

#endif // !_SINGLEBYTELETTER_H