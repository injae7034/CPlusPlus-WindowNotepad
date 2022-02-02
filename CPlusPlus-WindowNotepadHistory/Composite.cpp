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

//Add(Row����)
Long Composite::Add(Glyph* glyph)
{
	//1. �߰��� ������ �ּҸ� �Է¹޴´�.
	//2. ��뷮�� �Ҵ緮���� ������
	Long index;
	if (this->length < this->capacity)
	{
		//2.1 ����ִ� ������ ���������� �����Ѵ�.
		index = this->glyphs.Store(this->length, glyph);
	}
	//3. �׷��� ������
	else
	{
		//3.1 ������ �� ĭ �÷��� �������� �����Ѵ�.
		index = this->glyphs.AppendFromRear(glyph);
		//3.2 �Ҵ緮�� ������Ų��.
		this->capacity++;
	}
	//4. ��뷮�� ������Ų��.
	this->length++;
	//5. ���� ���ڰ� ������ ��ġ(ĳ���� ���� ���� ��ġ)�� �����Ѵ�.(Row����)
	this->current = index + 1;
	//6. ���� ������ ��ġ�� ����Ѵ�.(Row����)
	return index;
}

//Remove
Long Composite::Remove(Long index)
{
	//1. ���� ������ ��ġ�� �Է¹޴´�.
	//2. ���ڸ� ����� ���� ���� ��ġ�� ���� ���ڸ� �߰��� ��ġ(ĳ���� ���� ���� ��ġ)�� �Ǳ� ������ current�� �����Ѵ�.
	this->current = index;
	//3. �ش�迭��Ұ� �ּҸ� �����ϰ� �ֱ� ������ �ش�迭����� ���� �Ҵ�� ����(����)�� ���� ��������.
	Glyph* glyph = this->glyphs[index];
	if (glyph != 0)
	{
		delete glyph;
	}
	//4. �ش���ġ�� �迭����� ���� �Ҵ�� ����(����)�� �������� ���� �� �ּҸ� �����.
	index = this->glyphs.Delete(index);
	//5. �Ҵ緮�� ���ҽ�Ų��.
	this->capacity--;
	//6. ��뷮�� ���ҽ�Ų��.
	this->length--;
	//7. index�� ����Ѵ�.
	return index;
}

//GetAt
Glyph* Composite::GetAt(Long index)
{
	//1. ���� �迭����� ��ġ�� �Է��Ѵ�.
	//2. �ش� ��ġ�� �迭��Ҹ� ����Ѵ�.
	return this->glyphs.GetAt(index);
}

//ĳ���� ���� ��ġ ���η� �̵�
//ĳ���� ���� ����(����) ��ġ �� ������ �̵���Ű��
Long Composite::First()
{
	//1. ĳ���� ���� ����(����) ��ġ�� ���� �� ������ �̵���Ų��.
	this->current = 0;
	//2. ĳ���� ���� ����(����) ��ġ�� ����Ѵ�.
	return this->current;
}

//ĳ���� ���� ���� ��ġ �� ���������� �̵���Ű��
Long Composite::Last()
{
	//1. ĳ���� ���� ���� ��ġ�� ���� �� ���������� �̵���Ų��.
	this->current = this->length;
	//2. ĳ���� ���� ���� ��ġ�� ����Ѵ�.
	return this->current;
}

//ĳ���� ���� ����(����) ��ġ �� ĭ(��) �������� �̵���Ű��
Long Composite::Previous()
{
	//������ ��� ĳ���� �̵��� ������ �ȸ��������� �̵��� ���ϴ°�찡 ����� ����.
#if 0
	//1. ĳ���� ���� ���� ��ġ�� 0���� ũ��
	if (this->current > 0)
	{
		//1.1 ĳ���� ���� ���� ��ġ�� �� ĭ �������� �̵���Ų��.
		this->current--;
	}
#endif
	//�̵���ų ���� �׻� ���� �̵��� ��Ű��, �� ������ underflow�̸� �ּҰ����� ����������.
	//1. ĳ���� ���� ����(����) ��ġ�� �� ĭ(��) �������� �̵���Ų��.
	this->current--;
	//2. ĳ���� ���� ����(����) ��ġ�� underflow�̸�
	if (this->current == -1)
	{
		//2.1 ĳ���� ���� ����(����) ��ġ�� �ּҰ����� �����Ѵ�.
		this->current = 0;
	}
	//3. ĳ���� ���� ����(����) ��ġ�� ����Ѵ�.
	return this->current;
}

