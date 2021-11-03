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
	void FindDown(string keyword, Long* findingStartRowIndex, Long* findingStartLetterIndex,
		Long* findingEndRowIndex, Long* findingEndLetterIndex);//아래로 찾기
	void FindUp(string keyword, Long* findingStartRowIndex, Long* findingStartLetterIndex,
		Long* findingEndRowIndex, Long* findingEndLetterIndex);//위로 찾기
	void FindDownWithMatchCase(string keyword, Long* findingStartRowIndex, Long* findingStartLetterIndex,
		Long* findingEndRowIndex, Long* findingEndLetterIndex);//대/소문자 구분없이 아래로 찾기
	void FindUpWithMatchCase(string keyword, Long* findingStartRowIndex, Long* findingStartLetterIndex,
		Long* findingEndRowIndex, Long* findingEndLetterIndex);//대/소문자 구분없이 위로 찾기
	~GlyphFinder();//소멸자
};

#endif // !_GLYPHFINDER_H
