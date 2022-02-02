#ifndef _ARRAY_H
#define _ARRAY_H

typedef signed long int Long;
template <typename T>
class Array
{
public:
	Array(Long capacity = 256);//������
	Array(const Array& source);//���������
	~Array();//�Ҹ���
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
	Array<T>& operator=(const Array& source);//ġȯ������
	T& operator[](Long index);//÷�ڿ�����
	T* operator+(Long index);//�����Ϳ�����
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
	//1. capacity�� �Է¹޴´�.
	//2. front�迭�� �����.
	this->front = new T[capacity];
	//3. �Ҵ緮�� ���Ѵ�.
	this->capacity = capacity;
	//4. ��뷮�� ���Ѵ�.
	this->length = 0;
	//5. ������.
}

template <typename T>
Array<T>::~Array()//Destroy
{
	//1. �迭�� �Ҵ������Ѵ�.
	if (this->front != 0)
	{
		delete[] this->front;
	}
	//2. ������.
}

template <typename T>
Long Array<T>::Store(Long index, T object)
{
	//1. ��ġ�� ����� �Է¹޴´�.
	//2. ����� ��ġ�� �����Ѵ�.
	this->front[index] = object;
	//3. ��뷮�� ������Ų��.
	this->length++;
	//4. ��ġ�� ����Ѵ�.
	return  index;
	//5. ������.
}

template <typename T>
T& Array<T>::GetAt(Long index)
{
	//1. ��ġ�� �Է¹޴´�.
	//2. value�� ����Ѵ�.
	return this->front[index];
	//3. ������.
}

template <typename T>
Long Array<T>::Insert(Long index, T object)
{
	T(*front);
	Long i = 0;
	Long j = 0;
	//1. ��ġ�� ����� �Է¹޴´�.
	//2. �� front �迭�� �����.
	front = new T[this->capacity + 1];
	//3. �о��.
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
	//4. ���� front �迭�� ���ش�.
	if (this->front != 0)
	{
		delete[] this->front;
	}
	//5. �����Ѵ�.
	this->front = front;
	this->capacity++;
	this->front[index] = object;
	this->length++;
	//6. ��ġ�� ����Ѵ�.
	return index;
	//7. ������.
}

template <typename T>
Long Array<T>::AppendFromFront(T object)
{
	Long index = 0;
	T(*front);
	Long i = 1;
	Long j = 0;
	//1. ����� �Է¹޴´�.
	//2. �� front �迭�� �����.
	front = new T[this->capacity + 1];
	//3. �о��.
	while (j < this->length)
	{
		front[i] = this->front[j];
		i++;
		j++;
	}
	//4. ���� front �迭�� ���ش�.
	if (this->front != 0)
	{
		delete[] this->front;
	}
	//5. �����Ѵ�.
	this->front = front;
	this->capacity++;
	this->front[index] = object;
	this->length++;
	//6. ��ġ�� ����Ѵ�.
	return index;
	//7. ������.
}

template <typename T>
Long Array<T>::AppendFromRear(T object)
{
	Long index;
	T(*front);
	Long i = 0;
	//1. ����� �Է¹޴´�.
	//2. �� front �迭�� �����.
	front = new T[this->capacity + 1];
	//3. ���� front�迭�� �Ű����´�.
	while (i < this->length)
	{
		front[i] = this->front[i];
		i++;
	}
	//4. ���� front�迭�� ���ش�.
	if (this->front != 0)
	{
		delete[] this->front;
	}
	//5. �����Ѵ�.
	this->front = front;
	this->capacity++;
	index = this->capacity - 1;
	this->front[index] = object;
	this->length++;
	//6. ��ġ�� ����Ѵ�.
	return index;
	//7. ������.
}

template <typename T>
Long Array<T>::Delete(Long index)
{
	T(*front)=0;
	Long i = 0;
	Long j = 0;
	//1. ��ġ�� �Է¹޴´�.
	//2. �Ҵ緮�� 1���� ũ��
	if (this->capacity > 1)
	{
		//2.1 �� front �迭�� �����.
		front = new T[this->capacity - 1];
	}
	//3. ���� front�迭�� ������ �Ű����´�.
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
	//4. ���� front �迭�� ���ش�.
	if (this->front != 0)
	{
		delete[] this->front;
		this->front = 0;
	}
	//5. �Ҵ緮�� 1���� ũ��
	if (this->capacity > 1)
	{
		//5.1 ��front�� �ּҸ� �����Ѵ�.
		this->front = front;
	}
	//6. �Ҵ緮�� ���ҽ�Ų��.
	this->capacity--;
	//7. ��뷮�� ���ҽ�Ų��.
	this->length--;
	//8. ��ġ�� ����Ѵ�.
	index = -1;
	return index;
	//9. ������.
}

