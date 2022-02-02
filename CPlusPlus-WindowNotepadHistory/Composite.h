#ifndef _COMPOSITE_H
#define _COMPOSITE_H

#include "Glyph.h"
#include "Array.h"

typedef signed long int Long;

//Composite�� Row�������� �ۼ��ϰ� Note���� Note�������� �������̵�(������)�ؼ� �̿���. 
class Composite :public Glyph
{
public:
	Composite(Long capacity = 256);//����Ʈ������
	Composite(const Composite& source);//���������
	Composite& operator=(const Composite& source);//ġȯ������
	//�����Լ�:���������Լ��� ������ ���⶧���� �ݵ�� ��ӹ��� Ŭ�������� ���������Լ��� ����������߸� ��!
	//������ �����Լ��� ������ �ֱ� ������ ��ӹ��� Ŭ�����鿡 ���� �����ǵǾ �ǰ� �ȵǾ��.
	//��ӹ��� �Լ��� �����Լ��� �������ϸ� �������� �Լ��� ����ϰ� �ǰ�,
	//��ӹ��� �Լ��� �����Լ��� ���������������� �߻�Ŭ����(�θ�Ŭ����)�� �����Լ��� �״�� ����ϰ� ��.
	virtual Long Add(Glyph* glyph);//�����Լ�
	virtual Long Add(Long index, Glyph* glyph);//(Insert)�����Լ�
	virtual Long Remove(Long index);//�����Լ�
	virtual Glyph* Split(Long index);
	//��ӹ޴� Ŭ������ �θ�Ŭ������ �Լ��� �������ϱ� ���ؼ��� �Լ� �տ� virtual�� �ٿ��� �����Լ��� ��������!
	Glyph* GetAt(Long index); 
	virtual ~Composite() = 0;//�߻�Ŭ���� �Ҹ��� ���������Լ� ��ӹ��� Ŭ������ ���� �ݵ�� �����ǵǾ���!
	//�߻�Ŭ����(��ü�� ����X)�̹Ƿ� Clone�� ����X, Concrete Prototype, 
	//��ü���� ������ �ִ� �ڽĵ�(Row, Note) ������Ͽ��� Clone�޼ҵ带 �������ְ� cpp���� �������ָ� ��.
	//ĳ���� �̵��� ���õ� �����Լ� ���⼭ Row�������� �ۼ��ϰ� Note�� �ڱ���ؿ� �°� �������ؼ� �����!
	virtual Long First();
	virtual Long Last();
	virtual Long Previous();
	virtual Long Next();
	virtual Long NextWord();
	virtual Long PreviousWord();
	//�ζ����Լ�
	Long GetCapacity() const;
	Long GetLength() const;
	Long GetCurrent() const;
protected://��ӹ��� subclass���� superclass�� �����͸���� �����ϱ� ���ؼ� protected�� �������.
	Array<Glyph*> glyphs;
	Long capacity;
	Long length;
	Long current;//���� ���ڰ� ������ ĭ�� ��ġ: index + 1(ĳ���� ���� ��ġ)(Row����)
	//Note �������� current�� index�̰� ĳ���� ���� ��ġ�ε� Note���� ���������ָ� ��.
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
inline Long Composite::GetCurrent() const
{
	return this->current;
}

#endif // !_COMPOSITE_H

