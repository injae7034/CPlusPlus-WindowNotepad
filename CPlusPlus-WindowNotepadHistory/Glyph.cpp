#include "Glyph.h"

//����Ʈ������
Glyph::Glyph()
{

}

//�Ҹ���
Glyph::~Glyph()
{

}

//Add
Long Glyph::Add(Glyph* glyph)
{
	return -1;//�迭 ÷�ڰ� ���� ��� -1�� ��ȯ 0�� ù �迭����̰� �迭��Ұ� ���ٴ� ���� -1��.
}

//Remove
Long Glyph::Remove(Long index)
{
	return -1;//�迭 ÷�ڰ� ���� ��� -1�� ��ȯ 0�� ù �迭����̰� �迭��Ұ� ���ٴ� ���� -1��.
}

//GetAt
Glyph* Glyph::GetAt(Long index)
{
	return 0;//��ġ�� null�� ��� 0�� ��ȯ
}

//Clone
Glyph* Glyph::Clone()
{
	return 0;//Glyph*(��ġ)�� null�� ��� 0�� ��ȯ
}

//GetContent
string Glyph::GetContent()
{
	return "\0";
}



