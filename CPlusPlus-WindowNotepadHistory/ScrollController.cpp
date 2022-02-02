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
	//2. NotepadForm�� ���� ȭ�� ũ�⸦ ���Ѵ�.
	CRect rect;
	this->notepadForm->GetClientRect(&rect);
	//3. ���� ȭ���� ũ�⿡ �°� ���� ��ũ�ѹٸ� �����Ѵ�.
	this->horizontalScrollBar = new CScrollBar();
	this->horizontalScrollBar->Create(SBS_HORZ | SBS_BOTTOMALIGN | WS_HSCROLL | WS_CHILD, 
		rect, this->notepadForm, 0);
	this->horizontalScrollBar->ShowScrollBar();
	//4. ���� ȭ���� ũ�⿡ �°� ���� ��ũ�ѹٸ� �����Ѵ�.
	this->verticalScrollBar = new CScrollBar();
	this->verticalScrollBar->Create(SBS_VERT | SBS_RIGHTALIGN |   WS_CHILD,
		rect, this->notepadForm, 0);
	this->verticalScrollBar->ShowScrollBar();
	//5. ��ũ�ѿ� ���� ������ �������ش�.
	//SCROLLINFO scrollInfo;
	//scrollInfo.cbSize = sizeof(SCROLLINFO);
	//scrollInfo.fMask = SIF_ALL;
	//scrollInfo.nPos
	//this->horizontalScrollBar->SetScrollInfo(&scrollInfo);
	//this->verticalScrollBar->SetScrollInfo(&scrollInfo);
	//5. ���� ȭ���� ���� ���̸� ���Ѵ�.
	Long pageSize = rect.Width();
	//6. ���� ȭ�鿡�� �ؽ�Ʈ�� �ִ� ���� ���̸� ���Ѵ�.
	Long max = 0;//�ؽ�Ʈ ���� ���� �ִ밪
	Long textWidth;//�ؽ�Ʈ�� ���� ����
	Long i = 0;//�� �ݺ������
	//7 Note�� ���� �������� �������� �ݺ��Ѵ�.
	while (i < this->notepadForm->note->GetLength())
	{
		//7.1 �ؽ�Ʈ�� ���� ���̸� ���Ѵ�.
		textWidth = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->note->
			GetAt(i)->GetContent());
		//7.2 �ؽ�Ʈ�� ���� ���̰� �ִ밪���� ũ��
		if (textWidth > max)
		{
			//7.2.1 �ִ밪�� �������ش�.
			max = textWidth;
		}
		i++;
	}
	//8. ���ν�ũ���� �����͸� �������� ���ν�ũ���� �������ش�.
	this->scroll[0] = new HorizontalScroll(this, 0, 0, max, pageSize);
	//9. ���� ȭ���� ���� ���̸� ���Ѵ�.
	pageSize = rect.Height();
	//10. ���� ȭ�鿡�� �ؽ�Ʈ�� �� ���� ���̸� ���Ѵ�.
	max = this->notepadForm->textExtent->GetHeight() * this->notepadForm->note->GetLength();
	//11. ���ν�ũ���� ����� �����͸� �������� ���ο� ���ν�ũ���� �������ش�.
	this->scroll[1] = new VerticalScroll(this, 0, 0, max, pageSize);
	//12. ScrollController�� ObserverList�� �߰��Ѵ�.
	this->subject->Attach(this);
	//10. ��������� �뺸�Ѵ�.
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
	//5. ���� ȭ���� ���� ���̰� ���� ȭ�鿡�� �ؽ�Ʈ�� ���� ���� �ִ밪���� ũ�ų� ������
	if (pageSize >= max)
	{
		//5.1 ĳ���� ���� ��ũ�ѹٸ� �̿����� ���ϰ� �Ѵ�.
		this->horizontalScrollBar->EnableScrollBar(ESB_DISABLE_BOTH);
	}
	//6. ���� ȭ���� ���� ���̰� ���� ȭ�鿡�� �ؽ�Ʈ�� ���� ���� �ִ밪���� ������
	else
	{
		//6.1 ĳ���� ���� ��ũ�ѹٸ� �̿��� �� �ְ� �Ѵ�.
		this->horizontalScrollBar->EnableScrollBar(ESB_ENABLE_BOTH);
	}
	//7. ���� ������ ȭ�鿡�� ���ν�ũ�ѿ� ���� �����Ͱ� ����Ǿ��� ������ ������ ���ν�ũ���� �Ҵ������Ѵ�.
	if (this->scroll[0] != NULL)
	{
		delete this->scroll[0];
	}
	//8. ���� ���ν�ũ�ѹ��� ���� ��ġ�� ���Ѵ�.
	SCROLLINFO scrollInfo;
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_ALL;
	this->horizontalScrollBar->GetScrollInfo(&scrollInfo);
	//9. ���ν�ũ���� ����� �����͸� �������� ���ο� ���ν�ũ���� �������ش�.
	this->scroll[0] = new HorizontalScroll(this, scrollInfo.nPos, 0, max, pageSize);
	//10. ���� ȭ���� ���� ���̸� ���Ѵ�.
	pageSize = rect.Height();
	//11. ���� ȭ�鿡�� �ؽ�Ʈ�� �� ���� ���̸� ���Ѵ�.
	max = this->notepadForm->textExtent->GetHeight() * this->notepadForm->note->GetLength();
	//12. ���� ȭ���� ���� ���̰� ���� ȭ�鿡�� �ؽ�Ʈ�� �� ���� ���̺��� ũ�ų� ������
	if (pageSize >= max)
	{
		//12.1 ĳ���� ���� ��ũ�ѹٸ� �̿����� ���ϰ� �Ѵ�.
		this->verticalScrollBar->EnableScrollBar(ESB_DISABLE_BOTH);
	}
	//13. ���� ȭ���� ���� ���̰� ���� ȭ�鿡�� �ؽ�Ʈ�� �� ���� ���� ������
	else
	{
		//13.1 ĳ���� ���� ��ũ�ѹٸ� �̿��� �� �ְ� �Ѵ�.
		this->verticalScrollBar->EnableScrollBar(ESB_ENABLE_BOTH);
	}
	//14. ���� ������ ȭ�鿡�� ���ν�ũ�ѿ� ���� �����Ͱ� ����Ǿ��� ������ ������ ���ν�ũ���� �Ҵ������Ѵ�.
	if (this->scroll[1] != NULL)
	{
		delete this->scroll[1];
	}
	//15. ���� ���ν�ũ�ѹ��� ���� ��ġ�� ���Ѵ�.
	//this->verticalScrollBar->GetScrollInfo(&scrollInfo);
	scrollInfo.nPos = this->verticalScrollBar->GetScrollPos();
	//16. ���ν�ũ���� ����� �����͸� �������� ���ο� ���ν�ũ���� �������ش�.
	this->scroll[1] = new VerticalScroll(this, scrollInfo.nPos, 0, max, pageSize);
}

//�Ҹ���
ScrollController::~ScrollController()
{
	//1. CSrollBar�� �Ҵ������Ѵ�.
	if (this->horizontalScrollBar != 0)
	{
		delete this->horizontalScrollBar;
	}
	if (this->verticalScrollBar != 0)
	{
		delete this->verticalScrollBar;
	}
	//2. scroll�� �Ҵ������Ѵ�.
	if (this->scroll[0] != 0)
	{
		delete this->scroll[0];
	}
	if (this->scroll[1] != 0)
	{
		delete this->scroll[1];
	}
}