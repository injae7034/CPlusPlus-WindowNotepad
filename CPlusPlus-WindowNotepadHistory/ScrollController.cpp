#include "ScrollController.h"
#include "NotepadForm.h"
#include "Scroll.h"
#include "HorizontalScroll.h"
#include "VerticalScroll.h"
#include "TextExtent.h"
#include "Glyph.h"

//디폴트생성자
ScrollController::ScrollController(NotepadForm* notepadForm)
	: Observer(notepadForm)//ScrollController의 Subject에 NotepadForm을 연관화시킴.
{
	//1. ScrollController와 NotepadForm을 연관화시켜준다.
	this->notepadForm = notepadForm;
	//2. NotepadForm의 현재 화면 크기를 구한다.
	CRect rect;
	this->notepadForm->GetClientRect(&rect);
	//3. 현재 화면의 크기에 맞게 수평 스크롤바를 생성한다.
	this->horizontalScrollBar = new CScrollBar();
	this->horizontalScrollBar->Create(SBS_HORZ | SBS_BOTTOMALIGN | WS_HSCROLL | WS_CHILD, 
		rect, this->notepadForm, 0);
	this->horizontalScrollBar->ShowScrollBar();
	//4. 현재 화면의 크기에 맞게 수직 스크롤바를 생성한다.
	this->verticalScrollBar = new CScrollBar();
	this->verticalScrollBar->Create(SBS_VERT | SBS_RIGHTALIGN |   WS_CHILD,
		rect, this->notepadForm, 0);
	this->verticalScrollBar->ShowScrollBar();
	//5. 스크롤에 대한 정보를 설정해준다.
	//SCROLLINFO scrollInfo;
	//scrollInfo.cbSize = sizeof(SCROLLINFO);
	//scrollInfo.fMask = SIF_ALL;
	//scrollInfo.nPos
	//this->horizontalScrollBar->SetScrollInfo(&scrollInfo);
	//this->verticalScrollBar->SetScrollInfo(&scrollInfo);
	//5. 현재 화면의 가로 길이를 구한다.
	Long pageSize = rect.Width();
	//6. 현재 화면에서 텍스트의 최대 가로 길이를 구한다.
	Long max = 0;//텍스트 가로 길이 최대값
	Long textWidth;//텍스트의 가로 길이
	Long i = 0;//줄 반복제어변수
	//7 Note의 줄의 개수보다 작은동안 반복한다.
	while (i < this->notepadForm->note->GetLength())
	{
		//7.1 텍스트의 가로 길이를 구한다.
		textWidth = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->note->
			GetAt(i)->GetContent());
		//7.2 텍스트의 가로 길이가 최대값보다 크면
		if (textWidth > max)
		{
			//7.2.1 최대값을 변경해준다.
			max = textWidth;
		}
		i++;
	}
	//8. 가로스크롤의 데이터를 바탕으로 가로스크롤을 생성해준다.
	this->scroll[0] = new HorizontalScroll(this, 0, 0, max, pageSize);
	//9. 현재 화면의 세로 길이를 구한다.
	pageSize = rect.Height();
	//10. 현재 화면에서 텍스트의 총 세로 길이를 구한다.
	max = this->notepadForm->textExtent->GetHeight() * this->notepadForm->note->GetLength();
	//11. 세로스크롤의 변경된 데이터를 바탕으로 새로운 세로스크롤을 생성해준다.
	this->scroll[1] = new VerticalScroll(this, 0, 0, max, pageSize);
	//12. ScrollController를 ObserverList에 추가한다.
	this->subject->Attach(this);
	//10. 변경사항을 통보한다.
	this->notepadForm->Notify();
}

