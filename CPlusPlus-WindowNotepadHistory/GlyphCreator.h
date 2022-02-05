#ifndef _GLYPHCREATOR_H
#define _GLYPHCREATOR_H

#include "Glyph.h"

class GlyphCreator
{
public:
	GlyphCreator();//디폴트생성자
	Glyph* Create(char(*letter), bool isRow = true);//팩토리 메소드 패턴
	~GlyphCreator();//소멸자
};

#endif // !_GLYPHCREATOR_H
