#ifndef _NOTE_H
#define _NOTE_H

#include "Composite.h"

class Note :public Composite
{
public:
	Note(Long capacity = 256);//디폴트생성자
	Note(const Note& source);//복사생성자
	Note& operator=(const Note& source);//치환연산자
	virtual ~Note();//소멸자
	virtual Glyph* Clone();//Prototype Pattern Clone
	virtual string GetContent();
	//Composite에 있는 가상함수들을 Note기준에 맞게 재정의(오버라이딩)
	//재정의하는 함수는 상속받는 함수가 그 함수를 재정의하더라도 그 함수앞에 virtual을 붙여 줘야함.
	virtual Long Last();
	virtual Long Next();
	virtual Long NextWord();
	virtual void NextWordOnRowAutoChange(Long currentRowIndex, Long currentLetterIndex);
	virtual Long PreviousWord();
	virtual void PreviousWordOnRowAutoChange(Long currentRowIndex, Long currentLetterIndex);
	//Previous랑 First는 Composite와 정의가 중복되기때문에 Composite꺼 그대로 쓰면 됨.
	virtual Long Add(Glyph* glyph);
	virtual Long Add(Long index, Glyph* glyph);
	virtual Long Remove(Long index);
	virtual void CalculateSelectedRange(Long* startingRowPos, Long* startingLetterPos,
		Long* endingRowPos, Long* endingLetterPos);
	virtual void Accept(GlyphVisitor* glyphVisitor);
};

#endif // !_NOTE_H