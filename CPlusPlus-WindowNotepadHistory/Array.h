#ifndef _ARRAY_H
#define _ARRAY_H

typedef signed long int Long;
template <typename T>
class Array
{
public:
	Array(Long capacity = 256);//생성자
	Array(const Array& source);//복사생성자
	~Array();//소멸자
	T& GetAt(Long index);
	Long Store(Long index, T object);
	Long Insert(Long index, T object);
	Long AppendFromFront(T object);
	Long AppendFromRear(T object);
	Long Delete(Long index);
	Long DeleteFromFront();
	Long DeleteFromRear();
	void Clear();
	Long Modify(Long index, T object);
	Long LinearSearchUnique(void* key, int(*compare)(void*, void*));
	void LinearSearchDuplicate(void* key, Long* (*indexes), Long* count, int(*compare)(void*, void*));
	Long BinarySearchUnique(void* key, int(*compare)(void*, void*));
	void BinarySearchDuplicate(void* key, Long* (*indexes), Long* count, int(*compare)(void*, void*));
	void SelectionSort(int(*compare)(void*, void*));
	void BubbleSort(int(*compare)(void*, void*));
	void InsertionSort(int(*compare)(void*, void*));
	void Merge(const Array& one, const Array& other, int(*compare)(void*, void*));
	Array<T>& operator=(const Array& source);//치환연산자
	T& operator[](Long index);//첨자연산자
	T* operator+(Long index);//포인터연산자
	Long GetCapacity() const;
	Long GetLength() const;

private:
	T(*front);
	Long capacity;
	Long length;
};

template <typename T>
inline Long Array<T>::GetCapacity() const
{
	return this->capacity;
}

template <typename T>
inline Long Array<T>::GetLength() const
{
	return this->length;
}

template <typename T>
Array<T>::Array(Long capacity)//Create
{
	//1. capacity를 입력받는다.
	//2. front배열을 만든다.
	this->front = new T[capacity];
	//3. 할당량을 정한다.
	this->capacity = capacity;
	//4. 사용량을 정한다.
	this->length = 0;
	//5. 끝내다.
}

template <typename T>
Array<T>::~Array()//Destroy
{
	//1. 배열을 할당해제한다.
	if (this->front != 0)
	{
		delete[] this->front;
	}
	//2. 끝내다.
}

template <typename T>
Long Array<T>::Store(Long index, T object)
{
	//1. 위치와 대상을 입력받는다.
	//2. 대상을 위치에 저장한다.
	this->front[index] = object;
	//3. 사용량을 증가시킨다.
	this->length++;
	//4. 위치를 출력한다.
	return  index;
	//5. 끝내다.
}

template <typename T>
T& Array<T>::GetAt(Long index)
{
	//1. 위치를 입력받는다.
	//2. value를 출력한다.
	return this->front[index];
	//3. 끝내다.
}

template <typename T>
Long Array<T>::Insert(Long index, T object)
{
	T(*front);
	Long i = 0;
	Long j = 0;
	//1. 위치와 대상을 입력받는다.
	//2. 새 front 배열을 만든다.
	front = new T[this->capacity + 1];
	//3. 밀어낸다.
	while (i < index)
	{
		front[j] = this->front[i];
		j++;
		i++;
	}
	j++;
	while (i < this->length)
	{
		front[j] = this->front[i];
		j++;
		i++;
	}
	//4. 기존 front 배열을 없앤다.
	if (this->front != 0)
	{
		delete[] this->front;
	}
	//5. 삽입한다.
	this->front = front;
	this->capacity++;
	this->front[index] = object;
	this->length++;
	//6. 위치를 출력한다.
	return index;
	//7. 끝내다.
}

