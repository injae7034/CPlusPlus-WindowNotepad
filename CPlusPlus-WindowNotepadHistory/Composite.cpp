#include "Composite.h"

//����Ʈ������
Composite::Composite(Long capacity)
	:glyphs(capacity)
{
	this->capacity = capacity;
	this->length = 0;
}

//�Ҹ���
//Composite�� �ּҹ迭(Array<Glyph*>)�� ������ �����Ƿ� Composite�� �ٷ� �Ҹ��ϱ� ����
//Composite�� ����� ������ �ִ� �ּҹ迭�� �� �迭��Ұ� ���� �Ҵ�� ����(Row�ϼ��� �ְ�,
//SingleByteLetter�̰ų� DoubleByteLetter�ϼ��� ����)�� ���� �Ҵ������� �� ��Ű�� ����
//�ڱ� �ڽ�(Composite)�� �Ҵ��������Ѿ���! �׷��� �Ҹ��ڿ��� �ݺ�������ŭ �� �迭��Ҹ� �Ҵ�����ó���������!
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
	:glyphs(source.glyphs)//���� source�� ������� �ּҸ� ������.(��������)
	//this�� �� �迭��ҵ�(�ּҵ�)�� source�� �� �迭��ҵ�(�ּҵ�)�� ������ ����(�����Ҵ��)�� ����Ŵ.
	//���� ������ ��� ���� source�� �Ҹ��ڸ� ȣ���ϸ� this�� �Ҹ��ڸ� ȣ������ �ʾ������� �ұ��ϰ�
	//this�� ��۸��������� �迭��ҵ鸸 ������ �ְ� �ǹ���. �ֳ��ϸ� source�� �Ҹ��ڿ��� source��
	//�迭��ҵ��� ����Ű�� �ִ� ���� �Ҵ�� ������� ���� �Ҵ������ع��ȱ⶧���� this�� �� �迭��ҵ� 
	//source�� �� �迭��ҿ� ������ ������ ����Ű�� �־��� ������ this�� �Ҵ������� ���� ����Ű�� �ּҰ�
	//��۸������͵��� �迭��Ҹ� ������ �ְ� �Ǳ� ������ ������ �ǹ̿����� ������ �� �� ����
	//�׸��� ���߿� ���α׷��� ������ �� �̰� ������ ������ �߻��� Ȯ���� ���� �׷��� ���� ���縦 �ؾ���!
{
	Glyph* glyph;
	Long i = 0;
	//source�� ������ŭ �ݺ���
	while (i < source.length)
	{
		//source�� this�� �ƴѵ� source�� ������ ����ϱ� ���ؼ� const_cast<Composite&>�� ����ؾ���!
		//source �� �迭����� �����(�ּҵ�)�� �������� ����.
		glyph = const_cast<Composite&>(source).glyphs.GetAt(i);
		//this�� �� �迭��Ұ� ����� source�� �� �迭���(�ּҵ�)�� ����Ű�� ������� �����ϰ� ����Ŵ
		//�װ� �ٲٴµ� �ٲٴ� �� source�� �� �迭���(�ּҵ�)�� ����Ű�� ������� �����ϰ� �����ؼ�
		//���� ���� �Ҵ��ϰ� �� �ּҸ� this�� �� �迭���(�ּҵ�)�鿡 ������!
		//�̷��� ���� ���縦 �ϰ� ���� ���࿡ source�� �Ҹ��ϴ��� this�� �ƹ� ������ ����
		//this�� �迭��ҵ��� �ڱ�鸸�� ���� �Ҵ�� ������ ������ �־ �ƹ� ������ ����.
		this->glyphs.Modify(i, glyph->Clone());
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
}

//ġȯ������
Composite& Composite::operator=(const Composite& source)
{
	Glyph* glyph;
	Long i = 0;
	//1. ������ this(Composite)�� �迭��ҵ�(�ּҵ�)�� ���� �Ҵ�� ���� �� �ּҵ��ǹ迭 �Ҵ�����
	while (i < this->length)
	{
		glyph = this->glyphs.GetAt(i);
		if (glyph != 0)
		{
			delete glyph;
		}
		i++;
	}
	//2. Array ġȯ������ ȣ��(���� ����)(�ּҵ��� �迭�� �����ؿ�)(���� �Ҵ�� ������ ������� ����Ŵ)
	this->glyphs = source.glyphs;
	//3. source�� glyphs�� ���� ������ ���� ���� �Ҵ��ϰ� this->glyphs�� �ּҸ� ������(����)(��������)
	i = 0;
	while (i < source.length)
	{
		glyph = const_cast<Composite&>(source).glyphs.GetAt(i);
		this->glyphs.Modify(i, glyph->Clone());
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;

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
		//2.1 �迭�� ���������� ���� �Ҵ�� glyph(����)�� �ּҸ� �����Ѵ�.
		index = this->glyphs.Store(this->length, glyph);
	}
	//3. �׷��� ������
	else
	{
		//3.1 �迭�� ���̻� ������ ������ ���⶧���� ������ ��ĭ�� �÷��� glyph�� �����Ѵ�. 
		index = this->glyphs.AppendFromRear(glyph);
		//3.2 �Ҵ緮�� ������Ų��.
		this->capacity++;
	}
	//4. ��뷮�� ������Ų��.
	this->length++;
	//5. ��ġ�� ����Ѵ�.
	return index;
}

//Remove
Long Composite::Remove(Long index)
{
	//1. ���� ��ġ�� �Է¹޴´�.
	//2. �ش�迭��Ұ� �ּҸ� �����ϰ� �ֱ� ������ �ش�迭����� ���� �Ҵ�� ������ ���� ��������.
	Glyph* glyph = this->glyphs[index];
	if (glyph != 0)
	{
		delete glyph;
	}
	//3. �ش���ġ�� �迭����� ���� �Ҵ�� ������ �������� ���� �� �ּҸ� �����.
	index = this->glyphs.Delete(index);
	//4. �Ҵ緮�� ���ҽ�Ų��.
	this->capacity--;
	//5. ��뷮�� ���ҽ�Ų��.
	this->length--;
	//6. index�� ����Ѵ�.
	return index;
}

//GetAt
Glyph* Composite::GetAt(Long index)
{
	//1. ���� �迭����� ��ġ�� �Է��Ѵ�.
	//2. �ش� ��ġ�� �迭��Ҹ� ����Ѵ�.
	return this->glyphs.GetAt(index);
}