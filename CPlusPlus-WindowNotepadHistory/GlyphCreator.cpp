#include "GlyphCreator.h"
#include "Note.h"
#include "Row.h"
#include "SingleByteLetter.h"
#include "DoubleByteLetter.h"
#include<cstring>

using namespace std;

//����Ʈ������
GlyphCreator::GlyphCreator()
{

}

//�Ҹ���
GlyphCreator::~GlyphCreator()
{

}

//���丮�޼ҵ�����
Glyph* GlyphCreator::Create(char(*letter))
{
	Glyph* glyph = 0;
	
	if (letter[0] == '\0')
	{
		glyph = new Note();
	}
	else if (letter[0] == '\n' || letter[0] == '\r')
	{
		glyph = new Row();
	}
	else if (strlen(letter) < 2)
	{
		glyph = new SingleByteLetter(letter[0]);
	}
	else
	{
		glyph = new DoubleByteLetter(letter);
	}

	return glyph;
}