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
#include "ShiftRightArrowKeyAction.h"
#include "ShiftCtrlRightArrowKeyAction.h"
#include "ShiftLeftArrowKeyAction.h"
#include "ShiftCtrlLeftArrowKeyAction.h"
#include "ShiftDownArrowKeyAction.h"
#include "ShiftUpArrowKeyAction.h"
#include "ShiftPageDownKeyAction.h"
#include "ShiftPageUpKeyAction.h"
#include "ShiftEndKeyAction.h"
#include "ShiftHomeKeyAction.h"
#include "ShiftCtrlEndKeyAction.h"
#include "ShiftCtrlHomeKeyAction.h"
#include "CtrlAKeyAction.h"
#include "ShiftCtrlBackSpaceKeyAction.h"
#include "ShiftCtrlDeleteKeyAction.h"
#include "CtrlBackSpaceKeyAction.h"
#include "CtrlDeleteKeyAction.h"
#include "CtrlZKeyAction.h"
#include "CtrlYKeyAction.h"
#include "CtrlXKeyAction.h"
#include "CtrlCKeyAction.h"
#include "CtrlVKeyAction.h"

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
	//2. Shift키가 눌러졌는지 안눌러졌는지 상태를 체크해서 저장한다.
	Long shiftPressedCheck = GetKeyState(VK_SHIFT);
	//Shift 키와 Ctrl 키가 눌려진 채로 실행되는 것부터 먼저 체크하자!!!
	//3. Shift키와 Ctrl키를 누른채 오른쪽 방향키를 눌렀으면(오른쪽 단어 단위 이동하면서 글자선택)
	if (shiftPressedCheck & 0x8000 && ctrlPressedCheck & 0x8000 && nChar == VK_RIGHT)
	{
		keyAction = new ShiftCtrlRightArrowKeyAction(this->notepadForm);
	}
	//4. Shift키와 Ctrl키를 누른채 왼쪽 방향키를 눌렀으면(왼쪽 단어 단위 이동하면서 글자선택)
	else if (shiftPressedCheck & 0x8000 && ctrlPressedCheck & 0x8000 && nChar == VK_LEFT)
	{
		keyAction = new ShiftCtrlLeftArrowKeyAction(this->notepadForm);
	}
	//5. Shift키와 Ctrl키를 누른채 Home키를 눌렀으면(첫 줄, 첫 글자로 이동하면서 글자선택)
	else if (shiftPressedCheck & 0x8000 && ctrlPressedCheck & 0x8000 && nChar == VK_HOME)
	{
		keyAction = new ShiftCtrlHomeKeyAction(this->notepadForm);
	}
	//6. Shift키와 Ctrl키를 누른채 End키를 눌렀으면(마지막 줄, 마지막 글자로 이동하면서 글자선택)
	else if (shiftPressedCheck & 0x8000 && ctrlPressedCheck & 0x8000 && nChar == VK_END)
	{
		keyAction = new ShiftCtrlEndKeyAction(this->notepadForm);
	}
	//Shift키와 Ctrl키를 누른 채 BackSpace키를 눌렀으면
	//(캐럿이 있는 곳에서 줄의 처음까지 글자들을 다 지워버림)
	else if (shiftPressedCheck & 0x8000 && ctrlPressedCheck & 0x8000 && nChar == VK_BACK)
	{
		keyAction = new ShiftCtrlBackSpaceKeyAction(this->notepadForm);
	}
	//Shift키와 Ctrl키를 누른 채 Delete키를 눌렀으면
	//(캐럿이 있는 곳에서 줄의 마지막까지 글자들을 다 지워버림)
	else if (shiftPressedCheck & 0x8000 && ctrlPressedCheck & 0x8000 && nChar == VK_DELETE)
	{
		keyAction = new ShiftCtrlDeleteKeyAction(this->notepadForm);
	}
	//7. Shift키를 누른 채 오른쪽 방향키를 눌렀으면(오른쪽으로 한 글자 이동하면서 글자선택)
	else if (shiftPressedCheck & 0x8000 && nChar == VK_RIGHT)
	{
		keyAction = new ShiftRightArrowKeyAction(this->notepadForm);
	}
	//8. Shift키를 누른 채 왼쪽 방향키를 눌렀으면(왼쪽으로 한 글자 이동하면서 글자선택)
	else if (shiftPressedCheck & 0x8000 && nChar == VK_LEFT)
	{
		keyAction = new ShiftLeftArrowKeyAction(this->notepadForm);
	}
	//9. Shift키를 누른 채 위 방향키를 눌렀으면(위로 한 줄 이동하면서 글자선택)
	else if (shiftPressedCheck & 0x8000 && nChar == VK_UP)
	{
		keyAction = new ShiftUpArrowKeyAction(this->notepadForm);
	}
	//10. Shift키를 누른 채 아래 방향키를 눌렀으면(아래로 한 줄 이동하면서 글자선택)
	else if (shiftPressedCheck & 0x8000 && nChar == VK_DOWN)
	{
		keyAction = new ShiftDownArrowKeyAction(this->notepadForm);
	}
	//11. Shift키를 누른 채 PageUp키를 눌렀으면(한 페이지 위로 이동하면서 글자선택)
	else if (shiftPressedCheck & 0x8000 && nChar == VK_PRIOR)
	{
		keyAction = new ShiftPageUpKeyAction(this->notepadForm);
	}
	//12. Shift키를 누른 채 PageDown키를 눌렀으면(한 페이지 아래로 이동하면서 글자선택)
	else if (shiftPressedCheck & 0x8000 && nChar == VK_NEXT)
	{
		keyAction = new ShiftPageDownKeyAction(this->notepadForm);
	}
	//13. Shift키를 누른 채 Home키를 눌렀으면(첫 글자로 이동하면서 글자선택)
	else if (shiftPressedCheck & 0x8000 && nChar == VK_HOME)
	{
		keyAction = new ShiftHomeKeyAction(this->notepadForm);
	}
	//14. Shift키를 누른 채 End키를 눌렀으면(마지막 글자로 이동하면서 글자선택)
	else if (shiftPressedCheck & 0x8000 && nChar == VK_END)
	{
		keyAction = new ShiftEndKeyAction(this->notepadForm);
	}
	//15. Ctrl키를 누른 채 오른쪽 방향키를 눌렀으면(오른쪽으로 단어 단위 이동)
	else if (ctrlPressedCheck & 0x8000 && nChar == VK_RIGHT)
	{
		keyAction = new CtrlRightArrowKeyAction(this->notepadForm);
	}
	//16. Ctrl키를 누른 채 왼쪽 방향키를 눌렀으면(왼쪽으로 단어 단위 이동)
	else if (ctrlPressedCheck & 0x8000 && nChar == VK_LEFT)
	{
		keyAction = new CtrlLeftArrowKeyAction(this->notepadForm);
	}
	//17. Ctrl키를 누른 채 Home키를 눌렀으면(첫 줄, 첫 글자로 이동)
	else if (ctrlPressedCheck & 0x8000 && nChar == VK_HOME)
	{
		keyAction = new CtrlHomeKeyAction(this->notepadForm);
	}
	//18. Ctrl키를 누른 채 End키를 눌렀으면(마지막 줄, 마지막 글자로 이동)
	else if (ctrlPressedCheck & 0x8000 && nChar == VK_END)
	{
		keyAction = new CtrlEndKeyAction(this->notepadForm);
	}
	// Ctrl키를 누른 채 알파벳 'a'를 누르면(메모장 전체 텍스트 선택)
	else if (ctrlPressedCheck & 0x8000 && nChar == 'a' ||
		ctrlPressedCheck & 0x8000 && nChar == 'A')
	{
		keyAction = new CtrlAKeyAction(this->notepadForm);
	}
	//Ctrl키를 누른 채 알파벳 'z'를 누르면(실행취소)
	else if (ctrlPressedCheck & 0x8000 && nChar == 'z' ||
		ctrlPressedCheck & 0x8000 && nChar == 'Z')
	{
		keyAction = new CtrlZKeyAction(this->notepadForm);
	}
	//Ctrl키를 누른 채 알파벳 'y'를 누르면(다시실행)
	else if (ctrlPressedCheck & 0x8000 && nChar == 'y' ||
	ctrlPressedCheck & 0x8000 && nChar == 'Y')
	{
	keyAction = new CtrlYKeyAction(this->notepadForm);
	}
	//Ctrl키를 누른 채 알파벳 'x'를 누르면(잘라내기)
	else if (ctrlPressedCheck & 0x8000 && nChar == 'x' ||
	ctrlPressedCheck & 0x8000 && nChar == 'X')
	{
	keyAction = new CtrlXKeyAction(this->notepadForm);
	}
	//Ctrl키를 누른 채 알파벳 'c'를 누르면(복사하기)
	else if (ctrlPressedCheck & 0x8000 && nChar == 'c' ||
	ctrlPressedCheck & 0x8000 && nChar == 'C')
	{
	keyAction = new CtrlCKeyAction(this->notepadForm);
	}
	//Ctrl키를 누른 채 알파벳 'v'를 누르면(붙여넣기)
	else if (ctrlPressedCheck & 0x8000 && nChar == 'v' ||
	ctrlPressedCheck & 0x8000 && nChar == 'V')
	{
	keyAction = new CtrlVKeyAction(this->notepadForm);
	}
	//Ctrl키를 누른 채 BackSpace키를 눌렀으면
	//(캐럿이 있는 곳에서 왼쪽으로 단어단위로 글자들을 지워버림)
	else if (ctrlPressedCheck & 0x8000 && nChar == VK_BACK)
	{
		keyAction = new CtrlBackSpaceKeyAction(this->notepadForm);
	}
	//Ctrl키를 누른 채 Delete키를 눌렀으면
	//(캐럿이 있는 곳에서 오른쪽으로 단어단위로 글자들을 지워버림)
	else if (ctrlPressedCheck & 0x8000 && nChar == VK_DELETE)
	{
		keyAction = new CtrlDeleteKeyAction(this->notepadForm);
	}
	//19. 오른쪽 방향키를 눌렀으면(오른쪽으로 한 글자 이동)
	else if (nChar == VK_RIGHT)
	{
		keyAction = new RightArrowKeyAction(this->notepadForm);
	}
	//20. 왼쪽 방향키를 눌렀으면(왼쪽으로 한 글자 이동)
	else if (nChar == VK_LEFT)
	{
		keyAction = new LeftArrowKeyAction(this->notepadForm);
	}
	//21. 위쪽 방향키를 눌렀으면(위쪽으로 한 줄 이동)
	else if (nChar == VK_UP)
	{
		keyAction = new UpArrowKeyAction(this->notepadForm);
	}
	//22. 아래쪽 방향키를 눌렀으면(아래쪽으로 한 줄 이동)
	else if (nChar == VK_DOWN)
	{
		keyAction = new DownArrowKeyAction(this->notepadForm);
	}
	//23. Home키를 눌렀으면(첫 글자로 이동)
	else if (nChar == VK_HOME)
	{
		keyAction = new HomeKeyAction(this->notepadForm);
	}
	//24. End키를 눌렀으면(마지막 글자로 이동)
	else if (nChar == VK_END)
	{
		keyAction = new EndKeyAction(this->notepadForm);
	}
	//25. PageUp키를 눌렀으면(한 페이지 위로 이동)
	else if (nChar == VK_PRIOR)
	{
		keyAction = new PageUpKeyAction(this->notepadForm);
	}
	//26. PageDown키를 눌렀으면(한 페이지 아래로 이동)
	else if (nChar == VK_NEXT)
	{
		keyAction = new PageDownKeyAction(this->notepadForm);
	}
	//27. BackSpace키를 눌렀으면(앞의 글자를 지움)
	else if (nChar == VK_BACK)
	{
		keyAction = new BackSpaceKeyAction(this->notepadForm);
	}
	//28. Delete키를 눌렀으면(뒤의 글자를 지움)
	else if (nChar == VK_DELETE)
	{
		keyAction = new DeleteKeyAction(this->notepadForm);
	}
	//29. keyAction을 출력한다.
	return keyAction;
}

//소멸자
KeyActionCreator::~KeyActionCreator()
{

}
