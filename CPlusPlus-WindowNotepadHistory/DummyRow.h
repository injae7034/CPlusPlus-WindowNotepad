#ifndef _DUMMYROW_H
#define _DUMMYROW_H

#include "Row.h"

class DummyRow :public Row
{
public:
	DummyRow(Long capacity = 256);//����Ʈ������
	DummyRow(const DummyRow& source);//���������
	DummyRow& operator=(const DummyRow& source);//ġȯ������
	virtual ~DummyRow();//�Ҹ���
	virtual Glyph* Clone();//Prototype Patter Clone
	virtual void Accept(GlyphVisitor* glyphVisitor);
};

#endif // !_DUMMYROW_H
