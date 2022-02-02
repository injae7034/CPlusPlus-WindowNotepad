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
	Glyph* Clone();//Prototype Pattern Clone
	string GetContent();
};

#endif // !_NOTE_H