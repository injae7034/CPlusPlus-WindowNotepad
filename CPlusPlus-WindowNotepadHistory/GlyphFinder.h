#ifndef _GLYPHFINDER_H
#define _GLYPHFINDER_H

#include<string>

using namespace std;

typedef signed long int Long;

class Glyph;

class GlyphFinder
{
public:
	Glyph* note;
public:
	GlyphFinder(Glyph* note);//����Ʈ������
	void FindDown(string keyword, Long* findingRowIndex, Long* findingLetterIndex,
		Long* keywordLetterCount);//�Ʒ��� ã��
	~GlyphFinder();//�Ҹ���
};

#endif // !_GLYPHFINDER_H
