#ifndef _GLYPHCREATOR_H
#define _GLYPHCREATOR_H

#include "Glyph.h"

class GlyphCreator
{
public:
	GlyphCreator();//디폴트생성자
	Glyph* Create(char(*letter));//팩토리 메소드 패턴
	~GlyphCreator();//추상클래스 소멸자
};

#endif // !_GLYPHCREATOR_H
