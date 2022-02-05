#ifndef _STACK_H
#define _STACK_H
//스택 라이브러리에 Array라이브러리를 적용하기!
#include "Array.h"
typedef signed long int Long;
template <typename T>
class Stack
{
public:
	Stack(Long capacity = 256);//디폴트 생성자(따로 매개변수 입력이 없으면 기본으로 256개를 할당함!)
	Stack(const Stack& source);//복사생성자
	~Stack();//소멸자
	Long Push(T object);//현재 마지막 배열요소의 다음에 삽입하고 그 위치를 반환함
	T& Pop();//마지막 배열 요소를 삭제하고 반환함(꺼내기O)
	T& GetTop();//스택에서 제일 마지막 위치를 반환함(꺼내기X)
	Stack<T>& operator=(const Stack& source);//치환연산자
	T& operator[](Long index);//첨자연산자
	T* operator+(Long index);//포인터연산자
	//인라인함수
	Long GetCapacity() const;
	Long GetLength() const;
	bool IsEmpty() const;
private:
	Array<T> objects;
	Long capacity;
	Long length;
};
//인라인함수 정의
template <typename T>
inline Long Stack<T>::GetCapacity() const
{
	return this->capacity;
}

template <typename T>
inline Long Stack<T>::GetLength() const
{
	return this->length;
}

