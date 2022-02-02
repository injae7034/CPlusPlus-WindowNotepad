#ifndef _ROW_H
#define _ROW_H

#include "Composite.h"

class Row :public Composite
{
public:
	Row(Long capacity = 256);//����Ʈ������
	Row(const Row& source);//���������
	Row& operator=(const Row& source);//ġȯ������
	virtual ~Row();//�Ҹ���
	Glyph* Clone();//Prototype Patter Clone
	string GetContent();
};

#endif // !_ROW_H

