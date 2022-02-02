#ifndef _OBSERVER_H
#define _OBSERVER_H

class Subject;

class Observer
{
public:
	Subject* subject;
public:
	Observer(Subject* subject = 0);//����Ʈ������
	virtual ~Observer() = 0;//�߻�Ŭ���� �Ҹ���
	virtual void Update() = 0;//Subject�� �˸��� ���� ���¸� ��ȭ��Ŵ.
};

#endif // !_OBSERVER_H

