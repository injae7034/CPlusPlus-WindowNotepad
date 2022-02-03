#ifndef _SUBJECT_H
#define _SUBJECT_H
#include "Array.h"

typedef signed long int Long;
class Observer;

class Subject {
public:
	Subject(Long capacity = 8);//디폴트생성자 기본값은 바이트단위로 주기(8,16,...)
	virtual ~Subject() = 0;//추상클래스 소멸자(순수가상함수)(내용X, 상속받는 클래스들이 재정의 필수)
	//상속받는 클래스(notepadForm)이 재정의를 안하기 때문에 앞에 virtual를 안붙임.
	void Attach(Observer* observer);//옵저버등록
	void Detach(Observer* observer);//옵저버해지
	void Notify();//옵저버에게 변경되었다고 알림
	Long Insert(Long index, Observer* observer);//옵저버를 index에 끼워넣기
	Observer* GetAt(Long index);
	Long Search(Observer* key);//찾고자하는 옵저버의 주소를 넣으면 그 배열위치를 반환해줌.
	//인라인함수 선언
	Long GetCapacity() const;
	Long GetLength() const;
protected://상속받는 클래스들이 사용할 수 있도록 protected로 바꿈
	Array<Observer*> observers;//옵저버들의 주소를 저장하고 있는 배열
	Long capacity;
	Long length;
};

//인라인함수 정의
inline Long Subject::GetCapacity() const
{
	return this->capacity;
}
inline Long Subject::GetLength() const
{
	return this->length;
}

//함수포인터 선언
int CompareObserver(void* one, void* other);

#endif // !_SUBJECT_H

