#ifndef _HORIZONTALSCROLL_H
#define _HORIZONTALSCROLL_H

#include "Scroll.h"
class ScrollController;

class HorizontalScroll :public Scroll
{
public:
	HorizontalScroll(ScrollController* scrollController = 0);//����Ʈ������
	HorizontalScroll(ScrollController* scrollController, Long position, Long min, Long max,
		Long pageSize);//�Ű������� 5�� ������ ������;
	HorizontalScroll(const HorizontalScroll& source);//���������
	HorizontalScroll& operator=(const HorizontalScroll& source);//ġȯ������
	virtual Long LinePrevious();
	virtual Long LineNext();
	~HorizontalScroll();//�Ҹ���
};

#endif // !_HORIZONTALSCROLL_H