template <typename T>
Long Array<T>::AppendFromFront(T object)
{
	Long index = 0;
	T(*front);
	Long i = 1;
	Long j = 0;
	//1. 대상을 입력받는다.
	//2. 새 front 배열을 만든다.
	front = new T[this->capacity + 1];
	//3. 밀어낸다.
	while (j < this->length)
	{
		front[i] = this->front[j];
		i++;
		j++;
	}
	//4. 기존 front 배열을 없앤다.
	if (this->front != 0)
	{
		delete[] this->front;
	}
	//5. 삽입한다.
	this->front = front;
	this->capacity++;
	this->front[index] = object;
	this->length++;
	//6. 위치를 출력한다.
	return index;
	//7. 끝내다.
}

template <typename T>
Long Array<T>::AppendFromRear(T object)
{
	Long index;
	T(*front);
	Long i = 0;
	//1. 대상을 입력받는다.
	//2. 새 front 배열을 만든다.
	front = new T[this->capacity + 1];
	//3. 기존 front배열을 옮겨적는다.
	while (i < this->length)
	{
		front[i] = this->front[i];
		i++;
	}
	//4. 기존 front배열을 없앤다.
	if (this->front != 0)
	{
		delete[] this->front;
	}
	//5. 삽입한다.
	this->front = front;
	this->capacity++;
	index = this->capacity - 1;
	this->front[index] = object;
	this->length++;
	//6. 위치를 출력한다.
	return index;
	//7. 끝내다.
}

template <typename T>
Long Array<T>::Delete(Long index)
{
	T(*front)=0;
	Long i = 0;
	Long j = 0;
	//1. 위치를 입력받는다.
	//2. 할당량이 1보다 크면
	if (this->capacity > 1)
	{
		//2.1 새 front 배열을 만든다.
		front = new T[this->capacity - 1];
	}
	//3. 기존 front배열의 내용을 옮겨적는다.
	while (i < index)
	{
		front[j] = this->front[i];
		j++;
		i++;
	}
	i = index + 1;
	while (i < this->length)
	{
		front[j] = this->front[i];
		j++;
		i++;
	}
	//4. 기존 front 배열을 없앤다.
	if (this->front != 0)
	{
		delete[] this->front;
		this->front = 0;
	}
	//5. 할당량이 1보다 크면
	if (this->capacity > 1)
	{
		//5.1 새front의 주소를 저장한다.
		this->front = front;
	}
	//6. 할당량을 감소시킨다.
	this->capacity--;
	//7. 사용량을 감소시킨다.
	this->length--;
	//8. 위치를 출력한다.
	index = -1;
	return index;
	//9. 끝내다.
}

template <typename T>
Long Array<T>::DeleteFromFront()
{
	Long index = 0;
	T(*front) = 0;
	Long i = 1;
	Long j = 0;
	//1. 할당량이 1보다 크면
	if (this->capacity > 1)
	{
		//1.1 새front배열을 만든다.
		front = new T[this->capacity - 1];
	}
	//2. 기존 front배열의 내용을 옮겨적는다.
	while (i < this->length)
	{
		front[j] = this->front[i];
		j++;
		i++;
	}
	//3. 기존 front배열을 없앤다.
	if (this->front != 0)
	{
		delete[] this->front;
		this->front = 0;
	}
	//4. 할당량이 1보다 크면
	if (this->capacity > 1)
	{
		//4.1 새front의 주소를 저장한다.
		this->front = front;
	}
	//5. 할당량을 감소시킨다.
	this->capacity--;
	//6. 사용량을 감소시킨다.
	this->length--;
	//7. 위치를 출력한다.
	index = -1;
	return index;
	//8. 끝내다.
}

