#ifndef _CLIPBOARD_H
#define _CLIPBOARD_H

#include "Composite.h"

class Clipboard :public Composite
{
public:
	Clipboard(Long capacity = 256);//디폴트생성자
	Clipboard(const Clipboard& source);//복사생성자
	Clipboard& operator=(const Clipboard& source);//치환연산자
	virtual ~Clipboard();//소멸자
	virtual Glyph* Clone();//Prototype Pattern Clone
	virtual Long Add(Glyph* glyph);//복사한 내용을 리스트에 저장함.
	virtual Long Remove(Long index);//복사한 내용을 리스트에서 지움.
	virtual string GetContent();
	virtual Long RemoveAll();
};

#endif // !_CLIPBOARD_H
