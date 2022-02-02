#ifndef _SUBJECT_H
#define _SUBJECT_H
#include "Array.h"

typedef signed long int Long;
class Observer;

class Subject {
public:
	Subject(Long capacity = 8);//����Ʈ������ �⺻���� ����Ʈ������ �ֱ�(8,16,...)
	virtual ~Subject() = 0;//�߻�Ŭ���� �Ҹ���
	void Attach(Observer* observer);//���������
	void Detach(Observer* observer);//����������
	void Notify();//���������� ����Ǿ��ٰ� �˸�
protected://��ӹ޴� Ŭ�������� ����� �� �ֵ��� protected�� �ٲ�
	Array<Observer*> observers;//���������� �ּҸ� �����ϰ� �ִ� �迭
	Long capacity;
	Long length;
};

//�Լ������� ����
int CompareObserver(void* one, void* other);

#endif // !_SUBJECT_H

