#include "Subject.h"
#include "Observer.h"
#include "CaretController.h"

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
Long Subject::Attach(Observer* observer)
{
	Long index;
	//1. observer링크를 입력받는다.
	//2. 사용량이 할당량보다 작으면
	if (this->length < this->capacity)
	{
		index = this->observers.Store(this->length, observer);
	}
	//3. 그렇지 않으면
	else
	{
		index = this->observers.AppendFromRear(observer);
		//3.1 할당량을 증가시킨다.
		this->capacity++;
	}
	//4. 사용량을 증가시킨다.
	this->length++;
	//5. 위치를 반환한다.
	return index;
}

Long Subject::Attach(Long index, Observer* observer)
{
	//1. 끼워 넣을 위치와 observer링크를 입력받는다.
	//2. observer링크를 index번째에 끼워 넣는다.
	index = this->observers.Insert(index, observer);
	//3. 할당량을 증가시킨다.
	this->capacity++;
	//4. 사용량을 증가시킨다.
	this->length++;
	//5. 현재 글자의 위치를 출력한다.
	return index;
}

//옵저버해지
void Subject::Detach(Observer* observer)
{
	//1. 입력받은 옵저버의 주소로 옵저버링크배열의 위치를 구한다.
	Long index = this->observers.LinearSearchUnique(observer, CompareObserver);
	//2. 옵저버의 주소를 이용해 힙에 할당된 옵저버의 내용을 할당해제한다.
	if (index != -1)
	{
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

//Search
Long Subject::Search(Observer* key)
{
	//1. 입력받은 옵저버의 주소로 옵저버링크배열의 위치를 구한다.
	Long index = this->observers.LinearSearchUnique(key, CompareObserver);
	//2. 위치를 출력한다.
	return index;
#if 0
	//1. 찾고자 하는 옵저버를 매개변수로 입력받는다.
	Long index = 0;
	Observer* observer = 0;
	//2. index가 length보다 작은 동안 그리고 찾고자 하는 옵저버가 아닌동안 반복한다.
	while (index < this->length && dynamic_cast<Observer*>(observer) != key)
	{
		//2.1 옵저버 리스트에서 옵저버를 구한다.
		observer = this->observers.GetAt(index);
		//2.2 index를 증가시킨다.
		index++;
	}
	//3. 찾고자 하는 옵저버를 찾았으면
	if (dynamic_cast<Observer*>(observer) == key)
	{
		//3.1 index를 감소시켜준다.
		index--;
	}
	//4. 찾고자 하는 옵저버를 못찾았으면
	else
	{
		index = -1;
	}
	//5. 찾은 위치를 반환한다.
	return index;
#endif
}

//메모리맵을 그려서 정확하게 비교하자
//Obwerver*와 Observer**는 엄연히 다르다!! 그 차이를 알기 위해서 메모리맵을 그려서 비교해보자!
//함수포인터 정의
int CompareObserver(void* one, void* other)
{
	Observer** one_ = (Observer**)one;
	Observer* other_ = (Observer*)other;
	int ret;
	if (*one_ == other_)
	{
		ret = 0;
	}
	else
	{
		ret = 1;
	}
	return ret;
}

