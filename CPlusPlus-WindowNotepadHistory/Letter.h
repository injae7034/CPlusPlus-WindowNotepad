#ifndef _LETTER_H
#define _LETTER_H

#include "Glyph.h"
typedef signed long int Long;

class Letter :public Glyph
{
public:
	Letter(Long capacity = 256);//����Ʈ������
	//Letter(const Letter& source);//��������� �ʿ����
	//Letter& operator=(const Letter& source);//ġȯ������ �ʿ����
	virtual ~Letter() = 0;//�߻�Ŭ���� �Ҹ���
	//�߻�Ŭ����(��ü�� ����X)�̹Ƿ� Clone�� ����X, Concrete Prototype, �� ��ü���� ������ �ִ� �ڽĵ�(SingleByterLetter, DoubleByteLetter) ������Ͽ��� Clone�޼ҵ带 �������ְ� cpp���� �������ָ� ��.
	//�ζ����Լ�����
	//GetScript�� ��ȯ���� char�̳� char*�� �ٸ��⶧���� ���⼭ �ζ����Լ����Ǹ� ����X
	//���Լ� �ʿ����
	//bool IsEqual(const Letter& other);
	//bool IsNotEqual(const Letter& other);
	//�������Լ� �ʿ����
	//bool operator==(const Letter& other);
	//bool operator!=(const Letter& other);
};

#endif // !_LETTER_H
