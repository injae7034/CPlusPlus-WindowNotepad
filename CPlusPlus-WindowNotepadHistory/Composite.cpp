#include "Composite.h"

//����Ʈ������
Composite::Composite(Long capacity)
	:glyphs(capacity)
{
	this->capacity = capacity;
	this->length = 0;
	this->current = 0;
}

//�Ҹ���
Composite::~Composite()
{
	Glyph* glyph;
	Long i = 0;
	while (i < this->length)
	{
		glyph = this->GetAt(i);
		if (glyph != 0)
		{
			delete glyph;
		}
		i++;
	}
}

//���������
Composite::Composite(const Composite& source)
	:glyphs(source.glyphs)
{
	Glyph* glyph;
	Long i = 0;
	while (i < source.length)
	{
		glyph = const_cast<Composite&>(source).glyphs.GetAt(i);
		this->glyphs.Modify(i, glyph->Clone());
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
	this->current = source.current;
}

//ġȯ������
Composite& Composite::operator=(const Composite& source)
{
	Glyph* glyph;
	Long i = 0;
	//1. ������ this(Composite)�Ҵ�����
	while (i < this->length)
	{
		glyph = this->glyphs.GetAt(i);
		if (glyph != 0)
		{
			delete glyph;
		}
		i++;
	}
	//2. Array ġȯ������ ȣ��
	this->glyphs = source.glyphs;
	//3. source�� glyphs �� ���� ������ ���� ���� �Ҵ��ϰ� this->glyphs�� �ּҸ� ������(����)
	i = 0;
	while (i < source.length)
	{
		glyph = const_cast<Composite&>(source).glyphs.GetAt(i);
		this->glyphs.Modify(i, glyph->Clone());
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
	this->current = source.current;

	return *this;
}

//Add
Long Composite::Add(Glyph* glyph)
{
	//1. glyph��ũ�� �Է¹޴´�.
	//2. ��뷮�� �Ҵ緮���� ������
	Long index;
	if (this->length < this->capacity)
	{
		index = this->glyphs.Store(this->length, glyph);
	}
	//3. �׷��� ������
	else
	{
		index = this->glyphs.AppendFromRear(glyph);
		//3.1 �Ҵ緮�� ������Ų��.
		this->capacity++;
	}
	//4. ��뷮�� ������Ų��.
	this->length++;
	//5. ���� �� �Ǵ� ���� ���ڰ� ������ ��ġ�� �����Ѵ�.
	this->current = index + 1;
	//6. ���� ���� ��ġ �Ǵ� ���� ������ ��ġ�� ����Ѵ�.
	return index;
}

//Remove
Long Composite::Remove(Long index)
{
	//1. ���� ��ġ�� �Է¹޴´�.
	//2. ���ڳ� ���� ����� ���� ���� ��ġ�� ���� ���ڸ� �߰��� ��ġ�� �Ǳ� ������ current�� �����Ѵ�.
	this->current = index;
	//2. �ش���ġ�� �迭��Ҹ� �����.
	index = this->glyphs.Delete(index);
	//3. �Ҵ緮�� ���ҽ�Ų��.
	this->capacity--;
	//4. ��뷮�� ���ҽ�Ų��.
	this->length--;
	//5. index�� ����Ѵ�.
	return index;
}

//GetAt
Glyph* Composite::GetAt(Long index)
{
	//1. ���� �迭����� ��ġ�� �Է��Ѵ�.
	//2. �ش� ��ġ�� �迭��Ҹ� ����Ѵ�.
	return this->glyphs.GetAt(index);
}