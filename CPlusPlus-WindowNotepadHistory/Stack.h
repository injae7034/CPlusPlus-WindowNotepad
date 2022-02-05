#ifndef _STACK_H
#define _STACK_H
//���� ���̺귯���� Array���̺귯���� �����ϱ�!
#include "Array.h"
typedef signed long int Long;
template <typename T>
class Stack
{
public:
	Stack(Long capacity = 256);//����Ʈ ������(���� �Ű����� �Է��� ������ �⺻���� 256���� �Ҵ���!)
	Stack(const Stack& source);//���������
	~Stack();//�Ҹ���
	Long Push(T object);//���� ������ �迭����� ������ �����ϰ� �� ��ġ�� ��ȯ��
	T& Pop();//������ �迭 ��Ҹ� �����ϰ� ��ȯ��(������O)
	T& GetTop();//���ÿ��� ���� ������ ��ġ�� ��ȯ��(������X)
	Stack<T>& operator=(const Stack& source);//ġȯ������
	T& operator[](Long index);//÷�ڿ�����
	T* operator+(Long index);//�����Ϳ�����
	//�ζ����Լ�
	Long GetCapacity() const;
	Long GetLength() const;
	bool IsEmpty() const;
private:
	Array<T> objects;
	Long capacity;
	Long length;
};
//�ζ����Լ� ����
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
//����Ʈ������ ����
template <typename T>
Stack<T>::Stack(Long capacity)//Create
	:objects(capacity)//Array����Ʈ�����ڸ� ȣ����
{
	//1. �Ҵ緮�� ���Ѵ�.
	this->capacity = capacity;
	//2. ��뷮�� ���Ѵ�.
	this->length = 0;
	//3. ������.
}
//��������� ����
template <typename T>
Stack<T>::Stack(const Stack& source)
	:objects(source.objects)//Array�� ��������ڸ� ȣ����
{
	this->capacity = source.capacity;
	this->length = source.length;
}
//�Ҹ��� ����
template <typename T>
Stack<T>::~Stack()//Destroy
{
	//<����>Stack �Ҹ��ڰ� ȣ��Ǹ� �� ����鵵 ���� �� �Ҹ�ȴ�.
	//Array<T> objects�� Stack�� ����̱� ������ ���� �Ҹ�Ǵµ� �̶� Array���̺귯���� �Ҹ��ڰ� ȣ��Ǽ�
	//Array�� �� �Ҵ������� �Ǳ� ������ Stack�Ҹ��ڿ��� �ƹ� ������ ��� �ȴ�.
	//�ּҷ�(AddressBook)�� �Ҹ��ڰ� ȣ��Ǹ� �� ����� Array<Personal> personals�� �Ҹ�Ǹ鼭
	//�ڵ����� Array���̺귯�� �Ҹ��ڰ� ȣ��Ǿ persoanl���� �� �Ҵ����� ���ִ� �Ͱ� ������ ������!
}
//���� ������ �迭��� ������ ���ο� �迭 ��� �߰��ϰ� �� ��ġ�� ����ϱ�
template <typename T>
Long Stack<T>::Push(T object)
{
	Long index;//����� ��ġ�� ���� ����
	//1. ���ÿ� ������ ������ �Է¹޴´�.
	//2. length�� capacity���� ������
	if (this->length < this->capacity)
	{
		index = this->objects.Store(this->length, object);
	}//3.length�� capacity���� ũ�ų� ������
	else
	{
		index = this->objects.AppendFromRear(object);
		this->capacity++;
	}
	//4. ��뷮�� ������Ų��.
	this->length++;
	//5. ������ ��ġ�� ����Ѵ�.
	return index;
	//6. ������.
}
//������ �迭 ��Ҹ� �����ϰ� ��ȯ�ϱ�
template <typename T>
T& Stack<T>::Pop()
{
	//<����>���⼭�� ���� ���簳���� ���� �ȵ�! this->objects.DeleteFromRear()�� ȣ���ϸ� 
	//�迭���� ������ ���(T�� ��°���)�� �Ҵ������ϰ�, ������ 1�� �پ�� �迭�� ���� �����ؼ� �ű� 
	//�̶� T�� �����̸� �Ҵ������� ��� ������ ����� ���µ�, T�� �ּ��̸� T�� ������� T�� �ּҴ� ����
	//�� ������ ����Ű�� �ֱ� ������ T�ּҸ� ��� ������ �Ҵ������� �Ǿ���, T�ּҰ� ����Ű�� ���� �Ҵ�� 
	//������ ���� �״�� �ִ�. �׷��� T�� �����̳� �ּ��̳Ŀ� ���� �޶�����. T�� �����̸� ������ �Ҵ�������
	//�ʿ䰡 ����, T�� �ּ��̸� T�� �ּҸ� ��� ������ �Ҵ������Ǳ� ������ T�� ������ �״�� T�� �ֱ� ������
	//���߿� ������ �Ҵ������� ������Ѵ�. �׷��� �� ������ �ּҸ� ������ �ִ� T Object�� ��۸� �����Ͱ�
	//�ƴϴ�. �׷��� ���� ���� ���縦 ���ʿ䰡 ����. ������ �迭��Ҹ� �޾Ƽ� �״�� ��ȯ�ϸ� �ٽ� �����ִ�.
	//���� ��� T�� Command*�̸� stack�� List�� command�� ������ �ּҸ� �� �迭 ��Ҹ��� ��� �ְ�, 
	//�� �迭 ��ҵ��� ���� ���� �Ҵ�� command(����)�� ������ �ִ�. ���⼭ stack�� ������ �迭��Ҹ� 
	//GetAt���� ���ϸ� Command*(���� �Ҵ�� Command�� �ּ�)�� ��ȯ�ް�, �׸��� DeleteFromRear�� �ϸ�
	//stack���� �� command�� �ּҸ� ��� �ִ� ������ �Ҵ����������� Command�� �״�� ���� �ִ�. 
	//�׸��� �� �ּҴ� �Ʊ� GetAt�� ���� �������Ƿ�, �� �ּҸ� ��ȯ�ؼ� CommandHistory�� UndoList��
	//RedoList�� �����. �׸��� �ʿ��� �� �� ����� �ּҸ� ���� Command(���� �Ҵ�� ����)�� �����ϰ�,
	//CommandHistory�� �Ҵ����� �� ��, undoList�� redoList(Command�� �ּҸ� ��� �迭)�� �� �迭�����
	//������� �Ҵ����� �ϹǷ� �� �� Command�� ������ ��μ� �Ҵ������� �ȴ�.
	//���� ��� T�� AddressBook�� Personal(����)�̸� GetAt�� ���� ������ �迭��Ҹ� ���ϸ� Personal
	//(����)�� ��ȯ�ǰ�, DeleteFromRear�� �ϸ� ���� �Ҵ�� Personal�� ����(������ �迭���)�� �Ҵ�����
	//�ȴ�. �׷��� �Ʊ� GetAt�� ���� Personal�� ������ ��ȯ�޾ұ� ������ return�� �ϸ� Personl(����)��
	//��ȯ�ȴ�.(Command*�̸� Command*�� ��ȯ�ǵ���)
#if 0
	//������ �迭 ��Ҹ� ���Ѵ�.
	T lastObject = this->objects.GetAt(this->length - 1);
	//������ �迭 ��Ҹ� �����Ѵ�.(���� ����)
	T (*newObject) = new T(lastObject);
	//������ �迭 ��Ҹ� �����.
	this->objects.DeleteFromRear();
	//�Ҵ緮�� ���ҽ�Ų��.
	this->capacity--;
	//��뷮�� ���ҽ�Ų��.
	this->length--;
	//������ ������ �迭 ��Ҹ� ��ȯ�Ѵ�.
	return *newObject;
	//������.
#endif
	T Object = 0;
	//���ÿ� object�� ������
	if (this->length > 0)
	{
		//������ �迭 ��Ҹ� ���Ѵ�.
		Object = this->objects.GetAt(this->length - 1);
		//������ �迭 ��Ҹ� �����.
		this->objects.DeleteFromRear();
		//�Ҵ緮�� ���ҽ�Ų��.
		this->capacity--;
		//��뷮�� ���ҽ�Ų��.
		this->length--;
	}
	//������ ������ �迭 ��Ҹ� ��ȯ�Ѵ�.
	return Object;
}
//���� ������ �迭��Ҹ� ���ϱ�
template <typename T>
T& Stack<T>::GetTop()
{
	T object = 0;
	//1. Stack�� Object�� ������
	if (this->length > 0)
	{
		object = this->objects[this->length - 1];
	}
	//2. object�� ��ȯ�Ѵ�.
	return object;
	//3. ������.
}