template <typename T>
Long Array<T>::DeleteFromFront()
{
	Long index = 0;
	T(*front) = 0;
	Long i = 1;
	Long j = 0;
	//1. �Ҵ緮�� 1���� ũ��
	if (this->capacity > 1)
	{
		//1.1 ��front�迭�� �����.
		front = new T[this->capacity - 1];
	}
	//2. ���� front�迭�� ������ �Ű����´�.
	while (i < this->length)
	{
		front[j] = this->front[i];
		j++;
		i++;
	}
	//3. ���� front�迭�� ���ش�.
	if (this->front != 0)
	{
		delete[] this->front;
		this->front = 0;
	}
	//4. �Ҵ緮�� 1���� ũ��
	if (this->capacity > 1)
	{
		//4.1 ��front�� �ּҸ� �����Ѵ�.
		this->front = front;
	}
	//5. �Ҵ緮�� ���ҽ�Ų��.
	this->capacity--;
	//6. ��뷮�� ���ҽ�Ų��.
	this->length--;
	//7. ��ġ�� ����Ѵ�.
	index = -1;
	return index;
	//8. ������.
}

template <typename T>
Long Array<T>::DeleteFromRear()
{
	Long index = -1;
	T(*front) = 0;
	Long i = 0;

	//1. �Ҵ緮�� 1���� ũ��
	if (this->capacity > 1)
	{
		//1.1 ��front�迭�� �����.
		front = new T[this->capacity - 1];
	}
	//2. ���� front�迭�� ������ �Ű����´�.
	while (i < this->capacity - 1)
	{
		front[i] = this->front[i];
		i++;
	}
	//3. ���� front�迭�� ���ش�.
	if (this->front != 0)
	{
		delete[] this->front;
		this->front = 0;
	}
	//4. �Ҵ緮�� 1���� ũ��
	if (this->capacity > 1)
	{
		//4.1 ��front�� �ּҸ� �����Ѵ�.
		this->front = front;
	}
	//5. �Ҵ緮�� ���ҽ�Ų��.
	this->capacity--;
	//6. ��뷮�� ���ҽ�Ų��.
	this->length--;
	//7. ��ġ�� ����Ѵ�.
	return index;
	//8. ������.
}

template <typename T>
void Array<T>::Clear()
{
	//1. front�迭�� ���ش�.
	if (this->front != 0)
	{
		delete[]this->front;
		this->front = 0;
	}
	//2. �Ҵ緮�� 0���� ���Ѵ�.
	this->capacity = 0;
	//3. ��뷮�� 0���� ���Ѵ�.
	this->length = 0;
	//4. ������.
}

template <typename T>
Long Array<T> ::Modify(Long index, T object)
{
	//1. ��ġ�� ����� �Է¹޴´�.
	//2. ��ġ���� �����Ѵ�.
	this->front[index] = object;
	//3. ��ġ�� ����Ѵ�.
	return index;
	//4. ������.
}

template <typename T>
Long Array<T>::LinearSearchUnique(void* key, int(*compare)(void*, void*))
{
	Long index = -1;//��ġ�� 0���̽�
	Long i = 0;//0���̽�

	//1. ã���� �ϴ� ���� �Է¹޴´�.
	//2. ��뷮���� �������� �׸��� ã�� ���� �ٸ� ���� �ݺ��Ѵ�.
	while (i < this->length && compare(this->front + i, key) != 0)
	{
		//2.1 ��ġ�� ����.
		i++;
	}
	//3. ��뷮���� ������
	if (i < this->length)
	{
		//3.1 ��ġ�� �����Ѵ�.
		index = i;
	}
	//4. ��ġ�� ����Ѵ�.
	return index;
}

