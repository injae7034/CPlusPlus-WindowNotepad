#include "FirstScrollAction.h"
#include "ScrollController.h"
#include "Scroll.h"

//����Ʈ������
FirstScrollAction::FirstScrollAction(NotepadForm* notepadForm)
	:ScrollAction(notepadForm)
{

}

void FirstScrollAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. ������ ����Ʈ �迭���� �ش���ġ�� �������� ���Ѵ�.
	Long i = 0;
	Observer* observer = this->notepadForm->GetAt(i);
	//2. i�� length���� ���� ���� �������� ĳ���Ŵ����� �ƴѵ��� �ݺ��Ѵ�.
	while (i < this->notepadForm->GetLength()
		&& dynamic_cast<ScrollController*>(observer) != observer)
	{
		//2.1 ������ ����Ʈ���� �������� ���Ѵ�.
		observer = this->notepadForm->GetAt(i);
		//2.2 i�� ������Ų��.
		i++;
	}
	//3. ScrollController�̸�
	if (dynamic_cast<ScrollController*>(observer))
	{
		//3.1 ��ũ���� ó������ �̵��Ѵ�.
		dynamic_cast<ScrollController*>(observer)->scroll[0]->First();
	}
}

void FirstScrollAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. ������ ����Ʈ �迭���� �ش���ġ�� �������� ���Ѵ�.
	Long i = 0;
	Observer* observer = this->notepadForm->GetAt(i);
	//2. i�� length���� ���� ���� �������� ��ũ�ѸŴ����� �ƴѵ��� �ݺ��Ѵ�.
	while (i < this->notepadForm->GetLength()
		&& dynamic_cast<ScrollController*>(observer) != observer)
	{
		//2.1 ������ ����Ʈ���� �������� ���Ѵ�.
		observer = this->notepadForm->GetAt(i);
		//2.2 i�� ������Ų��.
		i++;
	}
	//3. ScrollController�̸�
	if (dynamic_cast<ScrollController*>(observer))
	{
		//3.1 ��ũ���� �� ���� �̵��Ѵ�.
		dynamic_cast<ScrollController*>(observer)->scroll[1]->First();
	}
}

//�Ҹ���
FirstScrollAction::~FirstScrollAction()
{

}