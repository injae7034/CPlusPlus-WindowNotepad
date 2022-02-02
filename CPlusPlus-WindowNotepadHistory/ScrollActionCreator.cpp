#include "ScrollActionCreator.h"
#include "ScrollAction.h"
#include "FirstScrollAction.h"
#include "LastScrollAction.h"
#include "PreviousScrollAction.h"
#include "NextScrollAction.h"

//nSBCode == SB_LINEUP || nSBCode == SB_LINELEFT �� ���  SB_LINEPREVIOUS�� �����Ѵ�.
//#define SB_LINEPREVIOUS 0

//����Ʈ������
ScrollActionCreator::ScrollActionCreator(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
}

//���丮�޼ҵ�����
ScrollAction* ScrollActionCreator::Create(UINT nSBCode)
{
	ScrollAction* scrollAction = 0;
	//if(nSBCode == SB_LINEPREVIOUS)
	//1. ��ũ���� ���� ���� �����̰ų� ���� �������� �����̸�
	if (nSBCode == SB_TOP || nSBCode == SB_LEFT)
	{
		scrollAction = new FirstScrollAction(this->notepadForm);
	}
	//2. ��ũ���� ���� �Ʒ��� �����̰ų� ���� ���������� �����̸�
	else if (nSBCode == SB_BOTTOM || nSBCode == SB_RIGHT)
	{
		scrollAction = new LastScrollAction(this->notepadForm);
	}
	//3. ��ũ���� �� �� ���� �ø��ų� �� ĭ �������� �����̸�
	else if (nSBCode == SB_LINEUP || nSBCode == SB_LINELEFT)
	{
		scrollAction = new PreviousScrollAction(this->notepadForm);
	}
	//4. ��ũ���� �� �� �Ʒ��� �����ų� �� ĭ ���������� �����̸�
	else if (nSBCode == SB_LINEDOWN || nSBCode == SB_LINERIGHT)
	{
		scrollAction = new LastScrollAction(this->notepadForm);
		//scrollAction = new NextScrollAction(this->notepadForm);
	}
	//scrollAction = new FirstScrollAction(this->notepadForm);

	return scrollAction;
}

//�Ҹ���
ScrollActionCreator::~ScrollActionCreator()
{

}