template <typename T>
void Array<T>::LinearSearchDuplicate(void* key, Long* (*indexes), Long* count, int(*compare)(void*, void*))
{
	Long i = 0;
	Long j = 0;

	//�ʱ�ȭ
	*indexes = 0;
	*count = 0;

	//1. ã���� �ϴ� ���� �Է¹޴´�.
	//2. ��뷮���� �������� �ݺ��Ѵ�.
	while (i < this->length)
	{
		//2.1 ã���� �ϴ� ���� ���� ������
		if (compare(this->front + i, key) == 0)
		{
			//2.1.1 ������ ����.
			(*count)++;
		}
		i++;
	}
	//3. ������ 0���� ũ��
	if (*count > 0)
	{
		//3.1 ��ġ�� �迭�� �����.
		*indexes = new Long[*count];
	}
	i = 0;//����ġ
	//4. ��뷮���� �������� �ݺ��Ѵ�.
	while (i < this->length)
	{
		//4.1 ã���� �ϴ� ���� ���� ������
		if (compare(this->front + i, key) == 0)
		{
			//4.1.1 ��ġ�� �����Ѵ�.
			(*indexes)[j] = i;
			j++;
		}
		i++;
	}
	//5. ��ġ�� �迭�� ������ ����Ѵ�.
	//6. ������.
}

template <typename T>
Long Array<T>::BinarySearchUnique(void* key, int(*compare)(void*, void*))
{
	Long index = -1;
	Long first = 0;
	Long middle;
	Long last;

	//1. ã���� �ϴ� ���� �Է¹޴´�.
	//2. ������ ���� ���Ѵ�.
	last = this->length - 1;
	//3. �߰����� ���Ѵ�.
	middle = (first + last) / 2;
	//4. ó���� ���������� �۰ų� �������� �׸��� �߰��� ã�� ���� �ٸ� ���� �ݺ��Ѵ�.
	while (first <= last && compare(this->front + middle, key) != 0)
	{
		//4.1  �߰��� ã�� ������ ������
		if (compare(this->front + middle, key) < 0)
		{
			//4.1.1 ù��°�� ������Ų��.
			first = middle + 1;
		}
		//4.2 �׷��� ������
		else
		{
			//4.2.1 �������� ���ҽ�Ų��.
			last = middle - 1;
		}
		//4.3 �߰��� ���Ѵ�.
		middle = (first + last) / 2;
	}
	//5. ù��°�� ���������� �۰ų� ������
	if (first <= last)
	{
		//5.1 ��ġ�� �����Ѵ�.
		index = middle;
	}
	//6. ��ġ�� ����Ѵ�.
	return index;
	//7. ������.
}

template <typename T>
void Array<T>::BinarySearchDuplicate(void* key, Long* (*indexes), Long* count, int(*compare)(void*, void*))
{
	Long first = 0;
	Long middle;
	Long last;
	Long i;
	Long j = 0;

	//�ʱ�ȭ
	*indexes = 0;
	*count = 0;

	//1. ã�� ���� �Է¹޴´�.
	//2. �������� ���Ѵ�.
	last = this->length - 1;
	//3. ��ġ�� �迭�� �����.
	*indexes = new Long[this->length];
	//4. �߰��� ���Ѵ�.
	middle = (first + last) / 2;
	//5. ó���� ���������� �۰ų� �������� �׸��� �߰��� ã�� ���� �ٸ� ���� �ݺ��Ѵ�.
	while (first <= last && compare(this->front + middle, key) != 0)
	{
		//5.1  �߰��� ã�� ������ ������
		if (compare(this->front + middle, key) < 0)
		{
			//5.1.1 ù��°�� ������Ų��.
			first = middle + 1;
		}
		//5.2 �׷��� ������
		else
		{
			//5.2.1 �������� ���ҽ�Ų��.
			last = middle - 1;
		}
		//5.3 �߰��� ���Ѵ�.
		middle = (first + last) / 2;
	}
	//6. �ߺ��� ������ ��ġ�� �����Ѵ�.
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
	//7. ��ġ�� �迭�� ������ ����Ѵ�.
	//8. ������.
}

template <typename T>
void Array<T>::SelectionSort(int(*compare)(void*, void*))
{
	T temp;
	Long i = 0;
	Long j;
	Long minimum;
	T lowest;

	//1. ������������ �����Ѵ�.
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
	//2. ������.
}

template <typename T>
void Array<T>::BubbleSort(int(*compare)(void*, void*))
{
	T temp;
	Long i;
	Long j;
	bool flag = true;

	//1. ������������ �����Ѵ�.
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
	//2. ������.
}

template <typename T>
void Array<T>::InsertionSort(int (*compare)(void*, void*))
{
	T temp;
	Long i = 1;
	Long j;
	//1. ������������ �����Ѵ�.
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
	//2. ������.
}

