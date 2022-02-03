#ifndef _LETTER_H
#define _LETTER_H

#include "Glyph.h"
typedef signed long int Long;

class Letter :public Glyph
{
public:
	Letter(bool isisSelected = false);//����Ʈ������
	virtual void Select(bool isSelected);
	virtual ~Letter() = 0;//�߻�Ŭ���� �Ҹ���
	//�߻�Ŭ����(��ü�� ����X)�̹Ƿ� Clone�� ����X, Concrete Prototype, �� ��ü���� ������ �ִ� �ڽĵ�(SingleByterLetter, DoubleByteLetter) ������Ͽ��� Clone�޼ҵ带 �������ְ� cpp���� �������ָ� ��.
	//�ζ����Լ�����
	virtual bool IsSelected() const;
protected:
	bool isSelected;
};

//�ζ����Լ� ����
inline bool Letter::IsSelected() const
{
	return this->isSelected;
}

#endif // !_LETTER_H
