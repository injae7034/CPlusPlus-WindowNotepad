#include "SingleByteLetter.h"

//����Ʈ������
SingleByteLetter::SingleByteLetter()
{
	this->script = '\0';
	//�����ʱ�ȭ�� �ι���(\0)�� �Ѵ�. (��ĭ)""�� ���ڿ��̶� string���� �ʱ�ȭ �� �� �����.
}

//�Ű����� 1�� ������
SingleByteLetter::SingleByteLetter(char script)
{
	this->script = script;
}

//���������
SingleByteLetter::SingleByteLetter(const SingleByteLetter& source)
{
	this->script = source.script;
}

//ġȯ������
SingleByteLetter& SingleByteLetter::operator=(const SingleByteLetter& source)
{
	this->script = source.script;
	
	return *this;
}

//�Ҹ���
SingleByteLetter::~SingleByteLetter()
{

}

//Prototype Pattern Clone
Glyph* SingleByteLetter::Clone()
{
	return new SingleByteLetter(*this);
}

//GetContent
string SingleByteLetter::GetContent()
{
	string script;
	script.push_back(this->script);
	//script.push_back('\0');

	return script;
}