#include "CtrlRightArrowKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "DummyRow.h"

//디폴트생성자
CtrlRightArrowKeyAction::CtrlRightArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlRightArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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
	//2. 메모장에서 현재 줄의 위치를 구한다.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//3. 메모장에서 현재 글자의 위치를 구한다.
	Long currentLetterIndex = this->notepadForm->note->GetAt(currentRowIndex)->GetCurrent();
	//4. 자동개행이 진행 중이 아니면
	if (this->notepadForm->isRowAutoChanging == false)
	{
		//4.1 이동하기 전에 줄의 위치를 구한다.
		Long previousRowIndex = currentRowIndex;
		//4.2 이동하기 전에 글자의 위치를 구한다.
		Long previousLetterIndex = currentLetterIndex;
		//4.3 노트에서 단어단위로 오른쪽으로 이동한다.
		currentLetterIndex = this->notepadForm->note->NextWord();
		//4.4 이동한 후에 줄의 위치를 구한다.
		currentRowIndex = this->notepadForm->note->GetCurrent();
		//4.5 메모장의 현재 줄을 이동한 후의 줄로 변경한다.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	}
	//5. 자동개행이 진행 중이면
	else
	{
		//5.1 메모장의 현재 줄에서 처음 글자부터 현재 글자까지의 content를 구한다.
		Long i = 0;
		Glyph* letter = 0;
		string letterContent = "";
		string currentLetters = "";
		while (i < currentLetterIndex)
		{
			//5.4.1 글자를 구한다.
			letter = this->notepadForm->current->GetAt(i);
			letterContent = letter->GetContent();
			//5.4.2 줄의 content에 더해준다.
			currentLetters += letterContent;
			i++;
		}
		//5.2 메모장의 현재 줄에서 처음글자부터 현재 글자까지의 content의 길이를 구한다.
		Long currentLettersLength = currentLetters.length();
		//5.3 현재 줄을 구한다.
		Glyph* row = this->notepadForm->note->GetAt(currentRowIndex);
		//5.4 현재 줄의 전체 content를 구한다.
		i = 0;
		string realRowContent = "";
		while (i < row->GetLength())
		{
			//5.4.1 글자를 구한다.
			letter = row->GetAt(i);
			letterContent = letter->GetContent();
			//5.4.2 줄의 content에 더해준다.
			realRowContent += letterContent;
			i++;
		}
		//5.5 다음 줄을 구한다.
		Long nextRowIndex = currentRowIndex + 1;
		Glyph* nextRow = this->notepadForm->note->GetAt(nextRowIndex);
		//5.6 다음 줄의 위치가 줄의 개수보다 작은동안 그리고 다음 줄이 가짜 줄인 동안 반복한다.
		string dummyRowContent = "";
		while (nextRowIndex < this->notepadForm->note->GetLength() &&
			dynamic_cast<DummyRow*>(nextRow))
		{
			//5.6.1 가짜줄의 content를 구한다.
			i = 0;
			dummyRowContent = "";
			while (i < nextRow->GetLength())
			{
				//5.6.1.1 글자를 구한다.
				letter = nextRow->GetAt(i);
				letterContent = letter->GetContent();
				//5.6.1.2 줄의 content에 더해준다.
				dummyRowContent += letterContent;
				i++;
			}
			//5.6.2 가짜줄의 content를 진짜 줄의 content에 더해준다.
			realRowContent += dummyRowContent;
			//5.6.3 다음 줄로 이동한다.
			nextRowIndex++;
			//5.6.4 다음 줄을 구한다.
			nextRow = this->notepadForm->note->GetAt(nextRowIndex);
		}
		//5.7 가짜 줄을 더한 진짜 줄의 content 길이를 구한다.
		Long rowContentLength = realRowContent.length();
		Long rowLastContentLength = rowContentLength - 1;
		i = currentLettersLength;
		Long j = 0;
		//5.8 현재 글자 위치가 현재 줄의 마지막 글자 위치면(현재 줄은 진짜 줄 또는 가짜 줄 하지만
		//다음 줄은 무조건 진짜 줄임. 그래서 현재 줄의 마지막 글자 위치에 있으면 다음 줄(진짜 줄)로 이동함.)
		if (i == rowContentLength)
		{
			//5.8.1 다음 줄로 이동한다.
			currentRowIndex++;
			//5.8.2 다음 줄의 처음글자위치로 이동한다.
			currentLetterIndex = 0;
			//5.8.3 줄을 구한다.
			row = this->notepadForm->note->GetAt(currentRowIndex);
			//5.8.4 줄의 content를 구한다.
			j = 0;
			realRowContent = "";
			while (j < row->GetLength())
			{
				//5.8.4.1 글자를 구한다.
				letter = row->GetAt(j);
				letterContent = letter->GetContent();
				//5.8.4.2 줄의 content에 더해준다.
				realRowContent += letterContent;
				j++;
			}
			//5.8.5 다음 줄을 구한다.
			nextRowIndex = currentRowIndex + 1;
			nextRow = this->notepadForm->note->GetAt(nextRowIndex);
			//5.8.6 다음 줄의 위치가 줄의 개수보다 작은동안 그리고 다음 줄이 가짜 줄인 동안 반복한다.
			dummyRowContent = "";
			while (nextRowIndex < this->notepadForm->note->GetLength() &&
				dynamic_cast<DummyRow*>(nextRow))
			{
				//5.8.6.1 가짜줄의 content를 구한다.
				j = 0;
				dummyRowContent = "";
				while (j < nextRow->GetLength())
				{
					//5.8.6.1.1 글자를 구한다.
					letter = nextRow->GetAt(j);
					letterContent = letter->GetContent();
					//5.8.6.1.2 줄의 content에 더해준다.
					dummyRowContent += letterContent;
					j++;
				}
				//5.8.8.2 가짜줄의 content를 진짜 줄의 content에 더해준다.
				realRowContent += dummyRowContent;
				//5.8.8.3 다음 줄로 이동한다.
				nextRowIndex++;
				//5.8.8.4 다음 줄을 구한다.
				nextRow = this->notepadForm->note->GetAt(nextRowIndex);
			}
			//5.8.7 줄의 content 길이를 구한다.
			rowContentLength = realRowContent.length();
			i = 0;
			letterContent = realRowContent[i];//한글자를 담을 공간
			//5.8.10 줄의 글자가 탭문자거나 스페이스문자이면
			if (letterContent == "\t" || letterContent == " ")
			{
				//5.8.10.1 마지막 글자 위치를 구한다.
				rowLastContentLength = rowContentLength - 1;
				//5.8.10.2 마지막 글자위치보다 작은동안 그리고 탭문자인동안 반복한다.
				while (i < rowLastContentLength && letterContent == "\t")
				{
					//5.8.10.2.1 현재 글자위치를 증가시킨다.
					currentLetterIndex++;
					i++;
					//5.8.10.2.2 글자를 저장한다.
					letterContent = realRowContent[i];
				}
				//5.8.10.3 마지막 글자위치와 같고 탭문자이면
				if (i == rowLastContentLength && letterContent == "\t")
				{
					//5.8.10.3.1 현재 글자위치를 증가시킨다.
					currentLetterIndex++;
					i++;
				}
				//5.8.10.4 마지막 글자위치보다 작은동안 그리고 스페이스문자인동안 반복한다.
				while (i < rowLastContentLength && letterContent == " ")
				{
					//5.8.10.4.1 현재 글자위치를 증가시킨다.
					currentLetterIndex++;
					i++;
					//5.8.10.4.2 글자를 저장한다.
					letterContent = realRowContent[i];
				}
				//5.8.10.5 마지막 글자위치와 같고 스페이스문자이면
				if (i == rowLastContentLength && letterContent == " ")
				{
					//5.8.10.5.1 현재 글자위치를 증가시킨다.
					currentLetterIndex++;
					i++;
				}
			}
		}
		//5.9 그게 아니면
		else
		{
			//5.9.1 한글이면
			if ((realRowContent[i] & 0x80))//한글이면(2byte문자)
			{
				//5.9.1.1 2byte로 저장한다.
				letterContent = realRowContent[i];
				i++;
				letterContent += realRowContent[i];
			}
			//5.9.2 한글이 아니면
			else
			{
				//5.9.2.1 1byte로 저장한다
				letterContent = realRowContent[i];
			}
			//5.9.3 마지막 글자위치보다 작은동안 그리고 탭문자와 스페이스문자가 아닌동안 반복한다.
			while (i < rowLastContentLength && letterContent != " " && letterContent != "\t")
			{
				//5.9.3.1 현재 글자위치를 증가시킨다.
				currentLetterIndex++;
				i++;
				//5.9.3.2 줄에서 i번째 읽을 글자가 한글이면
				if ((realRowContent[i] & 0x80))//한글이면(2byte문자)
				{
					//5.9.3.2.1 2byte로 저장한다.
					letterContent = realRowContent[i];
					i++;
					letterContent += realRowContent[i];
				}
				//5.9.3.3 한글이 아니면
				else
				{
					//5.9.3.3.1 1byte로 저장한다
					letterContent = realRowContent[i];
				}
			}
			//5.9.4 마지막 글자위치와 같고, 탭문자와 스페이스 문자가 아니면
			if (i == rowLastContentLength && letterContent != " " && letterContent != "\t")
			{
				//5.9.4.1 현재 글자위치를 증가시킨다.
				currentLetterIndex++;
				i++;
			}
			//5.9.5 마지막 글자위치보다 작은동안 그리고 탭문자인동안 반복한다.
			while (i < rowLastContentLength && letterContent == "\t")
			{
				//5.9.5.1 현재 글자위치를 증가시킨다.
				currentLetterIndex++;
				i++;
				//5.9.5.2 글자를 저장한다.
				letterContent = realRowContent[i];
			}
			//5.9.6 마지막 글자위치와 같고 탭문자이면
			if (i == rowLastContentLength && letterContent == "\t")
			{
				//5.9.6.1 현재 글자위치를 증가시킨다.
				currentLetterIndex++;
				i++;
			}
			//5.9.7 마지막 글자위치보다 작은동안 그리고 스페이스문자인동안 반복한다.
			while (i < rowLastContentLength && letterContent == " ")
			{
				//5.9.7.1 현재 글자위치를 증가시킨다.
				currentLetterIndex++;
				i++;
				//5.9.7.2 글자를 저장한다.
				letterContent = realRowContent[i];
			}
			//5.9.8 마지막 글자위치와 같고 스페이스문자이면
			if (i == rowLastContentLength && letterContent == " ")
			{
				//5.9.8.1 현재 글자위치를 증가시킨다.
				currentLetterIndex++;
				i++;
			}
		}
		//5.10 현재 줄을 구한다.
		Glyph* currentRow = this->notepadForm->note->GetAt(currentRowIndex);
		//5.11 현재 줄의 content를 구한다.
		j = 0;
		string currentRowContent = "";
		while (j < currentRow->GetLength())
		{
			//5.11.1 글자를 구한다.
			letter = currentRow->GetAt(j);
			letterContent = letter->GetContent();
			//5.11.2 줄의 content에 더해준다.
			currentRowContent += letterContent;
			j++;
		}
		//5.12 현재 줄의 content의 length를 구한다.
		Long currentRowContentLength = currentRowContent.length();
		//5.13 i가 현재 줄의 content의 length보다 크거나 같은동안 반복한다.
		//아까 앞에서 부분자동개행을 취소하는 것처럼 가짜줄의 content들을 진짜줄의 content에
		//추가시켰지만 사실상 현재 메모장에서는 자동개행이 되어 있는 상태이기 때문에
		//자동개행이 된 메모장에서 찾을 문자열을 찾아서 선택을 해야하기 때문에 아까 합쳤던 각각의 
		//가짜줄들을 다시 개별적으로 구해서 합친 줄에서 읽은 글자위치 i와 비교해서 현재 줄의 위치를
		//다시 구해준다.
		string nextRowContent = "";
		while (i > currentRowContentLength)
		{
			//5.13.1 현재 줄의 위치를 증가시킨다.
			//자동개행이 취소된 상태에서 해당 문자열을 찾았는데 최종적으로 읽은 글자가
			//현재 개별적인 줄의 길이보다 길기 때문에 다음 줄로 넘어가서 다시 비교해야한다.
			currentRowIndex++;
			//5.13.2 현재 글자위치도 자동개행이 취소된 상태의 글자위치기 때문에 현재 글자 
			//위치가 현재 줄의 개수보다 크기 때문에 그만큼 빼서 자동개행이 된 상태로 조정한다
			//앞에서 currentLetterIndex++로 인해 1이 증가된 상태이기 때문에 +1을 따로 안해줘도됨!
			currentLetterIndex -= currentRow->GetLength();
			//5.13.3 현재 줄의 다음줄을 구한다.
			currentRow = this->notepadForm->note->GetAt(currentRowIndex);
			//5.13.4 다음 줄의 content를 저장한다.
			j = 0;
			nextRowContent = "";
			while (j < currentRow->GetLength())
			{
				//5.13.4.1 글자를 구한다.
				letter = currentRow->GetAt(j);
				letterContent = letter->GetContent();
				//5.13.4.2 줄의 content에 더해준다.
				nextRowContent += letterContent;
				j++;
			}
			//5.13.5 다음 줄의 content를 현재 줄의 content에 더해준다.(누적)
			currentRowContent += nextRowContent;
			//5.13.6 누적된 줄의 content의 길이를 구한다.
			currentRowContentLength = currentRowContent.length();
		}
		//5.14 현재 줄의 위치와 글자위치를 다시 조정해준다.
		currentRowIndex = this->notepadForm->note->Move(currentRowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
		currentLetterIndex = this->notepadForm->current->Move(currentLetterIndex);
	}
}

//소멸자
CtrlRightArrowKeyAction::~CtrlRightArrowKeyAction()
{

}