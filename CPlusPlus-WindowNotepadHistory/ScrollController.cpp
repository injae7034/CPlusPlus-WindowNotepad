#include "ScrollController.h"
#include "NotepadForm.h"
#include "Scroll.h"
#include "HorizontalScroll.h"
#include "VerticalScroll.h"
#include "TextExtent.h"
#include "Glyph.h"
#include "PageMoveController.h"

//디폴트생성자
ScrollController::ScrollController(NotepadForm* notepadForm)
	: Observer(notepadForm)//ScrollController의 Subject에 NotepadForm을 연관화시킴.
{
	//1. ScrollController와 NotepadForm을 연관화시켜준다.
	this->notepadForm = notepadForm;
	//3. 현재 화면의 크기에 맞게 수평 스크롤바를 생성한다.
	this->notepadForm->ModifyStyle(WS_HSCROLL, 0);
	//4. 수평스크롤을 힙에 할당해준다.
	this->scroll[0] = new HorizontalScroll(this);
	//5. 현재 화면의 크기에 맞게 수직 스크롤바를 생성한다.
	this->notepadForm->ModifyStyle(WS_VSCROLL, 0);
	//6. 수직스크롤을 힙에 할당해준다.
	this->scroll[1] = new VerticalScroll(this);
	//7. ScrollController를 ObserverList에 추가한다.
	this->subject->Attach(this);
	//8. 변경사항을 통보한다.
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
	//5. 수평스크롤에 대한 정보를 갱신한다.
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
	//6. 자동개행이 진행중이 아니면
	if (this->notepadForm->isRowAutoChanging == false)
	{
		//6.1 현재 화면크기로 설정한다.(그래거 현재줄의 max값이 현재 화면의 가로 길이를 벗어나면
		//스크롤이 생성됨)
		scrollInfo.nPage = this->scroll[0]->pageSize;
	}
	//7. 자동개행이 진행 중이면
	else
	{
		//7.1 현재 화면의 가로길이를 줄의 길이 max보다 1 크게 설정한다
		//자동개행이 진행중이면 절대로 가로 스크롤이 생성되면 안되기 때문에
		//무조건 현재 최대 줄의 가로 길이보다 1크게 해서 가로 스크롤이 생성되지 않게 막는다.
		scrollInfo.nPage = this->scroll[0]->max + 1;

	}
	scrollInfo.nTrackPos = 2;
	//7. 수평스크롤바에 대한 수평스크롤 정보를 갱신해준다.
	this->notepadForm->SetScrollInfo(SB_HORZ, &scrollInfo);
	//8. 현재 화면의 세로 길이를 구한다.
	pageSize = rect.Height();
	//9.페이지 단위로 이동할 때 화면에서 위에 줄이 잘리지 않도록 여백을 구해서 max에 더해준다.
	Long letterHeight = this->notepadForm->textExtent->GetHeight();
	//nMax랑 nPage랑 크기가 같으면 수평스크롤이 생기기 때문에 -1을 해서 nMax보다 nPage가 작게 해줌!
	//왜냐혀면 뒤에서 max + blank를 해주기 때문에!
	Long blank = pageSize - (pageSize / letterHeight * letterHeight) - 1;
	//10. 현재 화면에서 텍스트의 총 세로 길이(용지최대세로길이)를 구한다.
	max = this->notepadForm->textExtent->GetHeight() * (this->notepadForm->note->GetLength());
	//11. 수직스크롤에 대한 정보를 갱신해준다.
	this->scroll[1]->min = 0;
	this->scroll[1]->max = max + blank;//페이지단위 이동시 남는 공간을 더해줌.
	this->scroll[1]->currentPos = this->notepadForm->GetScrollPos(SB_VERT);
	this->scroll[1]->pageSize = pageSize;
	scrollInfo.nMin = this->scroll[1]->min;
	scrollInfo.nMax = this->scroll[1]->max;
	scrollInfo.nPos = this->scroll[1]->currentPos;
	scrollInfo.nPage = this->scroll[1]->pageSize;
	//12. 수직스크롤바에 대한 수직스크롤 정보를 갱신해준다.
	this->notepadForm->SetScrollInfo(SB_VERT, &scrollInfo);
}

//소멸자
ScrollController::~ScrollController()
{
	//1. scroll을 할당해제한다.
	if (this->scroll[0] != 0)
	{
		delete this->scroll[0];
	}
	if (this->scroll[1] != 0)
	{
		delete this->scroll[1];
	}
}