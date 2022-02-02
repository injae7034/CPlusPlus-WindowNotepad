#include "Subject.h"
#include "Observer.h"

//����Ʈ������
Subject::Subject(Long capacity)
	:observers(capacity)
{
	this->capacity = capacity;
	this->length = 0;
}

//�Ҹ���
//AccountBook�� accounts�� account���� �ּҸ� �����ϰ� �ִ� ���� �Ҵ�� �迭�� ��ó��
//Subject�� observers�� observer���� �ּҸ� �����ϰ� �ִ� ���� �Ҵ�� �迭�̴�.
//���� observer���� �ּҸ� �����ϰ� �ִ� ���� �Ҵ�� �迭�� ��������� ���� �� �ּҵ��� ������ �ִ�
//������� ������ �Ҵ������ϰ� �״����� ������ �� �ּҵ��� ������ �ִ� �迭�� �Ҵ�����������Ѵ�.
//�Ҹ��ڿ����� ���� length�� capacity�� ���ҽ����� �ʿ䰡 ����!
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

//���������
void Subject::Attach(Observer* observer)
{
	//1. observer��ũ�� �Է¹޴´�.
	//2. ��뷮�� �Ҵ緮���� ������
	if (this->length < this->capacity)
	{
		this->observers.Store(this->length, observer);
	}
	//3. �׷��� ������
	else
	{
		this->observers.AppendFromRear(observer);
		//3.1 �Ҵ緮�� ������Ų��.
		this->capacity++;
	}
	//4. ��뷮�� ������Ų��.
	this->length++;
}

//����������
void Subject::Detach(Observer* observer)
{
	//1. �Է¹��� �������� �ּҷ� ��������ũ�迭�� ��ġ�� ���Ѵ�.
	Long index = this->observers.LinearSearchUnique(observer, CompareObserver);
	//2. �������� �ּҸ� �̿��� ���� �Ҵ�� �������� ������ �Ҵ������Ѵ�.
	if (observer != 0)
	{
		delete observer;
	}
	//3. ��������ũ�迭�� �迭���(�ּ�)�� �����.
	this->observers.Delete(index);
	//4. �Ҵ緮�� ���ҽ�Ų��.
	this->capacity--;
	//5. ��뷮�� ���ҽ�Ų��.
	this->length--;
}

//�������˸�
void Subject::Notify()
{
	//1. length��ŭ �ݺ��Ѵ�.
	Long i = 0;
	while (i < this->length)
	{
		//1.1 Observer��Ͽ� �x�� �������鿡�� Update�϶�� �޼����� ����!
		this->observers.GetAt(i)->Update();
		i++;
	}
}
#if 0
//Search
Observer* Subject::Search(size_t key)
{
	//1. observer�ּҹ迭���� ���ϴ� �ڷ������� ũ�⸦ ã�� ������ �ݺ��Ѵ�.
	Long i = 0;
	Observer* observer = 0;
	observer = this->observers.GetAt(i);
	//2. i�� length���� ���� ���� �׸��� �������� ĳ���Ŵ����� �ƴѵ��� �ݺ��Ѵ�.
	while (i < this->length && sizeof(*observer) != key)
	{
		//2.1 ������ ����Ʈ���� �������� ���Ѵ�.
		observer = this->observers.GetAt(i);
		//2.2 i�� ���ҽ�Ų��.
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
//�Լ������� ����
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

