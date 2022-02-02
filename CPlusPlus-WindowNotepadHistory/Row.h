#ifndef _ROW_H
#define _ROW_H

#include "Composite.h"

class Row :public Composite
{
public:
	Row(Long capacity = 256);//디폴트생성자
	Row(const Row& source);//복사생성자
	Row& operator=(const Row& source);//치환연산자
	virtual ~Row();//소멸자
	virtual Glyph* Clone();//Prototype Patter Clone
	virtual string GetContent();
	virtual string GetPartOfContent(Long current);
};

#endif // !_ROW_H