template <typename T>
void Array<T>::Merge(const Array& one, const Array& other, int(*compare)(void*, void*))
{
	Long i = 0;
	Long j = 0;
	Long k = 0;
	//1. one, other �迭�� �Է��Ѵ�.
	//2. this �迭�� ���ش�.
	if (this->front != 0)
	{
		delete[] this->front;
	}
	//3. this�迭�� ���� �Ҵ��Ѵ�.
	this->front = new T[one.length + other.length];
	this->capacity = one.length + other.length;
	this->length = 0;
	//4. length���� �������� �ݺ��Ѵ�.
	while (i < one.length && j < other.length)
	{
		//4.1 one�� �迭��Ұ� other�� �迭��Һ��� ũ��
		if (compare(one.front + i, other.front + j) > 0)
		{
			//4.1.1 other�� �迭��Ҹ� �����Ѵ�.
			this->front[k] = other.front[j];
			k++;
			j++;
		}
		//4.2 �׷��� ������
		else
		{
			//4.2.1 one�� �迭��Ҹ� �����Ѵ�.
			this->front[k] = one.front[i];
			k++;
			i++;
		}
	}
	//5. one�� length���� �������� �ݺ��Ѵ�.
	while (i < one.length)
	{
		//5.1 one�� �迭��Ҹ� �����Ѵ�.
		this->front[k] = one.front[i];
		k++;
		i++;
	}
	//6. other�� length���� �������� �ݺ��Ѵ�.
	while (j < other.length)
	{
		//6.1 other�� �迭��Ҹ� �����Ѵ�.
		this->front[k] = other.front[j];
		k++;
		j++;
	}
	//7. ��뷮�� ������Ų��.
	this->length = one.length + other.length;
	//8. ������.
}

//���������
template <typename T>
Array<T>::Array(const Array& source)
{
	Long i = 0;
	//1. ���ο� this �迭�� �����. �������̹Ƿ� ������ �����Ͱ� ���� ������ �ٷ� ���� ����� �ȴ�.
	this->front = new T[source.capacity];
	//2. �Ҵ緮�� ���Ѵ�.
	this->capacity = source.capacity;
	//3. ��뷮�� ���Ѵ�.
	this->length = 0;
	//4. source�迭�� �迭��Ҹ� �ű��.
	//source�迭�� �Ҵ������Ǵ��� ������ ���� this array�迭�� �����ϱ� ���ؼ� �迭��Ҹ� �ű�
	//this->front = source.front �� source.front�� �Ҵ�� ���� �ּҸ� �ܼ��� �����ѰŶ�
	//���࿡ source.front�� ���� �Ҵ������ϰԵǸ� this->front�� ��۸������Ͱ���.
	//�װ��� �����ϱ� ���� this.front�� ���� ���� �Ҵ��ϰ� source.front�� ������ this.front�� �ϳ��ϳ� �� �ű�.
	//�׷��ԵǸ� source.front�迭�� �Ҵ������ϴ��� this->front�� ������ �ڽ��� �迭�� ������ �ֱ⶧���� ������ ����.
	while (i < source.length)
	{
		this->front[i] = source.front[i];
		this->length++;
		i++;
	}
	//5. ������.
}

//ġȯ������
template <typename T>
Array<T>& Array<T>::operator=(const Array& source)
{
	Long i = 0;
	//1. ���� this�迭�� ���ش�.
	//�����ڰ� �ƴϱ� ������ ������ �����͸� ������ ����. �׷��� ������ �����͸� ���� �Ҵ������� �Ŀ�
	//���ο� ������ ���� �Ҵ��ؾ���.
	if (this->front != 0)
	{
		delete[] this->front;
	}
	//2. ���ο� this �迭�� �����.
	this->front = new T[source.capacity];
	//3. �Ҵ緮�� ���Ѵ�.
	this->capacity = source.capacity;
	//4. ��뷮�� ���Ѵ�.
	this->length = 0;
	//5. source�迭�� �迭��Ҹ� �ű��.
	while (i < source.length)
	{
		this->front[i] = source.front[i];
		this->length++;
		i++;
	}
	//6. this�� ����Ѵ�.
	return *this;
	//7. ������.
}

//÷�ڿ�����
template <typename T>
T& Array<T>::operator[](Long index)
{
	return this->front[index];
}

//�����Ϳ�����
template <typename T>
T* Array<T>::operator+(Long index)
{
	return this->front + index;
}

#endif // !_ARRAY_H


