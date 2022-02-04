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
	GlyphReplacement(Glyph* note);//디폴트생성자
	void Replace(string keyword, Long startSelectedRowIndex, Long startSelectedLetterIndex);
	void ReplaceAll();
	~GlyphReplacement();//소멸자
};

#endif // !_GLYPHREPLACEMENT_H
