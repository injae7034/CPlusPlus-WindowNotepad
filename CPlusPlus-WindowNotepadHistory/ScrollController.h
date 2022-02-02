#ifndef _SCROLLCONTROLLER_H
#define _SCROLLCONTROLLER_H

#include "Observer.h"
#include "NotepadForm.h"

class Scroll;

class ScrollController:public Observer
{
public:
	NotepadForm* notepadForm;
	Scroll* scroll[2];//Scroll포인터 배열
public:
	ScrollController(NotepadForm* notepadForm = 0);//디폴트생성자
	virtual void Update();
	~ScrollController();//소멸자
	//인라인함수
	CScrollBar* GetHorizontalScrollBar() const;
	CScrollBar* GetVerticalScrollBar() const;
	
private:
	CScrollBar* horizontalScrollBar;
	CScrollBar* verticalScrollBar;
};

//인라인함수 정의
inline CScrollBar* ScrollController::GetHorizontalScrollBar() const
{
	return const_cast<CScrollBar*>(this->horizontalScrollBar);
}
inline CScrollBar* ScrollController::GetVerticalScrollBar() const
{
	return const_cast<CScrollBar*>(this->verticalScrollBar);
}


#endif // !_SCROLLCONTROLLER_H
