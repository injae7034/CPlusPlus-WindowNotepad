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
	Glyph* Clone();//Prototype Pattern Clone
	string GetContent();
};

#endif // !_NOTE_H