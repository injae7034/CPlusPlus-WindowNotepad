#include "PreviousScrollAction.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"

//����Ʈ������
PreviousScrollAction::PreviousScrollAction(NotepadForm* notepadForm)
	:ScrollAction(notepadForm)
{

}

void PreviousScrollAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
		//3.1 ��ũ���� �ѱ۱������� ��ĭ �������� �̵��Ѵ�.
		Long position = this->notepadForm->textExtent->GetTextWidth("��");
		//dynamic_cast<ScrollController*>(observer)->scroll[0]->MovePrevious(position);
	}
}

void PreviousScrollAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
		//3.1 ��ũ���� ���� ���� �̵��Ѵ�.
		Long position = this->notepadForm->textExtent->GetHeight();
		//dynamic_cast<ScrollController*>(observer)->scroll[1]->MovePrevious(position);
	}
}

//�Ҹ���
PreviousScrollAction::~PreviousScrollAction()
{

}