template <typename T>
Long Array<T>::DeleteFromRear()
{
	Long index = -1;
	T(*front) = 0;
	Long i = 0;

	//1. 할당량이 1보다 크면
	if (this->capacity > 1)
	{
		//1.1 새front배열을 만든다.
		front = new T[this->capacity - 1];
	}
	//2. 기존 front배열의 내용을 옮겨적는다.
	while (i < this->capacity - 1)
	{
		front[i] = this->front[i];
		i++;
	}
	//3. 기존 front배열을 없앤다.
	if (this->front != 0)
	{
		delete[] this->front;
		this->front = 0;
	}
	//4. 할당량이 1보다 크면
	if (this->capacity > 1)
	{
		//4.1 새front의 주소를 저장한다.
		this->front = front;
	}
	//5. 할당량을 감소시킨다.
	this->capacity--;
	//6. 사용량을 감소시킨다.
	this->length--;
	//7. 위치를 출력한다.
	return index;
	//8. 끝내다.
}

template <typename T>
void Array<T>::Clear()
{
	//1. front배열을 없앤다.
	if (this->front != 0)
	{
		delete[]this->front;
		this->front = 0;
	}
	//2. 할당량을 0으로 정한다.
	this->capacity = 0;
	//3. 사용량을 0으로 정한다.
	this->length = 0;
	//4. 끝내다.
}

template <typename T>
Long Array<T> ::Modify(Long index, T object)
{
	//1. 위치와 대상을 입력받는다.
	//2. 위치에서 수정한다.
	this->front[index] = object;
	//3. 위치를 출력한다.
	return index;
	//4. 끝내다.
}

template <typename T>
Long Array<T>::LinearSearchUnique(void* key, int(*compare)(void*, void*))
{
	Long index = -1;//위치는 0베이스
	Long i = 0;//0베이스

	//1. 찾고자 하는 값을 입력받는다.
	//2. 사용량보다 작은동안 그리고 찾는 값과 다를 동안 반복한다.
	while (i < this->length && compare(this->front + i, key) != 0)
	{
		//2.1 위치를 센다.
		i++;
	}
	//3. 사용량보다 작으면
	if (i < this->length)
	{
		//3.1 위치를 저장한다.
		index = i;
	}
	//4. 위치를 출력한다.
	return index;
}

template <typename T>
void Array<T>::LinearSearchDuplicate(void* key, Long* (*indexes), Long* count, int(*compare)(void*, void*))
{
	Long i = 0;
	Long j = 0;

	//초기화
	*indexes = 0;
	*count = 0;

	//1. 찾고자 하는 값을 입력받는다.
	//2. 사용량보다 작은동안 반복한다.
	while (i < this->length)
	{
		//2.1 찾고자 하는 값과 서로 같으면
		if (compare(this->front + i, key) == 0)
		{
			//2.1.1 개수를 센다.
			(*count)++;
		}
		i++;
	}
	//3. 개수가 0보다 크면
	if (*count > 0)
	{
		//3.1 위치들 배열을 만든다.
		*indexes = new Long[*count];
	}
	i = 0;//원위치
	//4. 사용량보다 작은동안 반복한다.
	while (i < this->length)
	{
		//4.1 찾고자 하는 값과 서로 같으면
		if (compare(this->front + i, key) == 0)
		{
			//4.1.1 위치를 저장한다.
			(*indexes)[j] = i;
			j++;
		}
		i++;
	}
	//5. 위치들 배열과 개수를 출력한다.
	//6. 끝내다.
}

template <typename T>
Long Array<T>::BinarySearchUnique(void* key, int(*compare)(void*, void*))
{
	Long index = -1;
	Long first = 0;
	Long middle;
	Long last;

	//1. 찾고자 하는 값을 입력받는다.
	//2. 마지막 값을 구한다.
	last = this->length - 1;
	//3. 중간값을 구한다.
	middle = (first + last) / 2;
	//4. 처음이 마지막보다 작거나 같은동안 그리고 중간과 찾는 값이 다를 동안 반복한다.
	while (first <= last && compare(this->front + middle, key) != 0)
	{
		//4.1  중간이 찾는 값보다 작으면
		if (compare(this->front + middle, key) < 0)
		{
			//4.1.1 첫번째를 증가시킨다.
			first = middle + 1;
		}
		//4.2 그렇지 않으면
		else
		{
			//4.2.1 마지막을 감소시킨다.
			last = middle - 1;
		}
		//4.3 중간을 구한다.
		middle = (first + last) / 2;
	}
	//5. 첫번째가 마지막보다 작거나 같으면
	if (first <= last)
	{
		//5.1 위치를 저장한다.
		index = middle;
	}
	//6. 위치를 출력한다.
	return index;
	//7. 끝내다.
}

