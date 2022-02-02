#ifndef _OBSERVER_H
#define _OBSERVER_H

class Subject;

class Observer
{
public:
	Subject* subject;
public:
	Observer(Subject* subject = 0);//디폴트생성자
	virtual ~Observer() = 0;//추상클래스 소멸자
	virtual void Update() = 0;//Subject의 알림에 따라 상태를 변화시킴.
};

#endif // !_OBSERVER_H

