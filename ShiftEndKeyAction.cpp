#include "ShiftEndKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "DummyRow.h"

//디폴트생성자
ShiftEndKeyAction::ShiftEndKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//전략패턴
void ShiftEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 마지막 글자로 이동하기 전에 현재 줄에서 글자의 위치를 저장한다.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//2. 현재 줄에서 마지막 글자 위치로 이동하고 글자의 위치를 반환받는다.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	Long currentLetterIndex = this->notepadForm->current->Last();
	//3. 이동이 있었으면
	if (previousLetterIndex < currentLetterIndex)
	{
		//3.1 선택이 처음 시작되면
		if (this->notepadForm->isSelecting == false)
		{
			//3.1.1 선택이 진행되고 있는 중으로 상태를 바꾼다.
			this->notepadForm->isSelecting = true;
			//3.1.2 선택이 시작되는 캐럿의 x좌표를 저장한다.
			this->notepadForm->selectedStartXPos = previousLetterIndex;
			//3.1.3 선택이 시작되는 캐럿의 y좌표를 저장한다.
			this->notepadForm->selectedStartYPos = previousRowIndex;
		}
	}
	//4. 자동개행이 진행 중이면
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//4.1 마지막 글자로 이동하기 전의 글자위치와 마지막 글자위치가 같으면
		if (previousLetterIndex == currentLetterIndex)
		{
			//4.1.1 현재 줄의 위치가 마지막 줄의 위치가 아니면
			Long lastRowIndex = this->notepadForm->note->GetLength() - 1;
			if (currentRowIndex < lastRowIndex)
			{
				//4.1.1.1 다음 줄을 구한다.
				Glyph* nextRow = this->notepadForm->note->GetAt(currentRowIndex + 1);
				//4.1.1.2 다음 줄이 가짜 줄이면
				if (dynamic_cast<DummyRow*>(nextRow))
				{
					//4.1.1.2.1 현재 줄을 다음 줄로 이동시킨다.
					currentRowIndex = this->notepadForm->note->Move(currentRowIndex + 1);
					previousRowIndex = currentRowIndex;
					this->notepadForm->current = this->notepadForm->note
						->GetAt(currentRowIndex);
					//4.1.1.2.2 마지막 글자 위치로 이동하기 전에 처음 글자의 위치를 구한다.
					previousLetterIndex = this->notepadForm->current->First();
					//4.1.1.2.3 현재 줄의 마지막 글자위치로 이동한다.
					currentLetterIndex = this->notepadForm->current->Last();
					//4.1.1.2.4 선택이 처음 시작되면
					if (this->notepadForm->isSelecting == false)
					{
						//4.1.1.2.4.1 선택이 진행되고 있는 중으로 상태를 바꾼다.
						this->notepadForm->isSelecting = true;
						//4.1.1.2.4.2 선택이 시작되는 캐럿의 x좌표를 저장한다.
						this->notepadForm->selectedStartXPos = previousLetterIndex;
						//4.1.1.2.4.3 선택이 시작되는 캐럿의 y좌표를 저장한다.
						this->notepadForm->selectedStartYPos = previousRowIndex;
					}
				}
			}
		}
	}
	//5. 선택이 진행 중이면
	if (this->notepadForm->isSelecting == true)
	{
		//5.1 텍스트를 선택한다.
		this->notepadForm->selectingTexts->DoNext(previousRowIndex, previousLetterIndex,
			currentRowIndex, currentLetterIndex);
	}
}

//소멸자
ShiftEndKeyAction::~ShiftEndKeyAction()
{

}