#include "EndKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "DummyRow.h"

//디폴트생성자
EndKeyAction::EndKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//전략패턴
void EndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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
	//2. 마지막 글자로 이동하기 전에 현재 줄에서 글자의 위치를 저장한다.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//3. 현재 줄에서 마지막 글자 위치로 이동하고 글자의 위치를 반환받는다.
	Long currentLetterIndex = this->notepadForm->current->Last();
	//4. 자동개행이 진행 중이면
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//4.1 마지막 글자로 이동하기 전의 글자위치와 마지막 글자위치가 같으면
		if (previousLetterIndex == currentLetterIndex)
		{
			//4.1.1 현재 줄의 위치를 구한다.
			Long currentRowIndex = this->notepadForm->note->GetCurrent();
			//4.1.2 현재 줄의 위치가 마지막 줄의 위치가 아니면
			Long lastRowIndex = this->notepadForm->note->GetLength() - 1;
			if (currentRowIndex < lastRowIndex)
			{
				//4.1.2.1 다음 줄을 구한다.
				Glyph* nextRow = this->notepadForm->note->GetAt(currentRowIndex + 1);
				//4.1.2.2 다음 줄이 가짜 줄이면
				if (dynamic_cast<DummyRow*>(nextRow))
				{
					//4.1.2.2.1 현재 줄을 다음 줄로 이동시킨다.
					currentRowIndex = this->notepadForm->note->Move(currentRowIndex + 1);
					this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
					//4.1.2.2.2 현재 줄의 마지막 글자위치로 이동한다.
					currentLetterIndex = this->notepadForm->current->Last();
				}
			}
		}
	}
}

//소멸자
EndKeyAction::~EndKeyAction()
{

}