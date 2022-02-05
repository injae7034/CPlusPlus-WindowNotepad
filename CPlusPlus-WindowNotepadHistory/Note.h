#ifndef _NOTE_H
#define _NOTE_H

#include "Composite.h"

class Note :public Composite
{
public:
	Note(Long capacity = 256);//����Ʈ������
	Note(const Note& source);//���������
	Note& operator=(const Note& source);//ġȯ������
	virtual ~Note();//�Ҹ���
	virtual Glyph* Clone();//Prototype Pattern Clone
	virtual string GetContent();
	//Composite�� �ִ� �����Լ����� Note���ؿ� �°� ������(�������̵�)
	//�������ϴ� �Լ��� ��ӹ޴� �Լ��� �� �Լ��� �������ϴ��� �� �Լ��տ� virtual�� �ٿ� �����.
	virtual Long Last();
	virtual Long Next();
	virtual Long NextWord();
	virtual void NextWordOnRowAutoChange(Long currentRowIndex, Long currentLetterIndex);
	virtual Long PreviousWord();
	virtual void PreviousWordOnRowAutoChange(Long currentRowIndex, Long currentLetterIndex);
	//Previous�� First�� Composite�� ���ǰ� �ߺ��Ǳ⶧���� Composite�� �״�� ���� ��.
	virtual Long Add(Glyph* glyph);
	virtual Long Add(Long index, Glyph* glyph);
	virtual Long Remove(Long index);
	virtual void CalculateSelectedRange(Long* startingRowPos, Long* startingLetterPos,
		Long* endingRowPos, Long* endingLetterPos);
	virtual void Accept(GlyphVisitor* glyphVisitor);
};

#endif // !_NOTE_H