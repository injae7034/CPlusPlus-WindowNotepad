#include "ScrollAction.h"

//디폴트생성자
ScrollAction::ScrollAction(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
}

//전략패턴
void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

}

void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

}

//소멸자
ScrollAction::~ScrollAction()
{

}