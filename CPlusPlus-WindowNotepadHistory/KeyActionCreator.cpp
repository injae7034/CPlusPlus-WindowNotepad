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
#include "KeyActions/BackSpaceKeyAction.h"
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

//����Ʈ������
KeyActionCreator::KeyActionCreator(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
}

//���丮�޼ҵ� ����
KeyAction* KeyActionCreator::Create(UINT nChar)
{
	KeyAction* keyAction = 0;
	//1. CtrlŰ�� ���������� �ȴ��������� ���¸� üũ�ؼ� �����Ѵ�.
	Long ctrlPressedCheck = GetKeyState(VK_CONTROL);
	//2. ShiftŰ�� ���������� �ȴ��������� ���¸� üũ�ؼ� �����Ѵ�.
	Long shiftPressedCheck = GetKeyState(VK_SHIFT);
	//Shift Ű�� Ctrl Ű�� ������ ä�� ����Ǵ� �ͺ��� ���� üũ����!!!
	//3. ShiftŰ�� CtrlŰ�� ����ä ������ ����Ű�� ��������(������ �ܾ� ���� �̵��ϸ鼭 ���ڼ���)
	if (shiftPressedCheck & 0x8000 && ctrlPressedCheck & 0x8000 && nChar == VK_RIGHT)
	{
		keyAction = new ShiftCtrlRightArrowKeyAction(this->notepadForm);
	}
	//4. ShiftŰ�� CtrlŰ�� ����ä ���� ����Ű�� ��������(���� �ܾ� ���� �̵��ϸ鼭 ���ڼ���)
	else if (shiftPressedCheck & 0x8000 && ctrlPressedCheck & 0x8000 && nChar == VK_LEFT)
	{
		keyAction = new ShiftCtrlLeftArrowKeyAction(this->notepadForm);
	}
	//5. ShiftŰ�� CtrlŰ�� ����ä HomeŰ�� ��������(ù ��, ù ���ڷ� �̵��ϸ鼭 ���ڼ���)
	else if (shiftPressedCheck & 0x8000 && ctrlPressedCheck & 0x8000 && nChar == VK_HOME)
	{
		keyAction = new ShiftCtrlHomeKeyAction(this->notepadForm);
	}
	//6. ShiftŰ�� CtrlŰ�� ����ä EndŰ�� ��������(������ ��, ������ ���ڷ� �̵��ϸ鼭 ���ڼ���)
	else if (shiftPressedCheck & 0x8000 && ctrlPressedCheck & 0x8000 && nChar == VK_END)
	{
		keyAction = new ShiftCtrlEndKeyAction(this->notepadForm);
	}
	//7. ShiftŰ�� ���� ä ������ ����Ű�� ��������(���������� �� ���� �̵��ϸ鼭 ���ڼ���)
	else if (shiftPressedCheck & 0x8000 && nChar == VK_RIGHT)
	{
		keyAction = new ShiftRightArrowKeyAction(this->notepadForm);
	}
	//8. ShiftŰ�� ���� ä ���� ����Ű�� ��������(�������� �� ���� �̵��ϸ鼭 ���ڼ���)
	else if (shiftPressedCheck & 0x8000 && nChar == VK_LEFT)
	{
		keyAction = new ShiftLeftArrowKeyAction(this->notepadForm);
	}
	//9. ShiftŰ�� ���� ä �� ����Ű�� ��������(���� �� �� �̵��ϸ鼭 ���ڼ���)
	else if (shiftPressedCheck & 0x8000 && nChar == VK_UP)
	{
		keyAction = new ShiftUpArrowKeyAction(this->notepadForm);
	}
	//10. ShiftŰ�� ���� ä �Ʒ� ����Ű�� ��������(�Ʒ��� �� �� �̵��ϸ鼭 ���ڼ���)
	else if (shiftPressedCheck & 0x8000 && nChar == VK_DOWN)
	{
		keyAction = new ShiftDownArrowKeyAction(this->notepadForm);
	}
	//11. ShiftŰ�� ���� ä PageUpŰ�� ��������(�� ������ ���� �̵��ϸ鼭 ���ڼ���)
	else if (shiftPressedCheck & 0x8000 && nChar == VK_PRIOR)
	{
		keyAction = new ShiftPageUpKeyAction(this->notepadForm);
	}
	//12. ShiftŰ�� ���� ä PageDownŰ�� ��������(�� ������ �Ʒ��� �̵��ϸ鼭 ���ڼ���)
	else if (shiftPressedCheck & 0x8000 && nChar == VK_NEXT)
	{
		keyAction = new ShiftPageDownKeyAction(this->notepadForm);
	}
	//13. ShiftŰ�� ���� ä HomeŰ�� ��������(ù ���ڷ� �̵��ϸ鼭 ���ڼ���)
	else if (shiftPressedCheck & 0x8000 && nChar == VK_HOME)
	{
		keyAction = new ShiftHomeKeyAction(this->notepadForm);
	}
	//14. ShiftŰ�� ���� ä EndŰ�� ��������(������ ���ڷ� �̵��ϸ鼭 ���ڼ���)
	else if (shiftPressedCheck & 0x8000 && nChar == VK_END)
	{
		keyAction = new ShiftEndKeyAction(this->notepadForm);
	}
	//15. CtrlŰ�� ���� ä ������ ����Ű�� ��������(���������� �ܾ� ���� �̵�)
	else if (ctrlPressedCheck & 0x8000 && nChar == VK_RIGHT)
	{
		keyAction = new CtrlRightArrowKeyAction(this->notepadForm);
	}
	//16. CtrlŰ�� ���� ä ���� ����Ű�� ��������(�������� �ܾ� ���� �̵�)
	else if (ctrlPressedCheck & 0x8000 && nChar == VK_LEFT)
	{
		keyAction = new CtrlLeftArrowKeyAction(this->notepadForm);
	}
	//17. CtrlŰ�� ���� ä HomeŰ�� ��������(ù ��, ù ���ڷ� �̵�)
	else if (ctrlPressedCheck & 0x8000 && nChar == VK_HOME)
	{
		keyAction = new CtrlHomeKeyAction(this->notepadForm);
	}
	//18. CtrlŰ�� ���� ä EndŰ�� ��������(������ ��, ������ ���ڷ� �̵�)
	else if (ctrlPressedCheck & 0x8000 && nChar == VK_END)
	{
		keyAction = new CtrlEndKeyAction(this->notepadForm);
	}
	// CtrlŰ�� ���� ä ���ĺ� 'a'�� ������(�޸��� ��ü �ؽ�Ʈ ����)
	else if (ctrlPressedCheck & 0x8000 && nChar == 'a' ||
		ctrlPressedCheck & 0x8000 && nChar == 'A')
	{
		keyAction = new CtrlAKeyAction(this->notepadForm);
	}
	//19. ������ ����Ű�� ��������(���������� �� ���� �̵�)
	else if (nChar == VK_RIGHT)
	{
		keyAction = new RightArrowKeyAction(this->notepadForm);
	}
	//20. ���� ����Ű�� ��������(�������� �� ���� �̵�)
	else if (nChar == VK_LEFT)
	{
		keyAction = new LeftArrowKeyAction(this->notepadForm);
	}
	//21. ���� ����Ű�� ��������(�������� �� �� �̵�)
	else if (nChar == VK_UP)
	{
		keyAction = new UpArrowKeyAction(this->notepadForm);
	}
	//22. �Ʒ��� ����Ű�� ��������(�Ʒ������� �� �� �̵�)
	else if (nChar == VK_DOWN)
	{
		keyAction = new DownArrowKeyAction(this->notepadForm);
	}
	//23. HomeŰ�� ��������(ù ���ڷ� �̵�)
	else if (nChar == VK_HOME)
	{
		keyAction = new HomeKeyAction(this->notepadForm);
	}
	//24. EndŰ�� ��������(������ ���ڷ� �̵�)
	else if (nChar == VK_END)
	{
		keyAction = new EndKeyAction(this->notepadForm);
	}
	//25. PageUpŰ�� ��������(�� ������ ���� �̵�)
	else if (nChar == VK_PRIOR)
	{
		keyAction = new PageUpKeyAction(this->notepadForm);
	}
	//26. PageDownŰ�� ��������(�� ������ �Ʒ��� �̵�)
	else if (nChar == VK_NEXT)
	{
		keyAction = new PageDownKeyAction(this->notepadForm);
	}
	//27. BackSpaceŰ�� ��������(���� ���ڸ� ����)
	else if (nChar == VK_BACK)
	{
		keyAction = new BackSpaceKeyAction(this->notepadForm);
	}
	//28. DeleteŰ�� ��������(���� ���ڸ� ����)
	else if (nChar == VK_DELETE)
	{
		keyAction = new DeleteKeyAction(this->notepadForm);
	}
	//29. keyAction�� ����Ѵ�.
	return keyAction;
}

//�Ҹ���
KeyActionCreator::~KeyActionCreator()
{

}
