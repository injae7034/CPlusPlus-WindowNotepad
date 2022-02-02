#include "Row.h"

//����Ʈ������
Row::Row(Long capacity)
	:Composite(capacity)
{
	
}

//���������
Row::Row(const Row& source)
	:Composite(source)
{

}

//ġȯ������
Row& Row::operator=(const Row& source)
{
	Composite::operator=(source);

	return *this;
}

//�Ҹ���
Row::~Row()
{

}

//Prototype Patter Clone
Glyph* Row::Clone()
{
	return new Row(*this);
}

//GetContent
string Row::GetContent()
{
	string content;//���ڵ��� ������ ����(��)
	string letter;//���ڸ� ������ ����
	Long index = 0;
	//1. ������ ���ڰ�����ŭ �ݺ��Ѵ�.
	while (index < this->length)
	{
		//1.1 letter�� �ѱ���(Row�� �迭��� 1��)�� �����Ѵ�.
		letter = this->glyphs.GetAt(index)->GetContent();
		//1.2 letter�� �ǹ����̸�
		if (letter == "\t")
		{
			//1.2.1 ������ 8���� �ٲ۴�.
			letter = "        ";
		}
		//1.3 �ٿ� ���ڸ� �߰��Ѵ�.
		content += letter;
		index++;
	}
	//2. ���� ��ȯ�Ѵ�.
	return content;
	//3. ������.
}