//Update
void ScrollController::Update()
{
	//스크롤의 상태가 변경되었다는 통보(Notify)를 받아서 변경사항들을 Update하기 위한 처리과정
	//1. 현재 화면의 크기를 구한다.
	CRect rect;
	this->notepadForm->GetClientRect(&rect);
	//2. 현재 화면의 가로 길이를 구한다.
	Long pageSize = rect.Width();
	//3. 현재 화면에서 텍스트의 최대 가로 길이를 구한다.
	Long max = 0;//텍스트 가로 길이 최대값
	Long textWidth;//텍스트의 가로 길이
	Long i = 0;//줄 반복제어변수
	//4. Note의 줄의 개수보다 작은동안 반복한다.
	while (i < this->notepadForm->note->GetLength())
	{
		//4.1 텍스트의 가로 길이를 구한다.
		textWidth = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->note->
			GetAt(i)->GetContent());
		//4.2 텍스트의 가로 길이가 최대값보다 크면
		if (textWidth > max)
		{
			//4.2.1 최대값을 변경해준다.
			max = textWidth;
		}
		i++;
	}
	//5. 현재 화면의 가로 길이가 현재 화면에서 텍스트의 가로 길이 최대값보다 크거나 같으면
	if (pageSize >= max)
	{
		//5.1 캐럿의 가로 스크롤바를 이용하지 못하게 한다.
		this->horizontalScrollBar->EnableScrollBar(ESB_DISABLE_BOTH);
	}
	//6. 현재 화면의 가로 길이가 현재 화면에서 텍스트의 가로 길이 최대값보다 작으면
	else
	{
		//6.1 캐럿의 가로 스크롤바를 이용할 수 있게 한다.
		this->horizontalScrollBar->EnableScrollBar(ESB_ENABLE_BOTH);
	}
	//7. 현재 윈도우 화면에서 가로스크롤에 대한 데이터가 변경되었기 때문에 기존의 가로스크롤을 할당해제한다.
	if (this->scroll[0] != NULL)
	{
		delete this->scroll[0];
	}
	//8. 현재 가로스크롤바의 현재 위치를 구한다.
	SCROLLINFO scrollInfo;
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_ALL;
	this->horizontalScrollBar->GetScrollInfo(&scrollInfo);
	//9. 가로스크롤의 변경된 데이터를 바탕으로 새로운 가로스크롤을 생성해준다.
	this->scroll[0] = new HorizontalScroll(this, scrollInfo.nPos, 0, max, pageSize);
	//10. 현재 화면의 세로 길이를 구한다.
	pageSize = rect.Height();
	//11. 현재 화면에서 텍스트의 총 세로 길이를 구한다.
	max = this->notepadForm->textExtent->GetHeight() * this->notepadForm->note->GetLength();
	//12. 현재 화면의 세로 길이가 현재 화면에서 텍스트의 총 세로 길이보다 크거나 같으면
	if (pageSize >= max)
	{
		//12.1 캐럿의 세로 스크롤바를 이용하지 못하게 한다.
		this->verticalScrollBar->EnableScrollBar(ESB_DISABLE_BOTH);
	}
	//13. 현재 화면의 세로 길이가 현재 화면에서 텍스트의 총 세로 길이 작으면
	else
	{
		//13.1 캐럿의 세로 스크롤바를 이용할 수 있게 한다.
		this->verticalScrollBar->EnableScrollBar(ESB_ENABLE_BOTH);
	}
	//14. 현재 윈도우 화면에서 세로스크롤에 대한 데이터가 변경되었기 때문에 기존의 세로스크롤을 할당해제한다.
	if (this->scroll[1] != NULL)
	{
		delete this->scroll[1];
	}
	//15. 현재 세로스크롤바의 현재 위치를 구한다.
	//this->verticalScrollBar->GetScrollInfo(&scrollInfo);
	scrollInfo.nPos = this->verticalScrollBar->GetScrollPos();
	//16. 세로스크롤의 변경된 데이터를 바탕으로 새로운 세로스크롤을 생성해준다.
	this->scroll[1] = new VerticalScroll(this, scrollInfo.nPos, 0, max, pageSize);
}

//소멸자
ScrollController::~ScrollController()
{
	//1. CSrollBar를 할당해제한다.
	if (this->horizontalScrollBar != 0)
	{
		delete this->horizontalScrollBar;
	}
	if (this->verticalScrollBar != 0)
	{
		delete this->verticalScrollBar;
	}
	//2. scroll을 할당해제한다.
	if (this->scroll[0] != 0)
	{
		delete this->scroll[0];
	}
	if (this->scroll[1] != 0)
	{
		delete this->scroll[1];
	}
}