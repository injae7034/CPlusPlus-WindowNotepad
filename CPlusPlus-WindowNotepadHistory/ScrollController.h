#ifndef _SCROLLCONTROLLER_H
#define _SCROLLCONTROLLER_H

#include "Observer.h"
#include "NotepadForm.h"

class Scroll;

class ScrollController:public Observer
{
public:
	NotepadForm* notepadForm;
	Scroll* scroll[2];//Scroll������ �迭
public:
	ScrollController(NotepadForm* notepadForm = 0);//����Ʈ������
	virtual void Update();
	~ScrollController();//�Ҹ���
	//�ζ����Լ�
	CScrollBar* GetHorizontalScrollBar() const;
	CScrollBar* GetVerticalScrollBar() const;
	
private:
	CScrollBar* horizontalScrollBar;
	CScrollBar* verticalScrollBar;
};

//�ζ����Լ� ����
inline CScrollBar* ScrollController::GetHorizontalScrollBar() const
{
	return const_cast<CScrollBar*>(this->horizontalScrollBar);
}
inline CScrollBar* ScrollController::GetVerticalScrollBar() const
{
	return const_cast<CScrollBar*>(this->verticalScrollBar);
}


#endif // !_SCROLLCONTROLLER_H
