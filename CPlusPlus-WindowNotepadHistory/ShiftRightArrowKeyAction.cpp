#include "ShiftRightArrowKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "DummyRow.h"

//디폴트생성자
ShiftRightArrowKeyAction::ShiftRightArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//전략 패턴
void ShiftRightArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 다음으로 이동하기 전에 글자의 위치를 저장한다.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//2. 다음으로 이동하기 전에 줄의 위치를 저장한다.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//3. 다음으로 이동하고 현재 글자의 위치를 반환받는다.
	Long currentLetterIndex = this->notepadForm->current->Next();
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	Long lastRowIndex = this->notepadForm->note->GetLength() - 1;
	//4. 캐럿이 한 줄 내에서 이동했으면
	if (previousLetterIndex < currentLetterIndex)
	{
		//4.1 선택이 처음 시작되면
		if (this->notepadForm->isSelecting == false)
		{
			//4.1.1 선택이 진행되고 있는 중으로 상태를 바꾼다.
			this->notepadForm->isSelecting = true;
			//4.1.2 선택이 시작되는 캐럿의 x좌표를 저장한다.
			this->notepadForm->selectedStartXPos = previousLetterIndex;
			//4.1.3 선택이 시작되는 캐럿의 y좌표를 저장한다.
			this->notepadForm->selectedStartYPos = previousRowIndex;
		}
	}
	//5. 현재 줄의 위치가 마지막 줄의 위치보다 작고 이동 전의 글자위치와 이동 후의 글자위치가 같으면
	else if (currentRowIndex < lastRowIndex && previousLetterIndex == currentLetterIndex)
	{
		//5.1 선택이 처음 시작되면
		if (this->notepadForm->isSelecting == false)
		{
			//5.1.1 선택이 진행되고 있는 중으로 상태를 바꾼다.
			this->notepadForm->isSelecting = true;
			//5.1.2 선택이 시작되는 캐럿의 x좌표를 저장한다.
			this->notepadForm->selectedStartXPos = previousLetterIndex;
			//5.1.3 선택이 시작되는 캐럿의 y좌표를 저장한다.
			this->notepadForm->selectedStartYPos = previousRowIndex;
		}
	}
	//6. 현재 줄이 마지막 줄이 아니면
	if (currentRowIndex < lastRowIndex)
	{
		//6.1 다음 줄을 구한다.
		Glyph* nextRow = this->notepadForm->note->GetAt(currentRowIndex + 1);
		//6.2 다음 줄이 가짜 줄이면
		if (dynamic_cast<DummyRow*>(nextRow))
		{
			//6.2.1 현재 글자위치가 글자의 마지막에 있으면
			Long letterIndex = this->notepadForm->current->GetLength();
			if (currentLetterIndex == letterIndex)
			{
				//6.2.1.1 다음 줄로 이동시킨다.
				currentRowIndex = this->notepadForm->note->Next();
				//6.2.1.2 현재 줄을 다음으로 이동한 줄로 변경한다.
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
				//6.2.1.3 캐럿의 현재 위치를 처음으로 보낸다.(현재 줄이 다음 줄로 옮겨졌기 때문에
				//캐럿은 이전 줄의 마지막 위치에서 다음 줄의 처음 위치로 이동한다.)
				currentLetterIndex = this->notepadForm->current->First();
			}
		}
		//6.3 다음 줄이 진짜 줄이면
		else
		{
			//6.3.1 캐럿의 이동이 없었으면(캐럿이 줄의 마지막에 위치하고 있었기 때문에)
			if (previousLetterIndex == currentLetterIndex)
			{
				//6.3.1.1 다음 줄로 이동시킨다.
				currentRowIndex = this->notepadForm->note->Next();
				//6.3.1.2 줄의 이동이 있었으면(이동하기 전의 줄이 노트의 마지막 줄이 아니면)
				if (previousRowIndex != currentRowIndex)
				{
					//6.3.1.2.1 현재 줄을 다음으로 이동한 줄로 변경한다.
					this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
					//6.3.1.2.2 캐럿의 현재 위치를 처음으로 보낸다.(현재 줄이 다음 줄로 옮겨졌기 때문에
					//캐럿은 이전 줄의 마지막 위치에서 다음 줄의 처음 위치로 이동한다.)
					currentLetterIndex = this->notepadForm->current->First();
				}
			}
		}
	}
	//7. 글자가 선택중이면
	if (this->notepadForm->isSelecting == true)
	{
		//7.1 글자를 선택한다.
		this->notepadForm->selectingTexts->DoNext(previousRowIndex, previousLetterIndex,
			currentRowIndex, currentLetterIndex);
	}
}

//소멸자
ShiftRightArrowKeyAction::~ShiftRightArrowKeyAction()
{

}