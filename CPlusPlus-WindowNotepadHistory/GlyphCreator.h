#ifndef _GLYPHCREATOR_H
#define _GLYPHCREATOR_H

#include "Glyph.h"

class GlyphCreator
{
public:
	GlyphCreator();//����Ʈ������
	Glyph* Create(char(*letter), bool isRow = true);//���丮 �޼ҵ� ����
	~GlyphCreator();//�Ҹ���
};

#endif // !_GLYPHCREATOR_H
