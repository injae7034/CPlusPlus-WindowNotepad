#ifndef _DUMMYROW_H
#define _DUMMYROW_H

#include "Row.h"

class DummyRow :public Row
{
public:
	DummyRow(Long capacity = 256);//디폴트생성자
	DummyRow(const DummyRow& source);//복사생성자
	DummyRow& operator=(const DummyRow& source);//치환연산자
	virtual ~DummyRow();//소멸자
	virtual Glyph* Clone();//Prototype Patter Clone
	virtual void Accept(GlyphVisitor* glyphVisitor);
};

#endif // !_DUMMYROW_H
