#include "SelectText.h"
#include "NotepadForm.h"
#include "Glyph.h"

//디폴트생성자
SelectText::SelectText(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
}

//오른쪽으로 선택하다
void SelectText::DoNext(Long previousRowIndex, Long previousLetterIndex, Long currentRowIndex,
	Long currentLetterIndex)
{
	Glyph* row = 0;
	Glyph* letter = 0;
	Long letterCount = 0;
	Long letterIndex = 0;
	//1. 이동하기 전의 줄의 위치가 이동한 후의 줄의 위치보다 작거나 같은 동안 반복한다.
	while (previousRowIndex <= currentRowIndex)
	{
		//1.1 줄을 구한다.
		row = this->notepadForm->note->GetAt(previousRowIndex);
		//1.2 이동하기 전 줄의 위치값이 이동한 후 줄의 위치값과 같지 않으면
		if (previousRowIndex != currentRowIndex)
		{
			//1.2.1 줄의 글자 개수를 대입한다.
			letterCount = row->GetLength();
		}
		//1.3  이동하기 전 줄의 위치값이 이동한 후 줄의 위치값과 같으면
		else
		{
			//1.3.1 이동한 후 글자 위치를 대입한다.
			letterCount = currentLetterIndex;
		}
		//1.4 글자 위치를 원위치시킨다.
		letterIndex = previousLetterIndex;
		//1.5 글자 위치가 letterCount보다 작은동안 반복한다.
		while (letterIndex < letterCount)
		{
			//1.5.1 글자를 구한다.
			letter = row->GetAt(letterIndex);
			//1.5.2 글자가 선택이 안되어있으면
			if (letter->IsSelected() == false)
			{
				///1.5.2.1 글자를 선택이 된 상태로 바꿔준다.
				letter->Select(true);
			}
			//1.5.3 글자가 선택이 되어 있으면
			else
			{
				//1.5.3.1 글자를 선택이 안된 상태로 바꿔준다.
				letter->Select(false);
			}
			//1.5.4 글자 위치를 증가시킨다.
			letterIndex++;
		}
		//1.6 줄의 위치를 증가시킨다.
		previousRowIndex++;
		previousLetterIndex = 0;
	}
}

//왼쪽으로 선택하다
void SelectText::DoPrevious(Long previousRowIndex, Long previousLetterIndex, Long currentRowIndex,
	Long currentLetterIndex)
{
	Glyph* row = 0;
	Glyph* letter = 0;
	Long letterCount = 0;
	Long letterIndex = 0;
	//1. 이동하기 전의 줄의 위치가 이동한 후의 줄의 위치보다 작거나 같은 동안 반복한다.
	while (currentRowIndex <= previousRowIndex)
	{
		//1.1 줄을 구한다.
		row = this->notepadForm->note->GetAt(currentRowIndex);
		//1.2 이동하기 전 줄의 위치값이 이동한 후 줄의 위치값과 같지 않으면
		if (currentRowIndex != previousRowIndex)
		{
			//1.2.1 줄의 글자 개수를 대입한다.
			letterCount = row->GetLength();
		}
		//1.3  이동하기 전 줄의 위치값이 이동한 후 줄의 위치값과 같으면
		else
		{
			//1.3.1 이동한 후 글자 위치를 대입한다.
			letterCount = previousLetterIndex;
		}
		//1.4 글자 위치를 원위치시킨다.
		letterIndex = currentLetterIndex;
		//1.5 글자 위치가 letterCount보다 작은동안 반복한다.
		while (letterIndex < letterCount)
		{
			//1.5.1 글자를 구한다.
			letter = row->GetAt(letterIndex);
			//1.5.2 글자가 선택이 안되어있으면
			if (letter->IsSelected() == false)
			{
				///1.5.2.1 글자를 선택이 된 상태로 바꿔준다.
				letter->Select(true);
			}
			//1.5.3 글자가 선택이 되어 있으면
			else
			{
				//1.5.3.1 글자를 선택이 안된 상태로 바꿔준다.
				letter->Select(false);
			}
			//1.5.4 글자 위치를 증가시킨다.
			letterIndex++;
		}
		//1.6 줄의 위치를 증가시킨다.
		currentRowIndex++;
		currentLetterIndex = 0;
	}
}

//선택을 해제하다
void SelectText::Undo()
{
	//1. 선택된 범위를 구한다.
	Long startingRowPos = 0;
	Long startingLetterPos = 0;
	Long endingRowPos = 0;
	Long endingLetterPos = 0;
	this->notepadForm->note->CalculateSelectedRange(&startingRowPos,
		&startingLetterPos, &endingRowPos, &endingLetterPos);
	Long rowIndex = startingRowPos;
	Long index = startingLetterPos;
	Long rowCount = this->notepadForm->note->GetLength();
	Long letterCount = 0;
	Long letterIndex = 0;
	Glyph* row = 0;
	Glyph* letter = 0;
	bool isSelected = true;
	//2. 선택이 시작된 줄의 위치가 선택이 끝나는 줄의 위치보다 작거나 같은동안 반복한다.
	while (rowIndex <= endingRowPos && isSelected == true)
	{
		//2.1 줄을 구한다.
		row = this->notepadForm->note->GetAt(rowIndex);
		//2.2 줄의 글자개수를 구한다.
		letterCount = row->GetLength();
		//2.3 글자 위치를 원위치시킨다.
		letterIndex = index;
		//2.4 글자위치가 글자개수보다 작은동안 반복한다.
		while (letterIndex < letterCount && isSelected == true)
		{
			//2.4.1 글자를 구한다.
			letter = row->GetAt(letterIndex);
			//2.4.2 글자가 선택이 되어있으면
			if (letter->IsSelected() == true)
			{
				//2.4.2.1 현재 글자를 선택이 안된 상태로 바꿔준다.
				letter->Select(false);
			}
			//2.4.3 글자가 선택이 안되어있으면
			else
			{
				isSelected = false;
			}
			//2.4.4 글자 위치를 증가시킨다.
			letterIndex++;
		}
		//2.5 줄의 위치를 증가시킨다.
		rowIndex++;
		index = 0;
	}
}

//소멸자
SelectText::~SelectText()
{

}