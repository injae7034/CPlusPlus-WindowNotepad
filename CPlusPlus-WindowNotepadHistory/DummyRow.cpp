#include "DummyRow.h"

//����Ʈ������
DummyRow::DummyRow(Long capacity)
	:Row(capacity)//Row�� �⺻�����ڸ� ȣ���ϸ� �ű⼭ Compostie�� �����ڰ� ȣ��Ǽ�
	//�������(capacity, length, current ���� �ʱ�ȭ�ȴ�!)
{

}

//���������
DummyRow::DummyRow(const DummyRow& source)
	:Row(source)
{

}

//ġȯ������
DummyRow& DummyRow::operator=(const DummyRow& source)
{
	Row::operator=(source);

	return *this;
}

//�Ҹ���
DummyRow::~DummyRow()
{

}

//Prototype Patter Clone
Glyph* DummyRow::Clone()
{
	return new DummyRow(*this);
}