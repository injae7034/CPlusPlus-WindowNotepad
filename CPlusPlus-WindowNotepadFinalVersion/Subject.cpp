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
	// 함수포인터 = [캡처절] (매개변수) mutable(옵션) -> 반환형식 { 함수내용 }
   /*
   캡처절에 해당 영역의 데이터를 넣어줄 수 있다.
   ex) [index]
   하지만 값을 복제해서 가져오는 것이라 변경할 수 없고 참조형으로 가져오면 바깥의 index 값을 수정할 수 있다.
   mutable 속성을 사용하면 index의 값을 변경할 순 있으나 복제된 값이기에 바깥의 index와는 연관이 없다.
   */
	//람다식표현(함수포인터를 외부에 따로 안쓰고 함수 스택내에서 함수포인터를 정의해서 바로 사용하고,
	//함수스택이 종료될 때 같이 사라짐!)
	auto CompareTemp = [](void* one, void* other) -> int
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
	};
	//1. 입력받은 옵저버의 주소로 옵저버링크배열의 위치를 구한다.
	Long index = this->observers.LinearSearchUnique(observer, CompareTemp);
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
