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

//Add(Insert)
Long Glyph::Add(Long index, Glyph* glyph)
{
	return -1;
}

//Remove
Long Glyph::Remove(Long index)
{
	return -1;//�迭 ÷�ڰ� ���� ��� -1�� ��ȯ 0�� ù �迭����̰� �迭��Ұ� ���ٴ� ���� -1��.
}

Glyph* Glyph::Split(Long index)
{
	return 0;
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

//GetPartOfContent
string Glyph::GetPartOfContent(Long current)
{
	return "\0";
}

//First
Long Glyph::First()
{
	return -1;
}

//Last
Long Glyph::Last()
{
	return -1;
}

//Previous
Long Glyph::Previous()
{
	return -1;
}

//Next
Long Glyph::Next()
{
	return -1;
}

//NextWord
Long Glyph::NextWord()
{
	return -1;
}

//PreviousWord
Long Glyph::PreviousWord()
{
	return - 1;
}