template <typename T>
inline bool Stack<T>::IsEmpty() const
{
	bool isEmpty = true;

	if (this->length > 0)
	{
		isEmpty = false;
	}
	
	return isEmpty;
}
//디폴트생성자 정의
template <typename T>
Stack<T>::Stack(Long capacity)//Create
	:objects(capacity)//Array디폴트생성자를 호출함
{
	//1. 할당량을 정한다.
	this->capacity = capacity;
	//2. 사용량을 정한다.
	this->length = 0;
	//3. 끝내다.
}
//복사생성자 정의
template <typename T>
Stack<T>::Stack(const Stack& source)
	:objects(source.objects)//Array의 복사생성자를 호출함
{
	this->capacity = source.capacity;
	this->length = source.length;
}
//소멸자 정의
template <typename T>
Stack<T>::~Stack()//Destroy
{
	//<주의>Stack 소멸자가 호출되면 그 멤버들도 같이 다 소멸된다.
	//Array<T> objects는 Stack의 멤버이기 때문에 같이 소멸되는데 이때 Array라이브러리의 소멸자가 호출되서
	//Array가 다 할당해제가 되기 때문에 Stack소멸자에는 아무 내용이 없어도 된다.
	//주소록(AddressBook)의 소멸자가 호출되면 그 멤버인 Array<Personal> personals가 소멸되면서
	//자동으로 Array라이브러리 소멸자가 호출되어서 persoanl들을 다 할당해제 해주는 것과 동일한 원리다!
}
//제일 마지막 배열요소 다음에 새로운 배열 요소 추가하고 그 위치를 출력하기
template <typename T>
Long Stack<T>::Push(T object)
{
	Long index;//출력할 위치를 담을 공간
	//1. 스택에 삽입할 내용을 입력받는다.
	//2. length가 capacity보다 작으면
	if (this->length < this->capacity)
	{
		index = this->objects.Store(this->length, object);
	}//3.length가 capacity보다 크거나 같으면
	else
	{
		index = this->objects.AppendFromRear(object);
		this->capacity++;
	}
	//4. 사용량을 증가시킨다.
	this->length++;
	//5. 저장한 위치를 출력한다.
	return index;
	//6. 끝내다.
}
//마지막 배열 요소를 삭제하고 반환하기
template <typename T>
T& Stack<T>::Pop()
{
	//<주의>여기서는 깊은 복사개념을 쓰면 안됨! this->objects.DeleteFromRear()를 호출하면 
	//배열에서 마지막 요소(T를 담는공간)을 할당해제하고, 개수가 1개 줄어든 배열을 만들어서 복사해서 옮김 
	//이때 T가 내용이면 할당해제가 됬기 때문에 상관이 없는데, T가 주소이면 T가 사라져도 T의 주소는 힙에
	//또 내용을 가리키고 있기 때문에 T주소를 담는 공간만 할당해제가 되었고, T주소가 가리키던 힙에 할당된 
	//내용은 힙에 그대로 있다. 그래서 T가 내용이냐 주소이냐에 따라 달라진다. T가 내용이면 별도로 할당해제할
	//필요가 없고, T가 주소이면 T의 주소를 담는 공간만 할당해제되기 때문에 T의 내용은 그대로 T에 있기 때문에
	//나중에 별도로 할당해제를 해줘야한다. 그래서 이 내용의 주소를 가지고 있는 T Object는 댕글링 포인터가
	//아니다. 그래서 새로 깊은 복사를 할필요가 없다. 마지막 배열요소를 받아서 그대로 반환하면 다시 쓸수있다.
	//예를 들어 T가 Command*이면 stack의 List는 command를 수행할 주소를 각 배열 요소마다 담고 있고, 
	//그 배열 요소들은 각각 힙에 할당된 command(내용)를 가지고 있다. 여기서 stack의 마지막 배열요소를 
	//GetAt으로 구하면 Command*(힙에 할당된 Command의 주소)를 반환받고, 그리고 DeleteFromRear를 하면
	//stack에서 이 command의 주소를 담고 있는 공간을 할당해제하지만 Command는 그대로 힙에 있다. 
	//그리고 그 주소는 아까 GetAt을 통해 구했으므로, 이 주소를 반환해서 CommandHistory의 UndoList나
	//RedoList에 끼운다. 그리고 필요할 때 그 저장된 주소를 통해 Command(힙에 할당된 내용)를 수행하고,
	//CommandHistory가 할당해제 될 때, undoList와 redoList(Command의 주소를 담는 배열)의 각 배열요소의
	//내용들을 할당해제 하므로 그 때 Command의 내용이 비로소 할당해제가 된다.
	//예를 들어 T가 AddressBook의 Personal(내용)이면 GetAt을 통해 마지막 배열요소를 구하면 Personal
	//(내용)이 반환되고, DeleteFromRear를 하면 힙에 할당된 Personal의 내용(마지막 배열요소)은 할당해제
	//된다. 그러나 아까 GetAt을 통해 Personal의 내용을 반환받았기 때문에 return을 하면 Personl(내용)이
	//반환된다.(Command*이면 Command*가 반환되듯이)
#if 0
	//마지막 배열 요소를 구한다.
	T lastObject = this->objects.GetAt(this->length - 1);
	//마지막 배열 요소를 복사한다.(깊은 복사)
	T (*newObject) = new T(lastObject);
	//마지막 배열 요소를 지운다.
	this->objects.DeleteFromRear();
	//할당량을 감소시킨다.
	this->capacity--;
	//사용량을 감소시킨다.
	this->length--;
	//복사한 마지막 배열 요소를 반환한다.
	return *newObject;
	//끝내다.
#endif
	T Object = 0;
	//스택에 object가 있으면
	if (this->length > 0)
	{
		//마지막 배열 요소를 구한다.
		Object = this->objects.GetAt(this->length - 1);
		//마지막 배열 요소를 지운다.
		this->objects.DeleteFromRear();
		//할당량을 감소시킨다.
		this->capacity--;
		//사용량을 감소시킨다.
		this->length--;
	}
	//복사한 마지막 배열 요소를 반환한다.
	return Object;
}
//제일 마지막 배열요소를 구하기
template <typename T>
T& Stack<T>::GetTop()
{
	T object = 0;
	//1. Stack에 Object가 있으면
	if (this->length > 0)
	{
		object = this->objects[this->length - 1];
	}
	//2. object를 반환한다.
	return object;
	//3. 끝내다.
}

//치환연산자 정의
template <typename T>
Stack<T>& Stack<T>::operator=(const Stack& source)
{
	Long i = 0;
	//1. 기존 this배열을 없앤다.
	//생성자가 아니기 때문에 기존의 데이터를 가지고 있음. 그래서 기존의 데이터를 먼저 할당해제한 후에
	//새로운 내용을 힙에 할당해야함.
	if (this->objects != 0)
	{
		delete[] this->objects;
	}
	//2. 새로운 this 배열을 만든다.
	this->objects = new T[source.capacity];
	//3. 할당량을 정한다.
	this->capacity = source.capacity;
	//4. 사용량을 정한다.
	this->length = 0;
	//5. source배열의 배열요소를 옮긴다.
	while (i < source.length)
	{
		this->objects[i] = source.objects[i];
		this->length++;
		i++;
	}
	//6. this를 출력한다.
	return *this;
	//7. 끝내다.
}
//첨자연산자 정의
template <typename T>
T& Stack<T>::operator[](Long index)
{
	return this->objects[index];
}
//포인터연산자 정의
template <typename T>
T* Stack<T>::operator+(Long index)
{
	return this->objects + index;
}

#endif // !_STACK_H
