#include "ScrollActionCreator.h"
#include "ScrollAction.h"
#include "FirstScrollAction.h"
#include "LastScrollAction.h"
#include "PreviousScrollAction.h"
#include "NextScrollAction.h"
#include "PagePreviousScrollAction.h"
#include "PageNextScrollAction.h"
#include "ThumbPositionScrollAction.h"

//����Ʈ������
ScrollActionCreator::ScrollActionCreator(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
}

//���丮�޼ҵ�����
ScrollAction* ScrollActionCreator::Create(UINT nSBCode)
{
	ScrollAction* scrollAction = 0;
	//1. ��ũ���� ���� ��(SB_TOP)�� �����̰ų� ���� ����(SB_LEFT)���� �����̸�
	//SB_TOP�� SB_LEFT�� ���� 6���� ���� ����.
	if (nSBCode == 6)
	{
		scrollAction = new FirstScrollAction(this->notepadForm);
	}
	//2. ��ũ���� ���� �Ʒ�(SB_BOTTOM)�� �����̰ų� ���� ������(SB_RIGHT)���� �����̸�
	//SB_BOTTOM�� SB_RIGHT�� ���� 7�� ���� ����.
	else if (nSBCode == 7)
	{
		scrollAction = new LastScrollAction(this->notepadForm);
	}
	//3. ��ũ���� �� �� ��(SB_LINEUP)�� �ø��ų� �� ĭ ����(SB_LINELEFT)���� �����̸�
	//SB_LINEUP�� SB_LINELEFT�� ���� 0���� ���� ����.
	else if (nSBCode == 0)
	{
		scrollAction = new PreviousScrollAction(this->notepadForm);
	}
	//4. ��ũ���� �� �� �Ʒ�(SB_LINEDOWN)�� �����ų� �� ĭ ������(SB_LINERIGHT)���� �����̸�
	//SB_LINEDOWN�� SB_LINERIGHT�� ���� 1�� ���� ����.
	else if (nSBCode == 1)
	{
		scrollAction = new NextScrollAction(this->notepadForm);
	}
	//5. ��ũ���� �� ������ ��(SB_PAGEUP)�� �ø��ų� �� ������ ����(SB_PAGELEFT)���� �����̸�
	//SB_PAGEUP�� SB_PAGELEFT�� ���� 2�� ���� ����.
	else if (nSBCode == 2)
	{
		scrollAction = new PagePreviousScrollAction(this->notepadForm);
	}
	//6. ��ũ���� �� ������ �Ʒ�(SB_PAGEDOWN)�� �����ų� �� ������ ������(SB_PAGERIGHT)���� �����̸�
	//SB_PAGEDOWN�� SB_PAGERIGHT�� ���� 3���� ���� ����.
	else if (nSBCode == 3)
	{
		scrollAction = new PageNextScrollAction(this->notepadForm);
	}
	//7. ��ũ���� ���� ���콺�� Ŭ���� ��ġ�� �����̸�
	//SB_THUMBPOSITION�� ���� �ȵ�! �׷� ��ũ�� �̵��� �� ������ ���ϴ�!
	else if (nSBCode == SB_THUMBTRACK)
	{
		scrollAction = new ThumbPositionScrollAction(this->notepadForm);
	}

	return scrollAction;
}

//�Ҹ���
ScrollActionCreator::~ScrollActionCreator()
{

}