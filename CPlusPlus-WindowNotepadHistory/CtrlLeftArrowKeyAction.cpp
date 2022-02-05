#include "CtrlLeftArrowKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "DummyRow.h"

//디폴트생성자
CtrlLeftArrowKeyAction::CtrlLeftArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlLeftArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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
		//4.3 노트에서 단어단위로 왼쪽으로 이동한다.
		Long currentLetterIndex = this->notepadForm->note->PreviousWord();
		//4.4 이동한 후에 줄의 위치를 구한다.
		Long currentRowIndex = this->notepadForm->note->GetCurrent();
		//4.5 현재 줄을 이동한 후의 줄로 변경한다.
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
			//5.1.1 글자를 구한다.
			letter = this->notepadForm->current->GetAt(i);
			letterContent = letter->GetContent();
			//5.1.2 줄의 content에 더해준다.
			currentLetters += letterContent;
			i++;
		}
        //5.2 메모장의 현재 줄에서 처음글자부터 현재 글자까지의 content의 길이를 구한다.
        Long currentLettersLength = currentLetters.length();
        //5.3 현재 줄을 구한다.
        Glyph* row = this->notepadForm->note->GetAt(currentRowIndex);
        //5.4 줄의 content를 구한다.
        i = 0;
        string rowContent = "";
        while (i < currentLetterIndex)
        {
            //5.4.1 글자를 구한다.
            letter = row->GetAt(i);
            letterContent = letter->GetContent();
            //5.4.2 줄의 content에 더해준다.
            rowContent += letterContent;
            i++;
        }
        //5.5 현재 줄이 가짜 줄이면(현재 줄의 위치가 0이면 가짜 줄일 수 X)
        if (dynamic_cast<DummyRow*>(row))
        {
            //5.5.1 이전 줄로 이동한다.
            Long previousRowIndex = currentRowIndex - 1;
            //5.5.2 이전 줄을 구한다
            Glyph *previousRow = this->notepadForm->note->GetAt(previousRowIndex);
            //5.5.3 이전 줄이 가짜줄인동안 반복한다.
            string previousRowContent = "";
            while (dynamic_cast<DummyRow*>(previousRow))
            {
                //5.5.3.1 이전 줄의 content를 구한다.
                i = 0;
                previousRowContent = "";
                while (i < previousRow->GetLength())
                {
                    //5.5.3.1.1 글자를 구한다.
                    letter = previousRow->GetAt(i);
                    letterContent = letter->GetContent();
                    //5.5.3.1.2 줄의 content에 더해준다.
                    previousRowContent += letterContent;
                    i++;
                }
                //5.5.3.2 이전 줄의 content에 현재 줄의 cotent를 더해준다.(누적)
                previousRowContent += rowContent;
                //5.5.3.3 더해준 값을 옮겨 저장한다.(반복 구조에서 제대로 누적시키기 위해서)
                rowContent = previousRowContent;
                //5.5.3.4 메모장의 현재 글자위치에 이전 줄의 글자개수를 더해준다.
                currentLetterIndex += previousRow->GetLength();
                //5.5.3.5 이전으로 이동한다.
                previousRowIndex--;
                //5.5.3.6 이전 줄을 구한다.
                previousRow = this->notepadForm->note->GetAt(previousRowIndex);
            }
            //이전으로 이동했을 때 진짜 줄이 나와서 반복구조를 나왔기 때문에 지금 이전 줄이 진짜 줄이다.
            //따라서 이전 줄(진짜 줄)의 content를 구해서 아까 반복구조에서 구했던 가짜줄의 content를 더해준다
            //5.5.4 진짜 줄의 위치를 현재 줄의 위치로 설정한다.
            currentRowIndex = previousRowIndex;
            //5.5.5 진짜 줄의 content를 구한다.
            i = 0;
            previousRowContent = "";
            while (i < previousRow->GetLength())
            {
                //5.5.5.1 글자를 구한다.
                letter = previousRow->GetAt(i);
                letterContent = letter->GetContent();
                //5.5.5.2 줄의 content에 더해준다.
                previousRowContent += letterContent;
                i++;
            }
            //5.5.6 진짜 줄의 content를 누적시킨다.
            previousRowContent += rowContent;
            //5.5.7 더해준 값을 옮겨 저장한다.(반복 구조에서 제대로 누적시키기 위해서)
            rowContent = previousRowContent;
            //5.5.8 메모장의 현재 글자위치에 이전 줄의 글자개수를 더해준다.
            currentLetterIndex += previousRow->GetLength();
            //5.5.9 현재 줄의 길이를 구한다.(자동개행 취소된 마지막 글자위치에서 시작)
            currentLettersLength = rowContent.length();
        }
        //5.6 진짜 줄의 위치를 저장한다.
        Long realRowIndex = currentRowIndex;
        string koreanCompositionTemp;//역순으로 거슬러 올라가기 때문에 역순으로 한글을 조립하면 한글이 제대로
        //조립되지 않기 때문에 한글을 임시로 저장하고 원래 순서대로 조립할 임시 저장소가 필요함.
        //5.7 현재 글자 위치가 0보다 크면(현재 글자 위치가 0이면 단어단위로 이동할 수 X)
        if (currentLetterIndex > 0)
        {
            //5.7.1 줄에서 읽어야 할 글자 한 칸 만큼 감소시킨다.
            i = currentLettersLength - 1;
            //currentLetterIndex--;
            //5.7.2 줄에서 i번째 읽을 글자가 한글이면
            if ((rowContent[i] & 0x80))//한글이면(2byte문자)
            {
                //5.8.1.1 2byte로 저장한다.
                letterContent = rowContent[i];
                //역순으로 거슬러 가기 때문에 제대로 한글을 조립하기 위해서 임시저장을 해야함.
                koreanCompositionTemp = letterContent;
                i--;
                letterContent = rowContent[i];
                letterContent += koreanCompositionTemp;
            }
            //5.8.2 한글이 아니면
            else
            {
                //5.8.2.1 1byte로 저장한다
                letterContent = rowContent[i];
            }
            //5.8.3 현재 글자 위치가 1보다 크고 읽은 글자가 스페이스(공백)문자인동안 반복한다.
            while (currentLetterIndex > 1 && letterContent == " ")
            {
                //5.8.3.1 현재 글자위치를 감소시킨다.
                currentLetterIndex--;
                i--;
                //5.8.3.2 글자를 저장한다.
                letterContent = rowContent[i];
            }
            //5.8.4 현재 글자 위치가 1이고 읽은 글자가 스페이스(공백)문자이면
            if (currentLetterIndex == 1 && letterContent == " ")
            {
                //5.8.4.1 현재 글자위치를 감소시킨다.
                currentLetterIndex--;
                i--;
            }
            //5.8.5 현재 글자 위치가 1보다 크고 읽은 글자가 탭문자인동안 반복한다.
            while (currentLetterIndex > 1 && letterContent == "\t")
            {
                //5.8.5.1 현재 글자위치를 감소시킨다.
                currentLetterIndex--;
                i--;
                //5.8.5.2 글자를 저장한다.
                letterContent = rowContent[i];
            }
            //5.8.6 현재 글자 위치가 1이고 읽은 글자가 탭문자이면
            if (currentLetterIndex == 1 && letterContent == "\t")
            {
                //5.8.6.1 현재 글자위치를 감소시킨다.
                currentLetterIndex--;
                i--;
            }

            if (currentLetterIndex > 1 && letterContent != " " && letterContent != "\t")
            {
                //5.8.7.1 현재 읽은 글자가 한글이면
                if ((rowContent[i] & 0x80))//한글이면(2byte문자)
                {
                    //5.8.7.2.1 2byte로 저장한다.
                    letterContent = rowContent[i];
                    //역순으로 거슬러 가기 때문에 제대로 한글을 조립하기 위해서 임시저장을 해야함.
                    koreanCompositionTemp = letterContent;
                    i--;
                    letterContent = rowContent[i];
                    letterContent += koreanCompositionTemp;
                }
                //5.8.7.2 한글이 아니면
                else
                {
                    //5.8.7.2.1 1byte로 저장한다
                    letterContent = rowContent[i];
                }
                //5.8.7.3 현재 글자 위치를 감소시킨다.
                //currentLetterIndex--;
                //i--;
            }

            //5.8.7 현재 글자 위치가 1보다 크고 읽은 글자가 스페이스(공백)문자 그리고
            //탭문자가 아닌동안 반복한다.
            while (currentLetterIndex > 1 && letterContent != " " && letterContent != "\t")
            {
                //5.8.7.3 현재 글자 위치를 감소시킨다.
                currentLetterIndex--;
                i--;
                //5.8.7.1 현재 읽은 글자가 한글이면
                if ((rowContent[i] & 0x80))//한글이면(2byte문자)
                {
                    //5.8.7.2.1 2byte로 저장한다.
                    letterContent = rowContent[i];
                    //역순으로 거슬러 가기 때문에 제대로 한글을 조립하기 위해서 임시저장을 해야함.
                    koreanCompositionTemp = letterContent;
                    i--;
                    letterContent = rowContent[i];
                    letterContent += koreanCompositionTemp;
                }
                //5.8.7.2 한글이 아니면
                else
                {
                    //5.8.7.2.1 1byte로 저장한다
                    letterContent = rowContent[i];
                }
            }
            //5.8.8 현재 캐럿의 가로 위치가 1이고 읽은 글자가 스페이스(공백) 문자와 탭문자가 아니면
            if (currentLetterIndex == 1 && letterContent != " " && letterContent != "\t")
            {
                //5.8.8.1 현재 읽은 글자가 한글이면
                if ((rowContent[i] & 0x80))//한글이면(2byte문자)
                {
                    //5.8.8.2.1 2byte로 저장한다.
                    letterContent = rowContent[i];
                    //역순으로 거슬러 가기 때문에 제대로 한글을 조립하기 위해서 임시저장을 해야함.
                    koreanCompositionTemp = letterContent;
                    i--;
                    letterContent = rowContent[i];
                    letterContent += koreanCompositionTemp;
                }
                //5.8.8.2 한글이 아니면
                else
                {
                    //5.8.8.2.1 1byte로 저장한다
                    letterContent = rowContent[i];
                }
                //5.8.8.3 현재 글자 위치를 감소시킨다.
                currentLetterIndex--;
                i--;
            }
            //5.8.9 현재 글자 위치가 0이면
            if (currentLetterIndex == 0)
            {
                //5.8.9.1 글자를 저장한다.
                //letterContent = rowContent[i];
                //5.8.9.2 현재 줄의 첫번째 글자가 스페이스 또는 탭문자이면
                if (letterContent == "\t" || letterContent == " ")
                {
                    //5.8.9.2.1 현재 줄의 위치를 1만큼 감소시킨다.(이전 줄로 이동시킨다.)
                    currentRowIndex--;
                    //5.8.9.2.2 현재 줄의 위치가 underflow이면
                    if (currentRowIndex < 0)
                    {
                        //5.8.9.2.2.1 현재 줄의 위치를 최소값으로 변경한다.
                        currentRowIndex = 0;
                    }
                    //5.8.9.2.3 현재 줄의 위치가 underflow가 아니면(this->current >=0)
                    else
                    {
                        //5.8.9.2.3.1 현재 캐럿의 가로 위치를 마지막으로 이동시킨다.
                        currentRowIndex = this->notepadForm->note->Move(currentRowIndex);
                        this->notepadForm->current = this->notepadForm->
                            note->GetAt(currentRowIndex);
                        currentLetterIndex = this->notepadForm->current->Last();
                    }
                }
            }
        }
        //5.9 현재 글자 위치가 제일 처음이면(0이면)
        else if (currentLetterIndex == 0)
        {
            //5.9.1 현재 줄의 위치를 1만큼 감소시킨다.(이전 줄로 이동시킨다.)
            currentRowIndex--;
            //5.9.2 현재 줄의 위치가 underflow이면
            if (currentRowIndex < 0)
            {
                //5.9.2.1 현재 줄의 위치를 최소값으로 변경한다.
                currentRowIndex = 0;
            }
            //5.9.3 현재 줄의 위치가 underflow가 아니면(this->current >=0)
            else
            {
                //5.9.3.1 현재 캐럿의 가로 위치를 마지막으로 이동시킨다.
                currentRowIndex = this->notepadForm->note->Move(currentRowIndex);
                this->notepadForm->current = this->notepadForm->
                   note->GetAt(currentRowIndex);
                currentLetterIndex = this->notepadForm->current->Last();
            }
        }
        //5.10 메모장에서 현재 줄을 구한다.(진짜 줄)
        Glyph* currentRow = this->notepadForm->note->GetAt(currentRowIndex);
        //5.11 현재 줄의 content를 구한다.
        Long k = 0;
        string currentRowContent = "";
        while (k < currentRow->GetLength())
        {
            //5.11.1 글자를 구한다.
            letter = currentRow->GetAt(k);
            letterContent = letter->GetContent();
            //5.11.2 줄의 content에 더해준다.
            currentRowContent += letterContent;
            k++;
        }
        //5.12 현재 줄의 length를 구한다.
        Long currentRowContentLength = currentRowContent.length();
        //5.13 i가 현재 줄의 length보다 크거나 같은동안 반복한다.
        while (i >= currentRowContentLength)
        {
            //5.13.1 현재 줄의 길이를 뺀다.
            i -= currentRowContentLength;
            //5.13.2 현재 줄의 개수를 뺀다.
            currentLetterIndex -= currentRow->GetLength();
            //5.13.3 다음 줄로 이동한다.
            currentRowIndex++;
            //5.13.4 다음 줄을 구한다.
            currentRow = this->notepadForm->note->GetAt(currentRowIndex);
            //5.13.5 다음 줄의 content를 구한다.
            k = 0;
            currentRowContent = "";
            while (k < currentRow->GetLength())
            {
                //5.13.5.1 글자를 구한다.
                letter = currentRow->GetAt(k);
                letterContent = letter->GetContent();
                //5.13.5.2 줄의 content에 더해준다.
                currentRowContent += letterContent;
                k++;
            }
            //5.13.6 다음 줄의 content의 length를 구한다.
            currentRowContentLength = currentRowContent.length();
        }
        //5.14 현재 줄의 위치와 글자위치를 다시 조정해준다.
        currentRowIndex = this->notepadForm->note->Move(currentRowIndex);
        this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
        currentLetterIndex = this->notepadForm->current->Move(currentLetterIndex);
        //5.15 현재 줄의 위치가 현재 줄의 마지막 글자 위치이면
        if (currentLetterIndex == this->notepadForm->current->GetLength())
        {
            //15.1 현재 줄을 구한다.
            Glyph* currentRow = this->notepadForm->current;
            //15.2 다음 줄을 구한다.
            Glyph* nextRow = this->notepadForm->note->GetAt(currentRowIndex + 1);
            //15.3 다음 줄이 진짜 줄이고 현재 줄이 가짜 줄이면
            if (!dynamic_cast<DummyRow*>(nextRow) && dynamic_cast<DummyRow*>(currentRow))
            {
                //15.3.1 줄이 이전으로 이동했기 때문에 캐럿의 현재 위치를 마지막으로 변경한다.
                currentLetterIndex = this->notepadForm->current->Last();
            }
            //15.4 다음 줄이 진짜 줄이고 현재 줄이 진짜 줄이면
            else if (!dynamic_cast<DummyRow*>(nextRow) && !dynamic_cast<DummyRow*>(currentRow))
            {
                //15.4.1 줄이 이전으로 이동했기 때문에 캐럿의 현재 위치를 마지막으로 변경한다.
                currentLetterIndex = this->notepadForm->current->Last();
            }
            //15.5 그 이외에는
            else
            {
                //15.5.1 현재 줄의 위치와 글자위치를 다시 조정해준다.
                currentRowIndex = this->notepadForm->note->Move(currentRowIndex + 1);
                this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
                currentLetterIndex = this->notepadForm->current->First();
            }
        }
	}
}

//소멸자
CtrlLeftArrowKeyAction::~CtrlLeftArrowKeyAction()
{

}