//ĳ���� ���� ���� ��ġ �� ĭ �������� �̵���Ű��
Long Composite::Next()
{
	//������ ��� ĳ���� �̵��� ������ �ȸ��������� �̵��� ���ϴ°�찡 ����� ����.
#if 0
	//1. ĳ���� ���� ���� ��ġ�� ���� �ܾ� �������� ������
	if (this->current < this->length)
	{
		//1.1 ĳ���� ���� ���� ��ġ�� �� ĭ �������� �̵���Ų��.
		this->current++;
	}
#endif
	//�̵���ų ���� �׻� ���� �̵��� ��Ű��, �� ������ overflow�̸� �ִ밪���� ����������.
	//1. ĳ���� ���� ���� ��ġ�� �� ĭ �������� �̵���Ų��.
	this->current++;
	//2. ĳ���� ���� ���� ��ġ�� overflow�̸�
	if (this->current >= this->length)
	{
		//2.1 ĳ���� ���� ���� ��ġ�� �ִ밪���� �����Ѵ�.
		this->current = this->length;
	}
	//3. ĳ���� ���� ���� ��ġ�� ����Ѵ�.
	return this->current;
}

//�ٿ��� ĳ���� �ܾ������ ���������� �̵���Ű��(Row����)
Long Composite::NextWord()
{
	//1. ĳ���� ���� ���� ��ġ�� ���� ���� ���������� ��ġ���� �۰�,(�� �� ������ �̵��̰�)
	//���ڰ� �����̽��� �ǹ��ڰ� �ƴѵ��� �ݺ��Ѵ�.(���ڰ� �ѱ�, ����, ����, Ư�������� ��� �ݺ��ؼ� �̵�)
	//�� �� ������ �̵��ϴ°Ÿ� ó������. �ٿ��� �ٴ��� �̵���  Note->NextWord()���� ó������!
	//(ĳ���� �������� ������ ���� ���(ĳ���� ������ ���� �ڿ� ��ġ�� ���)�� �ٿ��� �ٷ� �̵��ϰ� �Ǵµ� 
	//�̴� Note->NextWord()���� ó������!, ���⼭�� ���� �������� �ܾ���� �̵��ϴ°Ÿ� ó������!)
	string letter;//�ѱ��ڸ� ���� ����
	//������ ���� ��ġ�� �Ѿ�� �������� ������ ���� ���ڰ� ���� ������ ���̳�!
	Long lastPositionOfLetter = this->length - 1;//������ ���� ��ġ
	//�̵��ϱ����� �̵��ߴ��� �̵����� �ʾҴ��� üũ���θ� ���� ���� ��ġ�� ������.(���� �ʿ����)
	//Long current = this->current;
	letter = this->GetAt(this->current)->GetContent();
	while (this->current < lastPositionOfLetter && letter != " " && letter != "\t")
	{
		//1.1 ĳ���� ���� ���� ��ġ�� 1��ŭ ������Ų��.
		//this->current=this->length-1�ϰ�� �� �ݺ����ǹ��� �����ؼ� ������ �ǰ�,
		//this->current++�� �ϸ鼭 this->current�� this->length�� ���� �������� �Ǵµ�
		//this->length�� ��ġ�� ���ڴ� ���⶧���� ���� ���ڸ� ������ ���̳�!
		//���� ���ǽ��� this->length-1���� �������� �ݺ��ؾ� ���������ڸ� �̹� �о��� ��쿡
		//�� �ݺ����� ���������� �Ǽ� ���� ���ڸ� �д� ������ �������� �ʴ´�.
		//this->Next(); Row�� ���곻���� Row�� ������ ����ȵ�
		this->current++;
		letter = this->GetAt(this->current)->GetContent();
	}
	//ĳ���� ���� ���� ��ġ�� �������� ������ ���� ��ġ�� ����, ���� ���� ���ڰ� �����̽� ����(����)��
	//�ǹ��ڰ� �ƴϸ�
	if (this->current == lastPositionOfLetter && letter != " " && letter != "\t")
	{
		//ĳ���� �������� ���������ں��� �ڿ� ��ġ��Ų��.
		//this->Last();
		this->current = this->length;
	}

#if 0
	//2. ĳ���� ���� ���� ��ġ��  ���� ���� ���������� ��ġ���� ������(�����̽��� �ǹ��ڰ� ������ ���)
	if (this->current < lastPositionOfLetter)
	{
#endif


	//2. ������ ���ڰ� �����̾����� ���ϱ� ���� ĳ���� ���� ���� ��ġ�� 1��ŭ ���ҽ�Ų��. 
	//���� ���� �������� ���� ���ڰ� ���̳� �����̽��ε� �� ���� ���ڰ� ���������� �߿�!
	//�װ� �ѱ����� ����, ����, Ư������������ ���� �ܾ���� ������ �̵������ �ٸ��⶧����
	//this->current--;
	//3. �����̽��� ���� ������ ������ ���ڰ� �����̾����� ���Ѵ�.
	//letter = this->GetAt(this->current)->GetContent();
	//4. ���� ���ڰ� �ѱ��� ���
#if 0
	if ((letter[0] & 0x80))
	{
		//4.1 ���� ���ڰ� �������� �ٽ� ���ϱ� ���� ĳ���� ���� ��ġ�� �ٽ� ����� �̵���Ų��.
		this->current++;
		//4.2 ���� ���ڰ� �������� ���Ѵ�.
		letter = this->GetAt(this->current)->GetContent();
		//4.3 ĳ���� ���� ���� ��ġ��  ���� ���� ���������� ��ġ���� �۰�
		//����(�����̽�����)�ε��� ���������� �̵��Ѵ�.
		while (this->current < lastPositionOfLetter && letter == " ")
		{
			//4.3.1. ĳ���� ���� ���� ��ġ�� 1��ŭ ������Ų��.
			this->current++;
			//4.3.2 ���� ���ڸ� �д´�.
			letter = this->GetAt(this->current)->GetContent();
		}
	}
#endif

	//5. ĳ���� ���� ���� ��ġ��  ���� ���� ���������� ��ġ���� �۰� 
	//���� ���ڰ� �ǹ����� ���� �ݺ��Ѵ�.
	while (this->current < lastPositionOfLetter && letter == "\t")
	{
		//5.1 ĳ���� ���� ���� ��ġ�� 1��ŭ ������Ų��.
		//this->Next();
		this->current++;
		letter = this->GetAt(this->current)->GetContent();
	}
	//ĳ���� ���� ���� ��ġ�� ���� ���� ������ ���� ��ġ�� ����, 
	//���� ���ڰ� �ǹ����̸�
	if (this->current == lastPositionOfLetter && letter == "\t")
	{
		//ĳ���� ���� ���� ��ġ�� ���� ���� ������ ���ں��� �ڿ� ��ġ��Ų��.
		//this->Last();
		this->current = this->length;
	}
	//6. ĳ���� ���� ���� ��ġ��  ���� ���� ���������� ��ġ���� �۰�
	//���� ���ڰ� ����(�����̽���)�����ε��� �ݺ��Ѵ�.
	while (this->current < lastPositionOfLetter && letter == " ")
	{
		//6.1 ĳ���� ���� ���� ��ġ�� 1��ŭ ������Ų��.
		//this->Next();
		this->current++;
		letter = this->GetAt(this->current)->GetContent();
	}
	//ĳ���� ���� ���� ��ġ�� ���� ���� ������ ���� ��ġ�� ����, 
	//���� ���ڰ� �����̽�����(����)�̸�
	if (this->current == lastPositionOfLetter && letter == " ")
	{
		//ĳ���� ���� ���� ��ġ�� ���� ���� ������ ���ں��� �ڿ� ��ġ��Ų��.
		//this->Last();
		this->current = this->length;
	}
	//}
	//7. ĳ���� ���� ���� ��ġ�� �̵��ϱ� ���� ��ġ�� ������(�̵����� �ʾ�����)
	//if (this->current == current)
	//{
		//7.1 ĳ���� ���� ���� ��ġ�� ���������� �̵������ش�
	//	this->current = this->length;
	//}
	//8. ���� ĳ���� ���� ��ġ�� ����Ѵ�.
	return this->current;
}

//�ٿ��� ĳ���� �ܾ������ �������� �̵���Ű��(Row����)
Long Composite::PreviousWord()
{
#if 0
	Long index = this->current - 1;
	string letter = this->GetAt(index)->GetContent();
	//1.3.3 ���� ĳ���� ���� ��ġ�� 0���� ũ�� ���� ���ڰ� �����̽�(����)�����ε��� �ݺ��Ѵ�.
	while (index > 0 && letter == " ")
	{
		//1.3.3.1 ���� ĳ���� ���� ��ġ�� �������� �̵���Ų��.
		//index = this->GetAt(this->current)->Previous();
		//index = this->current--;
		index--;
		//index = this->current;
		//1.3.3.2 ���� ĳ���� ���� ��ġ�� �ش��ϴ� ���ڸ� �д´�.
		//letter = this->GetAt(this->current)->GetAt(index)->GetContent();
		letter = this->GetAt(index)->GetContent();
	}
	//1.3.5 ���� ĳ���� ���� ��ġ�� 0���� ũ�� ���� ���ڰ� �ǹ����ε��� �ݺ��Ѵ�.
	while (index > 0 && letter == "\t")
	{
		//1.3.5.1 ���� ĳ���� ���� ��ġ�� �������� �̵���Ų��.
		//index = this->GetAt(this->current)->Previous();
		//index = this->current--;
		index--;
		//index = this->current;
		//1.3.5.2 ���� ĳ���� ���� ��ġ�� �ش��ϴ� ���ڸ� �д´�.
		//letter = this->GetAt(this->current)->GetAt(index)->GetContent();
		letter = this->GetAt(index)->GetContent();
	}
	//1.3.6 ���� ĳ���� ���� ��ġ�� 0���� ũ�� ���� ���ڰ� �����̽�(����)���� �׸���
	//�ǹ��ڰ� �ƴѵ��� �ݺ��Ѵ�.
	while (index > 0 && letter != " " && letter != "\t")
	{
		//isStopped == true;
		//1.3.6.1 ���� ĳ���� ���� ��ġ�� �������� �̵���Ų��.
		//index = this->GetAt(this->current)->Previous();
		//index = this->current--;
		index--;
		//index = this->current;
		//1.3.6.2 ���� ĳ���� ���� ��ġ�� �ش��ϴ� ���ڸ� �д´�.
		//letter = this->GetAt(this->current)->GetAt(index)->GetContent();
		letter = this->GetAt(index)->GetContent();
	}
#endif
#if 0
	//1. ĳ���� ���� ������ġ�� 1��ŭ ���ҽ�Ų��.
	this->current--;
	string letter;
	//2. ĳ���� ���� ���� ��ġ�� 0���� ũ�� ���� ���ڰ� �����̽� �Ǵ� ���ε��� �ݺ��Ѵ�.
	if (this->current > 0)
	{
		letter = this->GetAt(this->current)->GetContent();
	}
	while (this->current > 0 && letter == " " || letter == "\t")
	{
		//2.1 ĳ���� ���� ��ġ�� ���ҽ�Ų��.
		this->current--;
	}
	//3. ���� ĳ���� ��ġ�� 0���� ũ�ų� ���� ���� ���ڰ� �����̽��� �ƴϰ� �ǵ� �ƴϸ�
	if (this->current >= 0)
	{
		letter = this->GetAt(this->current)->GetContent();
	}
	if (this->current >= 0 && letter != " " && letter != "\t")
	{
		if (this->current > 0)
		{
			letter = this->GetAt(this->current)->GetContent();
		}
		//3.1 ĳ���� ���� ���� ��ġ�� 0���� ũ�� ���� ���ڰ� �����̽��� ���� �ƴѵ��� �ݺ��Ѵ�.
		while (this->current > 0 && letter != " " && letter != "\t")
		{
			this->current--;
		}
		//3.2 ���� ���ڰ� �����̽� �Ǵ� ���̸�
		letter = this->GetAt(this->current)->GetContent();
		if (letter == " " || letter == "\t")
		{
			//3.2.1 ĳ���� ���� ������ġ�� 1��ŭ ������Ų��.
			this->current++;
		}
	}
	//4. ���� ĳ���� ��ġ�� 0�̰� ���ڰ� �����̽� �Ǵ� ���̸�
	else if (this->current == 0 && letter == " " || letter == "\t")
	{
		//4.1 ĳ���� ���� ���� ��ġ�� 1��ŭ ���ҽ�Ų��.
		this->current--;
	}
	//5. ĳ���� ���� ���� ��ġ�� ����Ѵ�.
	return this->current;
#endif
#if 0
	//1. ĳ���� ���� ���� ��ġ�� ���� ���� ù��° ���� ��ġ���� ũ��,(�� �� ������ �̵��̰�)
	//���ڰ� �����̽������ε��� �ݺ��Ѵ�. �� �� ������ �̵��ϴ°Ÿ� ó������.
	//�ٿ��� �ٴ��� �̵���  Note->PreviousWord()���� ó������!
	//(ĳ���� �������� ������ ���� ���(ĳ���� ������ ���� �ڿ� ��ġ�� ���)�� �ٿ��� �ٷ� �̵��ϰ� �Ǵµ� 
	//�̴� Note->PreviousWord()���� ó������!, ���⼭�� ���� �������� �ܾ���� �̵��ϴ°Ÿ� ó������!)
	string letter;//�� ���ڸ� ���� ����
	this->Previous();//ĳ���� ����������ġ�� �����ϰ� �ִµ� �̵��� �������� �ϱ� ������
	//���� �ش� ���ڸ� �������� -1�� ����� ���� �ش� ���ڸ� �а� �̵��� �� �ִ�.
	letter = this->GetAt(this->current)->GetContent();
	while (this->current > 0 && letter == " ")
	{
		//6..1 ĳ���� ���� ���� ��ġ�� 1��ŭ ���ҽ�Ų��.
		this->Previous();
		letter = this->GetAt(this->current)->GetContent();
	}
	//ĳ���� ���� ���� ��ġ�� ���� ���� ù��° ���� ��ġ�� ����, 
	//���� ���ڰ� �����̽�����(����)�̸�
	if (this->current == 0 && letter == " ")
	{
		//ĳ���� ���� ���� ��ġ�� ���� ���� ������ ���ں��� �ڿ� ��ġ��Ų��.
		this->Last();
	}
#endif

	//1. ���� ĳ���� ���� ��ġ�� ���ҽ�Ų��.
	//Long index = this->GetAt(index)->Previous(); this�� Row�̹Ƿ� ��ǥ���� ���ڿ� �����ϰԵ�
	//Row�� ����ȿ��� Row�� �Լ��� ����ϸ� �ȵ�!
	//Long index = this->current--; �̷��� �ϸ� index�� ����Ǵ� ���� current�� -1�� �Ǳ� ����
	//���� ����ǰ� this->current�� -1�� ���ҵ� ���� ����Ǽ� index�� this->current�� ���� 1���̰���
	//Long previousCaretIndex = this->current;
	//bool isStopped = false;
	//Long index = this->current;
	//1.3.2 ���� ĳ���� ���� ��ġ�� �ش��ϴ� ���ڸ� �д´�.
	//GetAt�� �ι��ϸ� X
	//string letter = this->GetAt(this->current)->GetAt(index)->GetContent();
#if 0
	//1. ���ڸ� �б� ���� ���� ĳ���� ���� ��ġ�� 1��ŭ ���ҽ�Ų��. 
	this->current--;
	//2. ���� ĳ���� ���� ��ġ�� �ش��ϴ� ���ڸ� �д´�.
	string letter = this->GetAt(this->current)->GetContent();
	//3. ���� ĳ���� ���� ��ġ�� 0���� ũ�� ���� ���ڰ� �����̽�(����)�����ε��� �ݺ��Ѵ�.
	while (this->current > 0 && letter == " ")
	{
		//1.3.3.1 ���� ĳ���� ���� ��ġ�� �������� �̵���Ų��.
		//index = this->GetAt(this->current)->Previous();
		//index = this->current--;
		this->current--;
		//index = this->current;
		//1.3.3.2 ���� ĳ���� ���� ��ġ�� �ش��ϴ� ���ڸ� �д´�.
		//letter = this->GetAt(this->current)->GetAt(index)->GetContent();
		letter = this->GetAt(this->current)->GetContent();
	}
	//1.3.5 ���� ĳ���� ���� ��ġ�� 0���� ũ�� ���� ���ڰ� �ǹ����ε��� �ݺ��Ѵ�.
	while (this->current > 0 && letter == "\t")
	{
		//1.3.5.1 ���� ĳ���� ���� ��ġ�� �������� �̵���Ų��.
		//index = this->GetAt(this->current)->Previous();
		//index = this->current--;
		this->current--;
		//index = this->current;
		//1.3.5.2 ���� ĳ���� ���� ��ġ�� �ش��ϴ� ���ڸ� �д´�.
		//letter = this->GetAt(this->current)->GetAt(index)->GetContent();
		letter = this->GetAt(this->current)->GetContent();
	}
	//1.3.6 ���� ĳ���� ���� ��ġ�� 0���� ũ�� ���� ���ڰ� �����̽�(����)���� �׸���
	//�ǹ��ڰ� �ƴѵ��� �ݺ��Ѵ�.
	while (this->current > 0 && letter != " " && letter != "\t")
	{
		isStopped == true;
		//1.3.6.1 ���� ĳ���� ���� ��ġ�� �������� �̵���Ų��.
		//index = this->GetAt(this->current)->Previous();
		//index = this->current--;
		this->current--;
		//index = this->current;
		//1.3.6.2 ���� ĳ���� ���� ��ġ�� �ش��ϴ� ���ڸ� �д´�.
		//letter = this->GetAt(this->current)->GetAt(index)->GetContent();
		letter = this->GetAt(this->current)->GetContent();
	}
	//1.3.7 ���� ĳ���� ���� ��ġ�� 0���� ũ��
#endif

	//1. ���� ĳ���� ���� ��ġ ������ �ش��ϴ� ���ڸ� �д´�.
	string letter = this->GetAt(this->current - 1)->GetContent();
	//2. ���� ĳ���� ���� ��ġ�� 1���� ũ�� ���� ���ڰ� �����̽�(����)�����ε��� �ݺ��Ѵ�.
	while (this->current > 1 && letter == " ")
	{
		//2.1 ���� ĳ���� ���� ��ġ�� ��ĭ �������� �̵���Ų��.
		this->current--;
		//2.2 ���� ĳ���� ���� ��ġ ������ �ش��ϴ� ���ڸ� �д´�.
		letter = this->GetAt(this->current - 1)->GetContent();
	}
	//3. ���� ĳ���� ���� ��ġ�� 1�̰� ���� ���ڰ� �����̽�(����)�����̸�
	if (this->current == 1 && letter == " ")
	{
		//3.1 ���� ĳ���� ���� ��ġ�� 0���� �̵���Ų��.
		this->current = 0;
	}
	//4. ���� ĳ���� ���� ��ġ�� 1���� ũ�� ���� ���ڰ� �ǹ����ε��� �ݺ��Ѵ�.
	while (this->current > 1 && letter == "\t")
	{
		//4.1 ���� ĳ���� ���� ��ġ�� ��ĭ �������� �̵���Ų��.
		this->current--;
		//4.2 ���� ĳ���� ���� ��ġ ������ �ش��ϴ� ���ڸ� �д´�.
		letter = this->GetAt(this->current - 1)->GetContent();
	}
	//5. ���� ĳ���� ���� ��ġ�� 1�̰� ���� ���ڰ� �ǹ����̸�
	if (this->current == 1 && letter == "\t")
	{
		//5.1 ���� ĳ���� ���� ��ġ�� 0���� �̵���Ų��.
		this->current = 0;
	}
	//6. ���� ĳ���� ���� ��ġ�� 1���� ũ�� ���� ���ڰ� �����̽�(����)���� �׸���
	//�ǹ��ڰ� �ƴѵ��� �ݺ��Ѵ�.
	while (this->current > 1 && letter != " " && letter != "\t")
	{
		//6.1 ���� ĳ���� ���� ��ġ�� �������� �̵���Ų��.
		this->current--;
		//6.2 ���� ĳ���� ���� ��ġ�� �ش��ϴ� ���ڸ� �д´�.
		letter = this->GetAt(this->current - 1)->GetContent();
	}
	//7. ���� ĳ���� ���� ��ġ�� 1�̰� ���� ���ڰ� �����̽�(����) ���ڿ� �ǹ��ڰ� �ƴϸ�
	if (this->current == 1 && letter != " " && letter != "\t")
	{
		//7.1 ���� ĳ���� ���� ��ġ�� 0���� �̵���Ų��.
		this->current = 0;
	}
	//8. ���� ĳ���� ���� ��ġ�� ����Ѵ�.
	return this->current;
}