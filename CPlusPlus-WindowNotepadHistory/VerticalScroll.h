#ifndef  _VERTICALSCROLL_H
#define _VERTICALSCROLL_H

#include "Scroll.h"
#include "NotepadForm.h"

class ScrollController;

class VerticalScroll :public Scroll
{
public:
	VerticalScroll(ScrollController* scrollController = 0);//디폴트생성자
	VerticalScroll(ScrollController* scrollController, Long currentPos, Long min, Long max,
		Long pageSize);//매개변수를 5개 가지는 생성자
	VerticalScroll(const VerticalScroll& source);//복사생성자
	VerticalScroll& operator=(const VerticalScroll& source);//치환연산자
	virtual Long LinePrevious();
	virtual Long LineNext();
	virtual Long PagePrevious();
	virtual Long PageNext();
	~VerticalScroll();//소멸자
};

#endif // ! _VERTICALSCROLL_H
