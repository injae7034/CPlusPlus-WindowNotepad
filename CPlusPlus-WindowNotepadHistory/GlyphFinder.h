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
	GlyphFinder(Glyph* note);//디폴트생성자
	void FindDown(string keyword, Long* findingRowIndex, Long* findingLetterIndex,
		Long* keywordLetterCount);//아래로 찾기
	~GlyphFinder();//소멸자
};

#endif // !_GLYPHFINDER_H
