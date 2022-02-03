#include "ShiftPageDownKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"
#include "SelectText.h"

//디폴트생성자
ShiftPageDownKeyAction::ShiftPageDownKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//전략패턴
void ShiftPageDownKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 캐럿을 이동시키기 전에 먼저 현재 캐럿이 있는 곳으로 스크롤을 이동시킨다.
	this->notepadForm->Notify();

	//스크롤을 캐럿이 있는 곳으로 이동시킨 캐럿이 있는 곳을 기준삼아 다시 스크롤을 페이지단위로 이동시킨다.
	//2. 페이지 단위로 이동하기 전에 수직스크롤의 현재 위치를 저장한다.
	Long previousVPos = this->notepadForm->scrollController->scroll[1]->GetCurrentPos();
	//3. 수직스크롤을 다음 페이지로 이동시키고 이동된 수직스크롤의 현재 위치를 반환받는다.
	Long currentVPos = this->notepadForm->scrollController->scroll[1]->PageNext();
	//4. 수직스크롤에 이동이 있으면(previousVPos와 currentVPos가 다르면 이동이 있었다는 뜻임)
	if (previousVPos != currentVPos)
	{
		//4.1 수직스크롤바의 수직스크롤을 수직스크롤의 현재 위치로 이동시킨다.
		this->notepadForm->SetScrollPos(SB_VERT,
			this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
		//4.2 메모장에서 캐럿이 이동할 거리를 구한다.
		Long distance = currentVPos - previousVPos;
		//4.3 캐럿이 이동할 줄의 개수를 구한다.
		Long MovingRowCount = distance / this->notepadForm->textExtent->GetHeight();
		//4.4 이동하기 전에 줄의 위치를 구한다.
		Long previousRowIndex = this->notepadForm->note->GetCurrent();
		//4.5 이동하기 전의 글자 위치를 구한다.
		Long previousLetterIndex = this->notepadForm->current->GetCurrent();
		//4.6 현재 줄의 위치를 이동시킨다.
		Long currentRowIndex = previousRowIndex + MovingRowCount;
		currentRowIndex = this->notepadForm->note->Move(currentRowIndex);
		//4.7 이동한 후의 현재줄을 변경하고 현재 글자 위치를 구한다.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
		Long currentLetterIndex = this->notepadForm->current->GetCurrent();
		//4.8 이동하기 전 글자 위치가 0이 아니고 이동한 후 현재줄의 글자개수가 0이 아니면
		if (previousLetterIndex != 0 && this->notepadForm->current->GetLength() != 0)
		{
			//4.8.1 이동하기 전 줄의 텍스트 폭을 구한다.
			Long previousRowTextWidth = this->notepadForm->textExtent->
				GetTextWidth(this->notepadForm->note->GetAt(previousRowIndex)
					->GetPartOfContent(previousLetterIndex));
			//4.8.2 캐럿의 현재 가로 위치를 처음으로 이동시킨다.
			this->notepadForm->current->First();//캐럿의 가로 위치를 맨 처음(0)으로 보냄
			//4.8.3 첫번째 글자를 읽기 위해 캐럿을 1 증가시킨다.
			currentLetterIndex = this->notepadForm->current->Next();
			//4.8.4 현재 줄의 택스트의 폭을 구한다.
			Long currentRowTextWidth = this->notepadForm->textExtent->
				GetTextWidth(this->notepadForm->current->
					GetPartOfContent(currentLetterIndex).c_str());
			//4.8.5 i(캐럿의 현재 가로위치)가 length(현재줄의 글자개수)보다 작고
			//현재 줄의 텍스트 크기가 이동하기 전 줄의 텍스트 크기보다 작은동안 반복한다.
			while (currentLetterIndex < this->notepadForm->current->GetLength()
				&& currentRowTextWidth < previousRowTextWidth)
			{
				//4.8.5.1 i(캐럿의 현재 가로위치)를 증가시킨다
				//(캐럿의 가로 위치를 다음 칸으로 이동시킨다.).
				currentLetterIndex = this->notepadForm->current->Next();
				//4.8.5.2 현재 줄의 캐럿의 가로 위치까지의 택스트 폭을 구한다.
				currentRowTextWidth = this->notepadForm->textExtent->
					GetTextWidth(this->notepadForm->current->
						GetPartOfContent(currentLetterIndex).c_str());
			}
			//4.8.6 현재 줄의 텍스트 폭에서 이전 줄의 텍스트 폭을 뺀다.
			Long difference = currentRowTextWidth - previousRowTextWidth;
			//4.8.7 차이가 0보다 크면(현재줄의 텍스트를 다 읽지 못한 경우)
			if (difference > 0)
			{
				//4.8.7.1 캐럿의 현재 가로위치 이전의 글자 폭을 구한다.
				Long letterWidth = this->notepadForm->textExtent->
					GetTextWidth(this->notepadForm->current->
						GetAt(currentLetterIndex - 1)->GetContent());
				Long halfLetterSize = letterWidth / 2;
				//4.8.7.2 차이가 읽은 글자크기의 절반보다 같거나 크면
				if (difference >= halfLetterSize)
				{
					//4.8.7.2.1 캐럿의 현재 가로 위치를 이전으로 이동한다.
					currentLetterIndex = this->notepadForm->current->Previous();
				}
			}
		}
		//4.9 캐럿의 이전 가로 위치가 0 또는 캐럿의 현재 줄의 글자개수가 0개이면
		else
		{
			//4.9.1 현재 캐럿의 가로 위치를 0으로 이동시킨다.
			currentLetterIndex = this->notepadForm->current->First();
		}
		//4.10 글자를 선택한다.
		SelectText selectText(this->notepadForm);
		selectText.DoNext(previousRowIndex, previousLetterIndex, currentRowIndex,
			currentLetterIndex);
	}
}

//소멸자
ShiftPageDownKeyAction::~ShiftPageDownKeyAction()
{

}