template <typename T>
void Array<T>::BinarySearchDuplicate(void* key, Long* (*indexes), Long* count, int(*compare)(void*, void*))
{
	Long first = 0;
	Long middle;
	Long last;
	Long i;
	Long j = 0;

	//초기화
	*indexes = 0;
	*count = 0;

	//1. 찾는 값을 입력받는다.
	//2. 마지막을 구한다.
	last = this->length - 1;
	//3. 위치들 배열을 만든다.
	*indexes = new Long[this->length];
	//4. 중간을 구한다.
	middle = (first + last) / 2;
	//5. 처음이 마지막보다 작거나 같은동안 그리고 중간과 찾는 값이 다를 동안 반복한다.
	while (first <= last && compare(this->front + middle, key) != 0)
	{
		//5.1  중간이 찾는 값보다 작으면
		if (compare(this->front + middle, key) < 0)
		{
			//5.1.1 첫번째를 증가시킨다.
			first = middle + 1;
		}
		//5.2 그렇지 않으면
		else
		{
			//5.2.1 마지막을 감소시킨다.
			last = middle - 1;
		}
		//5.3 중간을 구한다.
		middle = (first + last) / 2;
	}
	//6. 중복된 수들의 위치를 저장한다.
	i = middle - 1;
	while (i >= 0 && compare(this->front + i, key) == 0)
	{
		i--;
	}
	i++;
	while (i < this->length && compare(this->front + i, key) == 0)
	{
		(*indexes)[j] = i;
		j++;
		(*count)++;
		i++;
	}
	//7. 위치들 배열과 개수를 출력한다.
	//8. 끝내다.
}

template <typename T>
void Array<T>::SelectionSort(int(*compare)(void*, void*))
{
	T temp;
	Long i = 0;
	Long j;
	Long minimum;
	T lowest;

	//1. 오름차순으로 정렬한다.
	while (i < this->length - 1)
	{
		j = i + 1;
		lowest = this->front[i];
		minimum = i;
		while (j < this->length)
		{
			if (compare(&lowest, this->front + j) > 0)
			{
				lowest = this->front[j];
				minimum = j;
			}
			j++;
		}
		if (minimum != i)
		{
			temp = this->front[i];
			this->front[i] = this->front[minimum];
			this->front[minimum] = temp;
		}
		i++;
	}
	//2. 끝내다.
}

template <typename T>
void Array<T>::BubbleSort(int(*compare)(void*, void*))
{
	T temp;
	Long i;
	Long j;
	bool flag = true;

	//1. 오름차순으로 정렬한다.
	i = this->length - 2;
	while (i >= 0 && flag == true)
	{
		flag = false;
		j = 0;
		while (j <= i)
		{
			if (compare(this->front + j, this->front + (j + 1)) > 0)
			{
				temp = this->front[j];
				this->front[j] = this->front[j + 1];
				this->front[j + 1] = temp;
				flag = true;
			}
			j++;
		}
		i--;
	}
	//2. 끝내다.
}

template <typename T>
void Array<T>::InsertionSort(int (*compare)(void*, void*))
{
	T temp;
	Long i = 1;
	Long j;
	//1. 오름차순으로 정렬한다.
	while (i < this->length)
	{
		temp = this->front[i];
		j = i - 1;
		while (j >= 0 && compare(this->front + j, &temp) > 0)
		{
			this->front[j + 1] = this->front[j];
			j--;
		}
		this->front[j + 1] = temp;
		i++;
	}
	//2. 끝내다.
}

