#ifndef _SUBJECT_H
#define _SUBJECT_H
#include "Array.h"

typedef signed long int Long;
class Observer;

class Subject {
public:
	Subject(Long capacity = 8);//디폴트생성자 기본값은 바이트단위로 주기(8,16,...)
	virtual ~Subject() = 0;//추상클래스 소멸자
	void Attach(Observer* observer);//옵저버등록
	void Detach(Observer* observer);//옵저버해지
	void Notify();//옵저버에게 변경되었다고 알림
protected://상속받는 클래스들이 사용할 수 있도록 protected로 바꿈
	Array<Observer*> observers;//옵저버들의 주소를 저장하고 있는 배열
	Long capacity;
	Long length;
};

//함수포인터 선언
int CompareObserver(void* one, void* other);

#endif // !_SUBJECT_H

