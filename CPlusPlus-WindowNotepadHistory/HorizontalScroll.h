#ifndef _HORIZONTALSCROLL_H
#define _HORIZONTALSCROLL_H

#include "Scroll.h"
#include "NotepadForm.h"

class HorizontalScroll :public Scroll
{
public:
	HorizontalScroll();//����Ʈ������
	HorizontalScroll(ScrollController* scrollController, Long position, Long min, Long max,
		Long pageSize);//�Ű������� 5�� ������ ������;
	HorizontalScroll(const HorizontalScroll& source);//���������
	HorizontalScroll& operator=(const HorizontalScroll& source);//ġȯ������
	~HorizontalScroll();//�Ҹ���
};

#endif // !_HORIZONTALSCROLL_H
