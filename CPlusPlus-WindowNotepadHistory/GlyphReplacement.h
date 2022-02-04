#ifndef _GLYPHREPLACEMENT_H
#define _GLYPHREPLACEMENT_H

#include<string>

using namespace std;

typedef signed long int Long;

class Glyph;

class GlyphReplacement
{
public:
	Glyph* note;
public:
	GlyphReplacement(Glyph* note);//����Ʈ������
	void Replace(string keyword, Long startSelectedRowIndex, Long startSelectedLetterIndex);
	void ReplaceAll();
	~GlyphReplacement();//�Ҹ���
};

#endif // !_GLYPHREPLACEMENT_H
