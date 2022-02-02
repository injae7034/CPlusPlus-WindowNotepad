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
	virtual Glyph* Clone();//Prototype Patter Clone
	virtual string GetContent();
	virtual string GetPartOfContent(Long current);
};

#endif // !_ROW_H

