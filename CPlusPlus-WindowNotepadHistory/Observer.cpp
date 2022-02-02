#include "Observer.h"

//디폴트생성자
Observer::Observer(Subject* subject)
{
	this->subject = subject;
}

//Update
void Observer::Update()
{

}

//소멸자
Observer::~Observer()
{

}