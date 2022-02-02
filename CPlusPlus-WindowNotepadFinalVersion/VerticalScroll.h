#ifndef  _VERTICALSCROLL_H
#define _VERTICALSCROLL_H

#include "Scroll.h"
#include "NotepadForm.h"

class ScrollController;

class VerticalScroll :public Scroll
{
public:
	VerticalScroll(ScrollController* scrollController = 0);//����Ʈ������
	VerticalScroll(ScrollController* scrollController, Long currentPos, Long min, Long max,
		Long pageSize);//�Ű������� 5�� ������ ������
	VerticalScroll(const VerticalScroll& source);//���������
	VerticalScroll& operator=(const VerticalScroll& source);//ġȯ������
	virtual Long LinePrevious();
	virtual Long LineNext();
	virtual Long PagePrevious();
	virtual Long PageNext();
	~VerticalScroll();//�Ҹ���
};

#endif // ! _VERTICALSCROLL_H
