#include "Clipboard.h"

//����Ʈ������
Clipboard::Clipboard(Long capacity)
	:Composite(capacity)
{

}

//���������
Clipboard::Clipboard(const Clipboard& source)
	:Composite(source)
{

}

//ġȯ������
Clipboard& Clipboard::operator=(const Clipboard& source)
{
	//this->Composite::operator=(source);
	Composite::operator=(source);

	return *this;
}

//�Ҹ���
Clipboard::~Clipboard()
{

}

//Prototype Patter Clone
Glyph* Clipboard::Clone()
{
	return new Clipboard(*this);
}

//GetContent
string Clipboard::GetContent()
{
	//1. ������ �������� �ݺ��Ѵ�.
	string content;
	Long rowIndex = 0;
	Long lastRowIndex = this->length - 1;
	while (rowIndex < lastRowIndex)
	{
		content += this->glyphs.GetAt(rowIndex)->GetContent();
		content += '\n';
		rowIndex++;
	}
	//2. ���������� ó���Ѵ�.
	//(������ �ٸ� ���� ó���ϴ� ������ ������ ���� ���� ó�����ϰ� ���� �ݺ����� �־��ָ�
	//���๮�ڰ� ���� �ԷµǼ� ������ �� ������ �� ���� ������� �����̴�.)
	if (rowIndex == lastRowIndex)
	{
		content += this->glyphs.GetAt(rowIndex)->GetContent();
	}

	return content;
}


//Clipboard���ؿ� �°� Add, Remove ������(�������̵�)
Long Clipboard::Add(Glyph* glyph)
{
	//1. �߰��� ��Ʈ�� �ּҸ� �Է¹޴´�.
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
	//5. Ŭ�����忡 ����� ��Ʈ����Ʈ���� ���� ��Ʈ ��ġ�� �����Ѵ�.
	this->current = index;
	//6. ���� ��Ʈ�� ��ġ�� ����Ѵ�.
	return index;
}

Long Clipboard::Remove(Long index)
{
	//1. ���� ��Ʈ�� ��ġ�� �Է¹޴´�.
	//2. ��Ʈ����Ʈ���� ��Ʈ�� ����� ���� ����� ��Ʈ�� ��ġ���� ���� ��ġ�� ���� ��ġ�� �Ǳ� ������ 
	//���� ��Ʈ�� ��ġ�� index-1�� �����Ѵ�.
	this->current = index - 1;
	//3. �ش�迭��Ұ� �ּҸ� �����ϰ� �ֱ� ������ �ش�迭����� ���� �Ҵ�� ����(��Ʈ)�� ���� ��������.
	Glyph* glyph = this->glyphs[index];
	if (glyph != 0)
	{
		delete glyph;
	}
	//4. �ش���ġ�� �迭����� ���� �Ҵ�� ����(��Ʈ)�� �������� ���� �� �ּҸ� �����.
	index = this->glyphs.Delete(index);
	//5. �Ҵ緮�� ���ҽ�Ų��.
	this->capacity--;
	//6. ��뷮�� ���ҽ�Ų��.
	this->length--;
	//7. index�� ����Ѵ�.
	return index;
}

Long Clipboard::RemoveAll()
{
	//1. Ŭ�����忡 �ִ� ���縮��Ʈ�� ��� �����.
	Glyph* glyph = 0;
	Long i = 0;
	while (i < this->length)
	{
		//1.1 �ش�迭��Ұ� �ּҸ� �����ϰ� �ֱ� ������ �ش�迭����� ���� �Ҵ�� ����(��Ʈ)��
		//���� �����.
		glyph = this->glyphs[i];
		if (glyph != 0)
		{
			delete glyph;
		}
		//1.2 �ش���ġ�� �迭����� ���� �Ҵ�� ����(��Ʈ)�� �������� ���� �� �ּҸ� �����.
		i = this->glyphs.Delete(i);
		//1.3 �Ҵ緮�� ���ҽ�Ų��.
		this->capacity--;
		//1.4 ��뷮�� ���ҽ�Ų��.
		this->length--;
	}
	//2. �� ����������
	if (this->length == 0)
	{
		i = -1;
	}
	//3. i�� ����Ѵ�.
	return i;
}