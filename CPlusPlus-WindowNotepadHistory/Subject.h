#ifndef _SUBJECT_H
#define _SUBJECT_H
#include "Array.h"

typedef signed long int Long;
class Observer;

class Subject {
public:
	Subject(Long capacity = 8);//����Ʈ������ �⺻���� ����Ʈ������ �ֱ�(8,16,...)
	virtual ~Subject() = 0;//�߻�Ŭ���� �Ҹ���(���������Լ�)(����X, ��ӹ޴� Ŭ�������� ������ �ʼ�)
	//��ӹ޴� Ŭ����(notepadForm)�� �����Ǹ� ���ϱ� ������ �տ� virtual�� �Ⱥ���.
	void Attach(Observer* observer);//���������
	void Detach(Observer* observer);//����������
	void Notify();//���������� ����Ǿ��ٰ� �˸�
	Long Insert(Long index, Observer* observer);//�������� index�� �����ֱ�
	Observer* GetAt(Long index);
	Long Search(Observer* key);//ã�����ϴ� �������� �ּҸ� ������ �� �迭��ġ�� ��ȯ����.
	//�ζ����Լ� ����
	Long GetCapacity() const;
	Long GetLength() const;
protected://��ӹ޴� Ŭ�������� ����� �� �ֵ��� protected�� �ٲ�
	Array<Observer*> observers;//���������� �ּҸ� �����ϰ� �ִ� �迭
	Long capacity;
	Long length;
};

//�ζ����Լ� ����
inline Long Subject::GetCapacity() const
{
	return this->capacity;
}
inline Long Subject::GetLength() const
{
	return this->length;
}

//�Լ������� ����
int CompareObserver(void* one, void* other);

#endif // !_SUBJECT_H

