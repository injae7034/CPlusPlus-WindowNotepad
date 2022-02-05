#include "GlyphCreator.h"
#include "Note.h"
#include "Row.h"
#include "SingleByteLetter.h"
#include "DoubleByteLetter.h"
#include "Clipboard.h"
#include<cstring>
#include "DummyRow.h"

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
Glyph* GlyphCreator::Create(char(*letter), bool isRow)
{
	Glyph* glyph = 0;
	if (strcmp(letter, "clipboard") == 0)
	{
		glyph = new Clipboard();
	}
	else if (letter[0] == '\0')
	{
		glyph = new Note();
	}
	else if (letter[0] == '\n' || letter[0] == '\r')
	{
		if (isRow == true)
		{
			glyph = new Row();
		}
		else
		{
			glyph = new DummyRow();
		}
	}
	else if ((letter[0] & 0x80))//�ѱ��̸�
	{
		
		glyph = new DoubleByteLetter(letter, false);
	}
	else
	{
		glyph = new SingleByteLetter(letter[0], false);
	}

	return glyph;
}