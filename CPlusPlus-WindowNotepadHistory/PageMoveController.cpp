#include "PageMoveController.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "TextExtent.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "CaretController.h"

//디폴트생성자
PageMoveController::PageMoveController(NotepadForm* notepadForm)
	: Observer(notepadForm)//PageMoveController의 Subject에 NotepadForm을 연관화시킴.
{
	//1. 입력받은 notepadForm의 주소를 저장한다.
	this->notepadForm = notepadForm;
	//2. CaretContoroller를 옵저버리스트에서 찾는다.
	Long i = this->subject->Search(this->notepadForm->caretController);
	//3. 옵저버리스트에 CaretController가 있으면
	if (i >= 0)
	{
		//3.1 CaretController 앞에 PageMoveController를 끼워넣는다.
		this->subject->Insert(i, this);
	}
	//4. 옵저버리스트에 CaretController가 없으면
	else
	{
		//4.1 PageMoveController를 추가한다.
		this->subject->Attach(this);
	}
}

//Update
void PageMoveController::Update()
{
	Long HScrollPos = 0;//수평스크롤의 현재위치
	Long VScrollPos = 0;//수직스크롤의 현재위치
	//메모장에서 캐럿의 세로위치 시작점과 종료점 사이에는 글자의 평균 높이만큼의 길이가 존재한다.
	Long beginCaretYPos = 0;//메모장에서 캐럿의 세로위치 시작지점
	Long endCaretYPos = 0;//메모장에서 캐럿의 세로위치 종료지점
	Long caretXPos = 0;//메모장에서 캐럿의 가로위치
	Long VerticalSum = 0;//수직스크롤의 현재위치 + 현재화면의 세로길이
	Long HorizontalSum = 0;//가로스크롤의 현재위치 + 현재화면의 가로길이
	Long distance = 0;//스크롤이 이동할 위치
	//스크롤의 Thumb는 스크롤의 Max까지 이동할 수 없다. 왜냐하면 Thumb의 길이는 현재 화면의 길이이기
	//때문에 스크롤의 Thumb는 최소 0부터 최대 Max-pageSize꺼지 이동할 수 있다.
	Long maxScrollPos = 0;//스크롤바의 Thumb이 이동할 수 최대 위치
	Long previousRowIndex = 0;//이동하기 전에 줄의 위치
	Long currentRowIndex = 0;//이동한 후에 현재 줄의 위치

	//1. 현재 줄의 위치를 구한다.
	currentRowIndex = this->notepadForm->note->GetCurrent();
	//2. 메모장에서 캐럿의 세로 범위(시작과 끝)을 구한다.(beginCaretYPos, endCaretYPos)
	beginCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex);
	endCaretYPos = this->notepadForm->textExtent->GetHeight() * (currentRowIndex + 1);
	//3. 수직스크롤을 이동하기 전에 현재 수직스크롤의 VScrollPos(스크롤의 현재 위치)를 구한다.
	VScrollPos = this->notepadForm->GetScrollPos(SB_VERT);
	//4. 세로스크롤의 현재위치(VScrollPos)와 현재화면의 세로길이의 합을 구한다.
	VerticalSum = VScrollPos + this->notepadForm->
		scrollController->scroll[1]->GetPageSize();
	//5. 캐럿의 세로 위치 시작점(beginCaretYPos)이 
	//수직스크롤의 현재위치(VScrollPos)보다 메모장에서 위에 있으면
	//캐럿이 화면보다 위에 있으면
	if (beginCaretYPos < VScrollPos)
	{
		//5.1 수직스크롤이 이동할 위치(distance)를 캐럿의 세로 위치 시작점으로 정한다.
		distance = beginCaretYPos;
		//5.2 수직스크의 현재 위치를 distance로 이동시킨다.
		this->notepadForm->scrollController->scroll[1]->Move(distance);
		//5.3 수직스크롤바의 Thumb를 수직스크롤의 현재위치로 이동시킨다.
		this->notepadForm->SetScrollPos(SB_VERT,
			this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
	}
	//6. 캐럿의 세로위치 끝지점(endCaretYPos)이 수직스크롤의 현재 위치(VScrollPos)와 
	//현재화면의 세로길이의 합(VerticalSum)보다 메모장에서 아래에 있으면
	//캐럿이 화면보다 아래에 있으면
	else if (endCaretYPos > VerticalSum)
	{
		//6.1 수직스크롤이 이동할 범위(distance)를 구한다.
		//캐럿의 세로 위치 마지막 지점에서 화면의 세로길이의 차로 정한다.
		distance = endCaretYPos - this->notepadForm->
			scrollController->scroll[1]->GetPageSize();
		//6.2 수직스크롤의 현재위치를 distance로 이동시킨다.
		this->notepadForm->scrollController->scroll[1]->Move(distance);
		//6.3 수직스크롤바의 Thumb를 수직스크롤의 현재 위치로 이동시킨다.
		this->notepadForm->SetScrollPos(SB_VERT,
			this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
	}
	//7. 메모장에서 캐럿의 가로위치(caretXPos)를 구한다.
	caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
		current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
	//8. 수평스크롤이 이동하기전에 수평스크롤의 현재위치(HScrollPos)를 구한다.
	HScrollPos = this->notepadForm->GetScrollPos(SB_HORZ);
	//9. 수평스크롤의 현재위치(HScrollPos)와 현재화면의 가로길이의 합을 구한다.
	HorizontalSum = HScrollPos + this->notepadForm->
		scrollController->scroll[0]->GetPageSize();
	//10. 캐럿의 현재 가로위치(caretXPos)가 수평스크롤의 현재위치(HScrollPos)보다 작으면
	//메모장에서 캐럿이 화면을 벗어나 왼쪽에 있으면
	if (caretXPos < HScrollPos)
	{
		//10.1 수평스크롤이 이동할 위치(distance)를 구한다.
		distance = caretXPos - (this->notepadForm->
			scrollController->scroll[0]->GetPageSize() / 5);
		//10.2 distance가 음수이면
		if (distance < 0)
		{
			//10.2.1 distance를 0으로 바꿔준다.
			distance = 0;
		}
		//10.3 수평스크롤의 현재위치를 distance로 이동시킨다.
		this->notepadForm->scrollController->scroll[0]->Move(distance);
		//10.4 수평스크롤바의 Thumb를 수평스크롤의 현재 위치로 이동시킨다.
		this->notepadForm->SetScrollPos(SB_HORZ,
			this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
	}
	//11. 캐럿의 현재 가로위치(caretXPos)가 수평스크롤의 현재위치(HScrollPos)와
	//현재화면의 가로길이의 합(HorizontalSum)보다 크면
	//메모장에서 캐럿이 화면을 벗어나 오른쪽에 있으면
	else if (caretXPos > HorizontalSum)
	{
		//11.1 수평스크롤이 이동할 위치(distance)를 구한다.
		distance = caretXPos - (this->notepadForm->
			scrollController->scroll[0]->GetPageSize() / 5 * 4);
		//11.2 수평스크롤바의 Thumb의 최대 이동값(maxScrollPos)를 구한다.
		maxScrollPos = this->notepadForm->scrollController->scroll[0]->GetMax()
			- this->notepadForm->scrollController->scroll[0]->GetPageSize();
		//11.3 distance가 maxScrollPos보다 크면
		if (distance > maxScrollPos)
		{
			//11.3.1 distance에 maxScrollPos를 대입한다.
			distance = maxScrollPos;
		}
		//11.4 수평스크롤의 현재위치를 distance로 이동시킨다.
		this->notepadForm->scrollController->scroll[0]->Move(distance);
		//11.5 수평스크롤바의 Thumb를 수평스크롤의 현재 위치로 이동시킨다.
		this->notepadForm->SetScrollPos(SB_HORZ,
			this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
	}
}

//소멸자
PageMoveController::~PageMoveController()
{

}