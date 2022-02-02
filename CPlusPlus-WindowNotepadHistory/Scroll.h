#ifndef _SCROLL_H
#define _SCROLL_H

#include<afxwin.h>//SCROLLINFO�� ����ϱ� ����

class ScrollController;//���漱��
typedef signed long int Long;

class Scroll
{
	friend ScrollController;//������Ŭ���� ���
	//ScrollController���� Scroll�� ������ �����ϰ� ������ �� ����.
	//ScrollController�� Scroll�� ���� ������ �����ϹǷ� Scroll�� ���� �����ϰ� ���� �����ų �� �ְ�
	//Friend Ŭ���� ����� �ϴ°��� �´�(�ƴϸ� ���������� ���� �Ź� ScrollController���� Scroll��
	//�����ڸ� ȣ���ϰ� �Ҵ������� ������ϴµ� �̰� �Ź��Ϸ��� �ϸ� ��ȿ������!)
public:
	ScrollController* scrollController;
public:
	Scroll(ScrollController* scrollController = 0);//����Ʈ������
	Scroll(ScrollController* scrollController, Long currentPos, Long min, Long max,
		Long pageSize);//�Ű������� 5�������� ������
	Scroll(const Scroll& source);//���������
	Scroll& operator=(const Scroll& source);//ġȯ������
	//Scroll �̵�����
	virtual Long First();
	virtual Long Last();
	virtual Long LinePrevious() = 0;
	virtual Long LineNext() = 0;
	virtual Long pagePrevious();
	virtual Long pageNext();
	virtual Long Move(Long currentPos);
	virtual ~Scroll() = 0;//�߻�Ŭ���� �Ҹ���
	//�ζ����Լ� ����
	Long GetCurrentPos() const;
	Long GetMin() const;
	Long GetMax() const;
	Long GetPageSize() const;
protected:
	Long currentPos;//����ȭ����ġ
	Long min;//���� �Ǵ� ���� �ؽ�Ʈ �ּҰ�
	Long max;//���� �Ǵ� ���� �ؽ�Ʈ �ִ밪
	Long pageSize;//���� ȭ���� ���� �Ǵ� ���� ũ��
};

//�ζ����Լ� ����
inline Long Scroll::GetCurrentPos() const
{
	return this->currentPos;
}
inline Long Scroll::GetMin() const
{
	return this->min;
}
inline Long Scroll::GetMax() const
{
	return this->max;
}
inline Long Scroll::GetPageSize() const
{
	return this->pageSize;
}


#endif // !_SCROLL_H
