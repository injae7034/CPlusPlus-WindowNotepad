#ifndef _DOUBLEBYTELETTER_H
#define _DOUBLEBYTELETTER_H

#include "Letter.h"
#include "Glyph.h"

class DoubleByteLetter :public Letter
{
public:
	DoubleByteLetter();//����Ʈ������
	DoubleByteLetter(char(*script));//�Ű�����1��������
	DoubleByteLetter(const DoubleByteLetter& source);//���������
	DoubleByteLetter& operator=(const DoubleByteLetter& source);//ġȯ������
	virtual ~DoubleByteLetter();//�Ҹ���
	virtual Glyph* Clone();//Prototype Pattern Clone
	virtual string GetContent();
	//�ζ����Լ�
	char* GetScript() const;

private:
	char script[2];
};

//�ζ����Լ� ����
//char*(*(������)�ּҷ� �迭�� ������)���ڹ迭�� �ּҸ� ��ȯ�ϸ� �� �迭 ��ü�� ��ȯ�ϴ°� ���� �ȵǰ� ��ȿ������. �迭 �ּҸ� �˸� ������ �� �ֱ� ������
inline char* DoubleByteLetter::GetScript() const
{
	return const_cast<char*>(this->script);
}

#endif // !_SINGLEBYTELETTER_H