//ġȯ������ ����
template <typename T>
Stack<T>& Stack<T>::operator=(const Stack& source)
{
	Long i = 0;
	//1. ���� this�迭�� ���ش�.
	//�����ڰ� �ƴϱ� ������ ������ �����͸� ������ ����. �׷��� ������ �����͸� ���� �Ҵ������� �Ŀ�
	//���ο� ������ ���� �Ҵ��ؾ���.
	if (this->objects != 0)
	{
		delete[] this->objects;
	}
	//2. ���ο� this �迭�� �����.
	this->objects = new T[source.capacity];
	//3. �Ҵ緮�� ���Ѵ�.
	this->capacity = source.capacity;
	//4. ��뷮�� ���Ѵ�.
	this->length = 0;
	//5. source�迭�� �迭��Ҹ� �ű��.
	while (i < source.length)
	{
		this->objects[i] = source.objects[i];
		this->length++;
		i++;
	}
	//6. this�� ����Ѵ�.
	return *this;
	//7. ������.
}
//÷�ڿ����� ����
template <typename T>
T& Stack<T>::operator[](Long index)
{
	return this->objects[index];
}
//�����Ϳ����� ����
template <typename T>
T* Stack<T>::operator+(Long index)
{
	return this->objects + index;
}

#endif // !_STACK_H