template <typename T>
void Array<T>::Merge(const Array& one, const Array& other, int(*compare)(void*, void*))
{
	Long i = 0;
	Long j = 0;
	Long k = 0;
	//1. one, other 배열을 입력한다.
	//2. this 배열을 없앤다.
	if (this->front != 0)
	{
		delete[] this->front;
	}
	//3. this배열을 새로 할당한다.
	this->front = new T[one.length + other.length];
	this->capacity = one.length + other.length;
	this->length = 0;
	//4. length보다 작은동안 반복한다.
	while (i < one.length && j < other.length)
	{
		//4.1 one의 배열요소가 other의 배열요소보다 크면
		if (compare(one.front + i, other.front + j) > 0)
		{
			//4.1.1 other의 배열요소를 저장한다.
			this->front[k] = other.front[j];
			k++;
			j++;
		}
		//4.2 그렇지 않으면
		else
		{
			//4.2.1 one의 배열요소를 저장한다.
			this->front[k] = one.front[i];
			k++;
			i++;
		}
	}
	//5. one의 length보다 작은동안 반복한다.
	while (i < one.length)
	{
		//5.1 one의 배열요소를 저장한다.
		this->front[k] = one.front[i];
		k++;
		i++;
	}
	//6. other의 length보다 작은동안 반복한다.
	while (j < other.length)
	{
		//6.1 other의 배열요소를 저장한다.
		this->front[k] = other.front[j];
		k++;
		j++;
	}
	//7. 사용량을 증가시킨다.
	this->length = one.length + other.length;
	//8. 끝내다.
}

//복사생성자
template <typename T>
Array<T>::Array(const Array& source)
{
	Long i = 0;
	//1. 새로운 this 배열을 만든다. 생성자이므로 기존의 데이터가 없기 때문에 바로 새로 만들면 된다.
	this->front = new T[source.capacity];
	//2. 할당량을 정한다.
	this->capacity = source.capacity;
	//3. 사용량을 정한다.
	this->length = 0;
	//4. source배열의 배열요소를 옮긴다.
	//source배열이 할당해제되더라도 별개로 힙에 this array배열을 저장하기 위해서 배열요소를 옮김
	//this->front = source.front 는 source.front에 할당된 힙의 주소를 단순히 복사한거라서
	//만약에 source.front의 힙을 할당해제하게되면 this->front는 댕글링포인터가됨.
	//그것을 방지하기 위해 this.front를 힙에 새로 할당하고 source.front의 내용을 this.front에 하나하나 다 옮김.
	//그렇게되면 source.front배열을 할당해제하더라도 this->front는 별개로 자신의 배열을 가지고 있기때문에 문제가 없음.
	while (i < source.length)
	{
		this->front[i] = source.front[i];
		this->length++;
		i++;
	}
	//5. 끝내다.
}

//치환연산자
template <typename T>
Array<T>& Array<T>::operator=(const Array& source)
{
	Long i = 0;
	//1. 기존 this배열을 없앤다.
	//생성자가 아니기 때문에 기존의 데이터를 가지고 있음. 그래서 기존의 데이터를 먼저 할당해제한 후에
	//새로운 내용을 힙에 할당해야함.
	if (this->front != 0)
	{
		delete[] this->front;
	}
	//2. 새로운 this 배열을 만든다.
	this->front = new T[source.capacity];
	//3. 할당량을 정한다.
	this->capacity = source.capacity;
	//4. 사용량을 정한다.
	this->length = 0;
	//5. source배열의 배열요소를 옮긴다.
	while (i < source.length)
	{
		this->front[i] = source.front[i];
		this->length++;
		i++;
	}
	//6. this를 출력한다.
	return *this;
	//7. 끝내다.
}

//첨자연산자
template <typename T>
T& Array<T>::operator[](Long index)
{
	return this->front[index];
}

//포인터연산자
template <typename T>
T* Array<T>::operator+(Long index)
{
	return this->front + index;
}

#endif // !_ARRAY_H


