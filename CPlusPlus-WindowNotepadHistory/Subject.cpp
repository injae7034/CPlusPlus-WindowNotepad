#include "Subject.h"
#include "Observer.h"
#include "CaretController.h"

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
Long Subject::Attach(Observer* observer)
{
	Long index;
	//1. observer��ũ�� �Է¹޴´�.
	//2. ��뷮�� �Ҵ緮���� ������
	if (this->length < this->capacity)
	{
		index = this->observers.Store(this->length, observer);
	}
	//3. �׷��� ������
	else
	{
		index = this->observers.AppendFromRear(observer);
		//3.1 �Ҵ緮�� ������Ų��.
		this->capacity++;
	}
	//4. ��뷮�� ������Ų��.
	this->length++;
	//5. ��ġ�� ��ȯ�Ѵ�.
	return index;
}

Long Subject::Attach(Long index, Observer* observer)
{
	//1. ���� ���� ��ġ�� observer��ũ�� �Է¹޴´�.
	//2. observer��ũ�� index��°�� ���� �ִ´�.
	index = this->observers.Insert(index, observer);
	//3. �Ҵ緮�� ������Ų��.
	this->capacity++;
	//4. ��뷮�� ������Ų��.
	this->length++;
	//5. ���� ������ ��ġ�� ����Ѵ�.
	return index;
}

//����������
void Subject::Detach(Observer* observer)
{
	//1. �Է¹��� �������� �ּҷ� ��������ũ�迭�� ��ġ�� ���Ѵ�.
	Long index = this->observers.LinearSearchUnique(observer, CompareObserver);
	//2. �������� �ּҸ� �̿��� ���� �Ҵ�� �������� ������ �Ҵ������Ѵ�.
	if (index != -1)
	{
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

//Search
Long Subject::Search(Observer* key)
{
	//1. �Է¹��� �������� �ּҷ� ��������ũ�迭�� ��ġ�� ���Ѵ�.
	Long index = this->observers.LinearSearchUnique(key, CompareObserver);
	//2. ��ġ�� ����Ѵ�.
	return index;
#if 0
	//1. ã���� �ϴ� �������� �Ű������� �Է¹޴´�.
	Long index = 0;
	Observer* observer = 0;
	//2. index�� length���� ���� ���� �׸��� ã���� �ϴ� �������� �ƴѵ��� �ݺ��Ѵ�.
	while (index < this->length && dynamic_cast<Observer*>(observer) != key)
	{
		//2.1 ������ ����Ʈ���� �������� ���Ѵ�.
		observer = this->observers.GetAt(index);
		//2.2 index�� ������Ų��.
		index++;
	}
	//3. ã���� �ϴ� �������� ã������
	if (dynamic_cast<Observer*>(observer) == key)
	{
		//3.1 index�� ���ҽ����ش�.
		index--;
	}
	//4. ã���� �ϴ� �������� ��ã������
	else
	{
		index = -1;
	}
	//5. ã�� ��ġ�� ��ȯ�Ѵ�.
	return index;
#endif
}

//�޸𸮸��� �׷��� ��Ȯ�ϰ� ������
//Obwerver*�� Observer**�� ������ �ٸ���!! �� ���̸� �˱� ���ؼ� �޸𸮸��� �׷��� ���غ���!
//�Լ������� ����
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

