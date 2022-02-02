#include "ScrollController.h"
#include "NotepadForm.h"
#include "Scroll.h"
#include "HorizontalScroll.h"
#include "VerticalScroll.h"
#include "TextExtent.h"
#include "Glyph.h"

//����Ʈ������
ScrollController::ScrollController(NotepadForm* notepadForm)
	: Observer(notepadForm)//ScrollController�� Subject�� NotepadForm�� ����ȭ��Ŵ.
{
	//1. ScrollController�� NotepadForm�� ����ȭ�����ش�.
	this->notepadForm = notepadForm;
	//3. ���� ȭ���� ũ�⿡ �°� ���� ��ũ�ѹٸ� �����Ѵ�.
	this->notepadForm->ModifyStyle(WS_HSCROLL, 0);
	//4. ����ũ���� ���� �Ҵ����ش�.
	this->scroll[0] = new HorizontalScroll(this);
	//5. ���� ȭ���� ũ�⿡ �°� ���� ��ũ�ѹٸ� �����Ѵ�.
	this->notepadForm->ModifyStyle(WS_VSCROLL, 0);
	//6. ������ũ���� ���� �Ҵ����ش�.
	this->scroll[1] = new VerticalScroll(this);
	//7. ScrollController�� ObserverList�� �߰��Ѵ�.
	this->subject->Attach(this);
	//8. ��������� �뺸�Ѵ�.
	this->notepadForm->Notify();
}

//Update
void ScrollController::Update()
{
	//��ũ���� ���°� ����Ǿ��ٴ� �뺸(Notify)�� �޾Ƽ� ������׵��� Update�ϱ� ���� ó������
	//1. ���� ȭ���� ũ�⸦ ���Ѵ�.
	CRect rect;
	this->notepadForm->GetClientRect(&rect);
	//2. ���� ȭ���� ���� ���̸� ���Ѵ�.
	Long pageSize = rect.Width();
	//3. ���� ȭ�鿡�� �ؽ�Ʈ�� �ִ� ���� ���̸� ���Ѵ�.
	Long max = 0;//�ؽ�Ʈ ���� ���� �ִ밪
	Long textWidth;//�ؽ�Ʈ�� ���� ����
	Long i = 0;//�� �ݺ������
	//4. Note�� ���� �������� �������� �ݺ��Ѵ�.
	while (i < this->notepadForm->note->GetLength())
	{
		//4.1 �ؽ�Ʈ�� ���� ���̸� ���Ѵ�.
		textWidth = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->note->
			GetAt(i)->GetContent());
		//4.2 �ؽ�Ʈ�� ���� ���̰� �ִ밪���� ũ��
		if (textWidth > max)
		{
			//4.2.1 �ִ밪�� �������ش�.
			max = textWidth;
		}
		i++;
	}
	//5. ����ũ�ѿ� ���� ������ �����Ѵ�.
	this->scroll[0]->min = 0;
	this->scroll[0]->max = max;
	this->scroll[0]->currentPos = this->notepadForm->GetScrollPos(SB_HORZ);
	this->scroll[0]->pageSize = pageSize;
	SCROLLINFO scrollInfo;
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_ALL;
	scrollInfo.nMin = this->scroll[0]->min;
	scrollInfo.nMax = this->scroll[0]->max;
	scrollInfo.nPos = this->scroll[0]->currentPos;
	scrollInfo.nPage = this->scroll[0]->pageSize;
	scrollInfo.nTrackPos = 2;
	//6. ����ũ�ѹٿ� ���� ����ũ�� ������ �������ش�.
	this->notepadForm->SetScrollInfo(SB_HORZ, &scrollInfo);
	//7. ���� ȭ���� ���� ���̸� ���Ѵ�.
	pageSize = rect.Height();
	//8.������ ������ �̵��� �� ȭ�鿡�� ���� ���� �߸��� �ʵ��� ������ ���ؼ� max�� �����ش�.
	Long letterHeight = this->notepadForm->textExtent->GetHeight();
	Long blank = pageSize - (pageSize / letterHeight * letterHeight);
	//9. ���� ȭ�鿡�� �ؽ�Ʈ�� �� ���� ����(�����ִ뼼�α���)�� ���Ѵ�.
	max = this->notepadForm->textExtent->GetHeight() * (this->notepadForm->note->GetLength());
	//10. ������ũ�ѿ� ���� ������ �������ش�.
	this->scroll[1]->min = 0;
	this->scroll[1]->max = max + blank;//���������� �̵��� ���� ������ ������.
	this->scroll[1]->currentPos = this->notepadForm->GetScrollPos(SB_VERT);
	this->scroll[1]->pageSize = pageSize;
	scrollInfo.nMin = this->scroll[1]->min;
	scrollInfo.nMax = this->scroll[1]->max;
	scrollInfo.nPos = this->scroll[1]->currentPos;
	scrollInfo.nPage = this->scroll[1]->pageSize;
	//11. ������ũ�ѹٿ� ���� ������ũ�� ������ �������ش�.
	this->notepadForm->SetScrollInfo(SB_VERT, &scrollInfo);
}

//�Ҹ���
ScrollController::~ScrollController()
{
	//1. scroll�� �Ҵ������Ѵ�.
	if (this->scroll[0] != 0)
	{
		delete this->scroll[0];
	}
	if (this->scroll[1] != 0)
	{
		delete this->scroll[1];
	}
}