#include "DoubleByteLetter.h"

//����Ʈ������
DoubleByteLetter::DoubleByteLetter()
{
	//���� �迭 �ʱ�ȭ�� ���� �迭 ����ҿ� �ι���(\0)�� ���������ν� �Ѵ�. ���� �迭�� ���ڿ��̶� �ٸ���.
	this->script[0] = '\0';
	this->script[1] = '\0';
}

//�Ű����� 1�� ������
DoubleByteLetter::DoubleByteLetter(char(*script))
// char(*script)�� �迭�������̰� char* script�� �׳� ������(�ּ�)�� �����Ѱ���
//����� ���ڹ迭�̱� ������ �迭������(�迭�� �����ּ�)�� �Է��ϴ°��� �´�
{
	//���ڹ迭�� �Ű������� �̿��� �� �����ּ�(char*)�� �Է��Ѵ�.
	this->script[0] = script[0];
	this->script[1] = script[1];
}

//���������
DoubleByteLetter::DoubleByteLetter(const DoubleByteLetter& source)
{
	//���ڹ迭�� ���ڿ�ó�� �ѹ��� ������ �Ұ�����. �� �迭��ҿ� ���� ������ �������!
	this->script[0] = source.script[0];
	this->script[1] = source.script[1];
}

//ġȯ������
DoubleByteLetter& DoubleByteLetter::operator=(const DoubleByteLetter& source)
{
	this->script[0] = source.script[0];
	this->script[1] = source.script[1];

	return *this;
}

//�Ҹ���
DoubleByteLetter::~DoubleByteLetter()
{

}

//Prototype Pattern Clone
Glyph* DoubleByteLetter::Clone()
{
	return new DoubleByteLetter(*this);
}

//GetContent
string DoubleByteLetter::GetContent()
{
	string script;
	script.push_back(this->script[0]);
	script.push_back(this->script[1]);
	//script.push_back('\0');

	return script;
}