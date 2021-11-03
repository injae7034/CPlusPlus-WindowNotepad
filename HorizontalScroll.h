#ifndef _HORIZONTALSCROLL_H
#define _HORIZONTALSCROLL_H

#include "Scroll.h"
class ScrollController;

class HorizontalScroll :public Scroll
{
public:
	HorizontalScroll(ScrollController* scrollController = 0);//디폴트생성자
	HorizontalScroll(ScrollController* scrollController, Long position, Long min, Long max,
		Long pageSize);//매개변수를 5개 가지는 생성자;
	HorizontalScroll(const HorizontalScroll& source);//복사생성자
	HorizontalScroll& operator=(const HorizontalScroll& source);//치환연산자
	virtual Long LinePrevious();
	virtual Long LineNext();
	~HorizontalScroll();//소멸자
};

#endif // !_HORIZONTALSCROLL_H
