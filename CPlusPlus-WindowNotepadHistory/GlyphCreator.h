#ifndef _GLYPHCREATOR_H
#define _GLYPHCREATOR_H

#include "Glyph.h"

class GlyphCreator
{
public:
	GlyphCreator();//����Ʈ������
	Glyph* Create(char(*letter));//���丮 �޼ҵ� ����
	~GlyphCreator();//�߻�Ŭ���� �Ҹ���
};

#endif // !_GLYPHCREATOR_H
