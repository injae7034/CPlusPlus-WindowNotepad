#include "Subject.h"
#include "Observer.h"

//디폴트생성자
Subject::Subject(Long capacity)
	:observers(capacity)
{
	this->capacity = capacity;
	this->length = 0;
}

//소멸자
//AccountBook의 accounts가 account들의 주소를 저장하고 있는 힙에 할당된 배열인 것처럼
//Subject의 observers도 observer들의 주소를 저장하고 있는 힙에 할당된 배열이다.
//따라서 observer들의 주소를 저장하고 있는 힙에 할당된 배열을 지우기전에 먼저 그 주소들이 가지고 있는
//내용들을 힙에서 할당해제하고 그다음에 힙에서 그 주소들을 가지고 있는 배열을 할당해제해줘야한다.
//소멸자에서는 따로 length랑 capacity를 감소시켜줄 필요가 없다!
Subject::~Subject()
{
	Observer* observer;
	Long i = 0;
	while (i < this->length)
	{
		observer = this->observers.GetAt(i);
		if (observer != 0)
		{
			delete observer;
		}
		i++;
	}
}

//GetAt
Observer* Subject::GetAt(Long index)
{
	return this->observers.GetAt(index);
}

//옵저버등록
void Subject::Attach(Observer* observer)
{
	//1. observer링크를 입력받는다.
	//2. 사용량이 할당량보다 작으면
	if (this->length < this->capacity)
	{
		this->observers.Store(this->length, observer);
	}
	//3. 그렇지 않으면
	else
	{
		this->observers.AppendFromRear(observer);
		//3.1 할당량을 증가시킨다.
		this->capacity++;
	}
	//4. 사용량을 증가시킨다.
	this->length++;
}

//옵저버해지
void Subject::Detach(Observer* observer)
{
	//1. 입력받은 옵저버의 주소로 옵저버링크배열의 위치를 구한다.
	Long index = this->observers.LinearSearchUnique(observer, CompareObserver);
	//2. 옵저버의 주소를 이용해 힙에 할당된 옵저버의 내용을 할당해제한다.
	if (observer != 0)
	{
		delete observer;
	}
	//3. 옵저버링크배열의 배열요소(주소)를 지운다.
	this->observers.Delete(index);
	//4. 할당량을 감소시킨다.
	this->capacity--;
	//5. 사용량을 감소시킨다.
	this->length--;
}

//옵저버알림
void Subject::Notify()
{
	//1. length만큼 반복한다.
	Long i = 0;
	while (i < this->length)
	{
		//1.1 Observer목록에 엤는 옵저버들에게 Update하라고 메세지를 보냄!
		this->observers.GetAt(i)->Update();
		i++;
	}
}
#if 0
//Search
Observer* Subject::Search(size_t key)
{
	//1. observer주소배열에서 원하는 자료유형의 크기를 찾을 때까지 반복한다.
	Long i = 0;
	Observer* observer = 0;
	observer = this->observers.GetAt(i);
	//2. i가 length보다 작은 동안 그리고 옵저버가 캐럿매니저가 아닌동안 반복한다.
	while (i < this->length && sizeof(*observer) != key)
	{
		//2.1 옵저버 리스트에서 옵저버를 구한다.
		observer = this->observers.GetAt(i);
		//2.2 i를 감소시킨다.
		i++;
	}
	//3.
	if (sizeof(*observer) != key)
	{
		//3.1 
		observer = 0;

	}
	//if(sizeof)
	return observer;
}
#endif
//함수포인터 정의
int CompareObserver(void* one, void* other)
{
	int ret;
	if (one == other)
	{
		ret = 0;
	}
	else
	{
		ret = 1;
	}
	return ret;
}

