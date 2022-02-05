#include "RightArrowKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "DummyRow.h"

//디폴트생성자
RightArrowKeyAction::RightArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void RightArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 선택이 진행되고 있는 중이었으면
	if (this->notepadForm->isSelecting == true)
	{
		//1.1. 선택된 텍스트를 선택해제한다.(선택을 끝낸다.)
		this->notepadForm->selectingTexts->Undo();
		//1.2 선택이 끝난 상태로 바꾼다.
		this->notepadForm->isSelecting = false;
		//1.3 선택이 끝났기 때문에 캐럿의 x좌표를 0으로 저장한다.
		this->notepadForm->selectedStartXPos = 0;
		//1.4 선택이 끝났기 때문에 캐럿의 y좌표를 0으로 저장한다.
		this->notepadForm->selectedStartYPos = 0;
	}
	//2. 다음으로 이동하기 전에 글자의 위치를 저장한다.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//3. 다음으로 이동하기 전에 줄의 위치를 저장한다.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//4. 다음으로 이동하고 현재 글자의 위치를 반환받는다.
	Long currentLetterIndex = this->notepadForm->current->Next();
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//5. 현재 줄이 마지막 줄이 아니면
	Long lastRowIndex = this->notepadForm->note->GetLength() - 1;
	if (currentRowIndex < lastRowIndex)
	{
		//5.1 다음 줄을 구한다.
		Glyph* nextRow = this->notepadForm->note->GetAt(currentRowIndex + 1);
		//5.2 다음 줄이 가짜 줄이면
		if (dynamic_cast<DummyRow*>(nextRow))
		{
			//5.2.1 현재 글자위치가 글자의 마지막에 있으면
			Long letterIndex = this->notepadForm->current->GetLength();
			if (currentLetterIndex == letterIndex)
			{
				//5.2.1 다음 줄로 이동시킨다.
				currentRowIndex = this->notepadForm->note->Next();
				//5.2.2 현재 줄을 다음으로 이동한 줄로 변경한다.
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
				//5.2.3 캐럿의 현재 위치를 처음으로 보낸다.(현재 줄이 다음 줄로 옮겨졌기 때문에
				//캐럿은 이전 줄의 마지막 위치에서 다음 줄의 처음 위치로 이동한다.)
				currentLetterIndex = this->notepadForm->current->First();
			}
		}
		//5.3 다음 줄이 진짜 줄이면
		else
		{
			//5.3.1 캐럿의 이동이 없었으면(캐럿이 줄의 마지막에 위치하고 있었기 때문에)
			if (previousLetterIndex == currentLetterIndex)
			{
				//5.3.1.1 다음 줄로 이동시킨다.
				currentRowIndex = this->notepadForm->note->Next();
				//5.3.1.2 줄의 이동이 있었으면(이동하기 전의 줄이 노트의 마지막 줄이 아니면)
				if (previousRowIndex != currentRowIndex)
				{
					//5.3.1.2.1 현재 줄을 다음으로 이동한 줄로 변경한다.
					this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
					//5.3.1.2.2 캐럿의 현재 위치를 처음으로 보낸다.(현재 줄이 다음 줄로 옮겨졌기 때문에
					//캐럿은 이전 줄의 마지막 위치에서 다음 줄의 처음 위치로 이동한다.)
					currentLetterIndex = this->notepadForm->current->First();
				}
			}
		}
	}
	
}

//소멸자
RightArrowKeyAction::~RightArrowKeyAction()
{

}

