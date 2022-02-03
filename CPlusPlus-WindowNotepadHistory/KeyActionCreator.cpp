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
	//Ctrl�� ������ ä�� ����Ǵ� �ͺ��� ���� üũ����!!!
	//2. CtrlŰ�� ���� ä ������ ����Ű�� ��������
	if (ctrlPressedCheck & 0x8000 && nChar == VK_RIGHT)
	{
		keyAction = new CtrlRightArrowKeyAction(this->notepadForm);
	}
	//3. CtrlŰ�� ���� ä ���� ����Ű�� ��������
	else if (ctrlPressedCheck & 0x8000 && nChar == VK_LEFT)
	{
		keyAction = new CtrlLeftArrowKeyAction(this->notepadForm);
	}
	//4. CtrlŰ�� ���� ä HomeŰ�� ��������
	else if (ctrlPressedCheck & 0x8000 && nChar == VK_HOME)
	{
		keyAction = new CtrlHomeKeyAction(this->notepadForm);
	}
	//5. CtrlŰ�� ���� ä EndŰ�� ��������
	else if (ctrlPressedCheck & 0x8000 && nChar == VK_END)
	{
		keyAction = new CtrlEndKeyAction(this->notepadForm);
	}
	//6. ������ ����Ű�� ��������
	else if (nChar == VK_RIGHT)
	{
		keyAction = new RightArrowKeyAction(this->notepadForm);
	}
	//7. ���� ����Ű�� ��������
	else if (nChar == VK_LEFT)
	{
		keyAction = new LeftArrowKeyAction(this->notepadForm);
	}
	//8. ���� ����Ű�� ��������
	else if (nChar == VK_UP)
	{
		keyAction = new UpArrowKeyAction(this->notepadForm);
	}
	//9. �Ʒ��� ����Ű�� ��������
	else if (nChar == VK_DOWN)
	{
		keyAction = new DownArrowKeyAction(this->notepadForm);
	}
	//10. HomeŰ�� ��������
	else if (nChar == VK_HOME)
	{
		keyAction = new HomeKeyAction(this->notepadForm);
	}
	//11. EndŰ�� ��������
	else if (nChar == VK_END)
	{
		keyAction = new EndKeyAction(this->notepadForm);
	}
	//12. PageUpŰ�� ��������
	else if (nChar == VK_PRIOR)
	{
		keyAction = new PageUpKeyAction(this->notepadForm);
	}
	//13. PageDownŰ�� ��������
	else if (nChar == VK_NEXT)
	{
		keyAction = new PageDownKeyAction(this->notepadForm);
	}
	//14. BackSpaceŰ�� ��������
	else if (nChar == VK_BACK)
	{
		keyAction = new BackSpaceKeyAction(this->notepadForm);
	}
	//15. DeleteŰ�� ��������
	else if (nChar == VK_DELETE)
	{
		keyAction = new DeleteKeyAction(this->notepadForm);
	}
	//16. keyAction�� ����Ѵ�.
	return keyAction;
}

//�Ҹ���
KeyActionCreator::~KeyActionCreator()
{

}
