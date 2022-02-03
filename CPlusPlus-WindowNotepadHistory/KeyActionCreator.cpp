#include "KeyActionCreator.h"
#include "RightArrowKeyAction.h"
#include "CtrlRightArrowKeyAction.h"
#include "LeftArrowKeyAction.h"
#include "CtrlLeftArrowKeyAction.h"
#include "UpArrowKeyAction.h"
#include "DownArrowKeyAction.h"
#include "HomeKeyAction.h"
#include "CtrlHomeKeyAction.h"
#include "EndKeyAction.h"
#include "CtrlEndKeyAction.h"
#include "KeyAction.h"
#include "PageUpKeyAction.h"
#include "PageDownKeyAction.h"
#include "BackSpaceKeyAction.h"
#include "DeleteKeyAction.h"

//디폴트생성자
KeyActionCreator::KeyActionCreator(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
}

//팩토리메소드 패턴
KeyAction* KeyActionCreator::Create(UINT nChar)
{
	KeyAction* keyAction = 0;
	//1. Ctrl키가 눌러졌는지 안눌러졌는지 상태를 체크해서 저장한다.
	Long ctrlPressedCheck = GetKeyState(VK_CONTROL);
	//Ctrl이 눌려진 채로 실행되는 것부터 먼저 체크하자!!!
	//2. Ctrl키를 누른 채 오른쪽 방향키를 눌렀으면
	if (ctrlPressedCheck & 0x8000 && nChar == VK_RIGHT)
	{
		keyAction = new CtrlRightArrowKeyAction(this->notepadForm);
	}
	//3. Ctrl키를 누른 채 왼쪽 방향키를 눌렀으면
	else if (ctrlPressedCheck & 0x8000 && nChar == VK_LEFT)
	{
		keyAction = new CtrlLeftArrowKeyAction(this->notepadForm);
	}
	//4. Ctrl키를 누른 채 Home키를 눌렀으면
	else if (ctrlPressedCheck & 0x8000 && nChar == VK_HOME)
	{
		keyAction = new CtrlHomeKeyAction(this->notepadForm);
	}
	//5. Ctrl키를 누른 채 End키를 눌렀으면
	else if (ctrlPressedCheck & 0x8000 && nChar == VK_END)
	{
		keyAction = new CtrlEndKeyAction(this->notepadForm);
	}
	//6. 오른쪽 방향키를 눌렀으면
	else if (nChar == VK_RIGHT)
	{
		keyAction = new RightArrowKeyAction(this->notepadForm);
	}
	//7. 왼쪽 방향키를 눌렀으면
	else if (nChar == VK_LEFT)
	{
		keyAction = new LeftArrowKeyAction(this->notepadForm);
	}
	//8. 위쪽 방향키를 눌렀으면
	else if (nChar == VK_UP)
	{
		keyAction = new UpArrowKeyAction(this->notepadForm);
	}
	//9. 아래쪽 방향키를 눌렀으면
	else if (nChar == VK_DOWN)
	{
		keyAction = new DownArrowKeyAction(this->notepadForm);
	}
	//10. Home키를 눌렀으면
	else if (nChar == VK_HOME)
	{
		keyAction = new HomeKeyAction(this->notepadForm);
	}
	//11. End키를 눌렀으면
	else if (nChar == VK_END)
	{
		keyAction = new EndKeyAction(this->notepadForm);
	}
	//12. PageUp키를 눌렀으면
	else if (nChar == VK_PRIOR)
	{
		keyAction = new PageUpKeyAction(this->notepadForm);
	}
	//13. PageDown키를 눌렀으면
	else if (nChar == VK_NEXT)
	{
		keyAction = new PageDownKeyAction(this->notepadForm);
	}
	//14. BackSpace키를 눌렀으면
	else if (nChar == VK_BACK)
	{
		keyAction = new BackSpaceKeyAction(this->notepadForm);
	}
	//15. Delete키를 눌렀으면
	else if (nChar == VK_DELETE)
	{
		keyAction = new DeleteKeyAction(this->notepadForm);
	}
	//16. keyAction을 출력한다.
	return keyAction;
}

//소멸자
KeyActionCreator::~KeyActionCreator()
{

}
