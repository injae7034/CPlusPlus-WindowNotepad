#ifndef _COMPOSITE_H
#define _COMPOSITE_H

#include "Glyph.h"
#include "Array.h"

typedef signed long int Long;

class Composite :public Glyph
{
public:
	Composite(Long capacity = 256);//����Ʈ������
	Composite(const Composite& source);//���������
	Composite& operator=(const Composite& source);//ġȯ������
	Long Add(Glyph* glyph);
	Long Remove(Long index);
	Glyph* GetAt(Long index);
	virtual ~Composite() = 0;//�߻�Ŭ���� �Ҹ���
	//�߻�Ŭ����(��ü�� ����X)�̹Ƿ� Clone�� ����X, Concrete Prototype,
	//�� ��ü���� ������ �ִ� �ڽĵ�(Row, Note) ������Ͽ��� Clone�޼ҵ带 �������ְ� cpp���� �������ָ� ��.
	//�ζ����Լ�
	Long GetCapacity() const;
	Long GetLength() const;
protected://��ӹ��� subclass���� superclass�� �����͸���� �����ϱ� ���ؼ� protected�� �������.
	Array<Glyph*> glyphs;
	Long capacity;
	Long length;
};

//�ζ����Լ�����
inline Long Composite::GetCapacity() const
{
	return this->capacity;
}
inline Long Composite::GetLength() const
{
	return this->length;
}

#endif // !_COMPOSITE_H