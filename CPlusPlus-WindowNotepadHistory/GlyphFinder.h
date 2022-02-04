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
	void FindDown(string keyword, Long* findingStartRowIndex, Long* findingStartLetterIndex,
		Long* findingEndRowIndex, Long* findingEndLetterIndex);//�Ʒ��� ã��
	void FindUp(string keyword, Long* findingStartRowIndex, Long* findingStartLetterIndex,
		Long* findingEndRowIndex, Long* findingEndLetterIndex);//���� ã��
	void FindDownWithMatchCase(string keyword, Long* findingStartRowIndex, Long* findingStartLetterIndex,
		Long* findingEndRowIndex, Long* findingEndLetterIndex);//��/�ҹ��� ���о��� �Ʒ��� ã��
	void FindUpWithMatchCase(string keyword, Long* findingStartRowIndex, Long* findingStartLetterIndex,
		Long* findingEndRowIndex, Long* findingEndLetterIndex);//��/�ҹ��� ���о��� ���� ã��
	~GlyphFinder();//�Ҹ���
};

#endif // !_GLYPHFINDER_H
