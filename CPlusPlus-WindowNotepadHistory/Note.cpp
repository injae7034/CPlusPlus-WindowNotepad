#include "Note.h"
#include "Letter.h"
#include "GlyphVisitor.h"
#include "SingleByteLetter.h"
#include "DoubleByteLetter.h"
#include "DummyRow.h"

//디폴트생성자
Note::Note(Long capacity)
	:Composite(capacity)
{

}

//복사생성자
Note::Note(const Note& source)
	:Composite(source)
{

}

//치환연산자
Note& Note::operator=(const Note& source)
{
	//this->Composite::operator=(source);
	Composite::operator=(source);

	return *this;
}

//소멸자
Note::~Note()
{

}

//Prototype Patter Clone
Glyph* Note::Clone()
{
	return new Note(*this);
}

//GetContent
string Note::GetContent()
{
	//1. 마지막 줄전까지 반복한다.
	string content;
	Long rowIndex = 0;
	Long lastRowIndex = this->length - 1;
	while (rowIndex < lastRowIndex)
	{
		content += this->glyphs.GetAt(rowIndex)->GetContent();
		content += '\n';
		rowIndex++;
	}
	//2. 마지막줄을 처리한다.
	//(마지막 줄만 따로 처리하는 이유는 마지막 줄을 따로 처리안하고 위의 반복문에 넣어주면
	//개행문자가 같이 입력되서 마지막 줄 다음에 한 줄이 더생기기 때문이다.)
	if (rowIndex == lastRowIndex)
	{
		content += this->glyphs.GetAt(rowIndex)->GetContent();
	}

	return content;
}

//캐럿의 현재 줄의 위치를 마지막으로 이동시키기
Long Note::Last()
{
	//1. 캐럿의 현재 세로 위치를 노트의 맨 밑으로 이동시킨다.
	this->current = this->length - 1;
	//2. 캐럿의 현재 가로 위치를 출력한다.
	return this->current;
}

//캐럿의 현재 줄의 위치를 한 줄 다음(아래)으로 이동시키기
Long Note::Next()
{
	//이동시킬 때는 항상 먼저 이동을 시키고, 그 다음에 overflow이면 최대값으로 변경해주자.
	//1. 캐럿의 현재 세로 위치를 한 줄 다음으로 이동시킨다.
	this->current++;
	//2. 캐럿의 현재 세로 위치가 overflow이면
	if (this->current >= this->length)
	{
		//2.1 캐럿의 현재 세로 위치를 최대값으로 변경한다.
		this->current = this->length - 1;
	}
	//3. 캐럿의 현재 세로 위치를 출력한다.
	return this->current;
}

//Note기준에 맞게 Add, Remove 재정의(오버라이딩)
Long Note::Add(Glyph* glyph)
{
	//1. 추가할 줄의 주소를 입력받는다.
	//2. 사용량이 할당량보다 작으면
	Long index;
	if (this->length < this->capacity)
	{
		//2.1 비어있는 공간에 순차적으로 저장한다.
		index = this->glyphs.Store(this->length, glyph);
	}
	//3. 그렇지 않으면
	else
	{
		//3.1 공간을 한 칸 늘려서 마지막에 저장한다.
		index = this->glyphs.AppendFromRear(glyph);
		//3.2 할당량을 증가시킨다.
		this->capacity++;
	}
	//4. 사용량을 증가시킨다.
	this->length++;
	//5. 캐럿의 현재 세로 위치(현재 줄의 위치)를 저장한다.(Note기준)
	this->current = index;
	//6. 현재 줄의 위치를 출력한다.(Note기준)
	return index;
}

//Add(Insert)
Long Note::Add(Long index, Glyph* glyph)
{
	//1. 끼워 쓸 줄의 index와 줄의 주소를 입력받는다.
	//2. 줄을 배열 요소의 index번쨰에 끼워 넣는다.
	index = this->glyphs.Insert(index, glyph);
	//3. 할당량을 증가시킨다.
	this->capacity++;
	//4. 사용량을 증가시킨다.
	this->length++;
	//5. 캐럿의 현재 세로 위치(현재 줄의 위치)를 저장한다.(Note기준)
	this->current = index;
	//6. 현재 줄의 위치를 출력한다.(Note기준)
	return index;
}

Long Note::Remove(Long index)
{
	//1. 지울 줄의 위치를 입력받는다.
	//2. 줄을 지우고 나면 지우는 줄의 위치보다 이전 위치가 현재 위치가 되기 때문에 
	//현재 캐럿의 세로 위치를 index-1로 저장한다.
	this->current = index - 1;
	//3. 해당배열요소가 주소를 저장하고 있기 때문에 해당배열요소의 힙에 할당된 내용(줄)을 먼저 지워야함.
	Glyph* glyph = this->glyphs[index];
	if (glyph != 0)
	{
		delete glyph;
	}
	//4. 해당위치의 배열요소의 힙에 할당된 내용(줄)을 지웠으니 이제 그 주소를 지운다.
	index = this->glyphs.Delete(index);
	//5. 할당량을 감소시킨다.
	this->capacity--;
	//6. 사용량을 감소시킨다.
	this->length--;
	//7. index를 출력한다.
	return index;
}

//노트에서 캐럿을 단어단위로 오른쪽으로 이동시키기
Long Note::NextWord()
{
	Long index = 0;
	string letter;
	//1. 캐럿의 현재 가로위치가 현재줄의 글자개수와 같은 경우
	//(캐럿이 현재 줄의 마지막 글자 뒤에 위치하고 있는 경우이고 이 경우에만 다음 줄로 이동함!)
	//즉, 현재줄에서 다음줄로 이동하는 경우(줄에서 줄로 이동하는 경우) 처리하기
	if (this->GetAt(this->current)->GetCurrent()
		== this->GetAt(this->current)->GetLength())
	{
		//1.1 캐럿의 현재 세로 위치를 1만큼 증가시킨다.
		this->current++;
		//1.2 캐럿의 현재 세로 위치가 overflow이면
		if (this->current >= this->length)
		{
			//1.2.1 캐럿의 현재 세로 위치를 최대값으로 변경한다.
			this->current = this->length - 1;
			//1.2.2 캐럿의 현재 가로 위치를 마지막으로 변경한다.
			index = this->GetAt(this->current)->Last();
		}
		//1.3 캐럿의 현재 세로 위치가 overflow가 아니면
		else
		{
			//1.3.1 캐럿이 다음 줄로 이동했기 때문에 캐럿의 현재 가로 위치를 0으로 변경한다.
			index = this->GetAt(this->current)->First();
			//1.3.2 다음으로 이동한 줄의 글자 개수가 0보다 크면
			//다음으로 이동한 줄이 글자를 최소 1개 이상은 가지고 있으면
			//다음으로 이동한 줄의 글자 개수가 0이면 읽을 글자가 없기때문에 글자를 읽으면 안되고
			//First로 이동시켰기 때문에 그 자리에 그대로 있으면 됨.
			if (this->GetAt(this->current)->GetLength() > 0)
			{
				//1.3.2.1. 다음으로 이동한 줄의 첫번째 글자를 읽는다.
				letter = this->GetAt(this->current)->GetAt(index)->GetContent();
				//1.3.2.2 마지막 글자 위치를 저장한다.
				Long lastPositionOfLetter = this->GetAt(this->current)->GetLength() - 1;
				//1.3.2.3 캐럿의 현재 가로 위치가 줄의 마지막 글자 위치보다 작은동안 그리고
				//현재 읽은 문자가 탭문자인 동안 반복한다.
				while (index < lastPositionOfLetter && letter == "\t")
				{
					//1.3.2.1.1 캐럿의 가로위치를 다음으로 이동시킨다.
					index = this->GetAt(this->current)->Next();
					//1.3.2.1.2 다음 글자를 읽는다.
					letter = this->GetAt(this->current)->GetAt(index)->GetContent();
				}
				//1.3.2.4 캐럿의 현재 가로 위치가 줄의 마지막 글자 위치와 같고,
				//현재 읽은 문자가 탭문자이면
				if (index == lastPositionOfLetter && letter == "\t")
				{
					//1.3.2.4.1 캐럿의 가로 위치를 마지막으로 이동시킨다.
					index = this->GetAt(this->current)->Last();
				}
				//1.3.2.5 캐럿의 현재 가로 위치가 줄의 마지막 글자 위치보다 작은동안 그리고
				//현재 읽은 문자가 공백(스페이스바 문자)인 동안 반복한다.
				while (index < lastPositionOfLetter && letter == " ")
				{
					//1.3.2.2.1 캐럿의 가로위치를 다음으로 이동시킨다.
					index = this->GetAt(this->current)->Next();
					//1.3.2.2.2 다음 글자를 읽는다.
					letter = this->GetAt(this->current)->GetAt(index)->GetContent();
				}
				//1.3.2.6 캐럿의 현재 가로 위치가 줄의 마지막 글자 위치와 같고,
				//현재 읽은 문자가 스페이스문자(공백)이면
				if (index == lastPositionOfLetter && letter == " ")
				{
					//1.3.2.6.1 캐럿의 가로 위치를 마지막으로 이동시킨다.
					index = this->GetAt(this->current)->Last();
				}
			}
		}
	}
	//2. 캐럿의 현재 가로위치가 현재줄의 글자개수와 다른 경우(한줄내에서만 이동하는 경우)
	else
	{
		//2.1 현재 줄의 WordNext를 호출한다.
		index = this->GetAt(this->current)->NextWord();
	}
	//3. 캐럿의 현재 가로 위치를 출력한다.
	return index;
}

//메모장에서 자동개행이 진행 중인 상태에서 캐럿을 단어단위로 오른쪽으로 이동시키기
void Note::NextWordOnRowAutoChange(Long currentRowIndex, Long currentLetterIndex)
{
	//1. 메모장의 현재 줄에서 처음 글자부터 현재 글자까지의 content를 구한다.
	Long i = 0;
	Glyph* letter = 0;
	string letterContent = "";
	string currentLetters = "";
	Glyph* row = this->GetAt(currentRowIndex);
	while (i < currentLetterIndex)
	{
		//1.1 글자를 구한다.
		letter = row->GetAt(i);
		letterContent = letter->GetContent();
		//1.2 줄의 content에 더해준다.
		currentLetters += letterContent;
		i++;
	}
	//2. 메모장의 현재 줄에서 처음글자부터 현재 글자까지의 content의 길이를 구한다.
	Long currentLettersLength = currentLetters.length();
	//3. 현재 줄의 처음글자부터 마지막글자까지의 content를 구한다.
	i = 0;
	string realRowContent = "";
	while (i < row->GetLength())
	{
		//3.1 글자를 구한다.
		letter = row->GetAt(i);
		letterContent = letter->GetContent();
		//3.2 줄의 content에 더해준다.
		realRowContent += letterContent;
		i++;
	}
	//4. 다음 줄을 구한다.
	Long nextRowIndex = currentRowIndex + 1;
	Glyph* nextRow = this->GetAt(nextRowIndex);
	//5. 다음 줄의 위치가 줄의 개수보다 작은동안 그리고 다음 줄이 가짜 줄인 동안 반복한다.
	string dummyRowContent = "";
	while (nextRowIndex < this->GetLength() &&
		dynamic_cast<DummyRow*>(nextRow))
	{
		//5.1 가짜줄의 content를 구한다.
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
		//5.2 가짜줄의 content를 진짜 줄의 content에 더해준다.
		realRowContent += dummyRowContent;
		//5.3 다음 줄로 이동한다.
		nextRowIndex++;
		//5.4 다음 줄을 구한다.
		nextRow = this->GetAt(nextRowIndex);
	}
	//6. 가짜 줄을 더한 진짜 줄의 content 길이를 구한다.
	Long rowContentLength = realRowContent.length();
	Long rowLastContentLength = rowContentLength - 1;
	i = currentLettersLength;
	Long j = 0;
	//7. 현재 글자 위치가 현재 줄의 마지막 글자 위치면(현재 줄은 진짜 줄 또는 가짜 줄 하지만
	//다음 줄은 무조건 진짜 줄임. 그래서 현재 줄의 마지막 글자 위치에 있으면 다음 줄(진짜 줄)로 이동함.)
	if (i == rowContentLength)
	{
		//7.1 다음 줄로 이동한다.
		currentRowIndex++;
		//7.2 다음 줄의 처음글자위치로 이동한다.
		currentLetterIndex = 0;
		//7.3 줄을 구한다.
		row = this->GetAt(currentRowIndex);
		//7.4 줄의 content를 구한다.
		j = 0;
		realRowContent = "";
		while (j < row->GetLength())
		{
			//7.4.1 글자를 구한다.
			letter = row->GetAt(j);
			letterContent = letter->GetContent();
			//7.4.2 줄의 content에 더해준다.
			realRowContent += letterContent;
			j++;
		}
		//7.5 다음 줄을 구한다.
		nextRowIndex = currentRowIndex + 1;
		nextRow = this->GetAt(nextRowIndex);
		//7.6 다음 줄의 위치가 줄의 개수보다 작은동안 그리고 다음 줄이 가짜 줄인 동안 반복한다.
		dummyRowContent = "";
		while (nextRowIndex < this->GetLength() &&
			dynamic_cast<DummyRow*>(nextRow))
		{
			//7.6.1 가짜줄의 content를 구한다.
			j = 0;
			dummyRowContent = "";
			while (j < nextRow->GetLength())
			{
				//7.6.1.1 글자를 구한다.
				letter = nextRow->GetAt(j);
				letterContent = letter->GetContent();
				//7.6.1.2 줄의 content에 더해준다.
				dummyRowContent += letterContent;
				j++;
			}
			//7.8.2 가짜줄의 content를 진짜 줄의 content에 더해준다.
			realRowContent += dummyRowContent;
			//7.8.3 다음 줄로 이동한다.
			nextRowIndex++;
			//7.8.4 다음 줄을 구한다.
			nextRow = this->GetAt(nextRowIndex);
		}
		//7.7 줄의 content 길이를 구한다.
		rowContentLength = realRowContent.length();
		i = 0;
		letterContent = realRowContent[i];//한글자를 담을 공간
		//7.8 줄의 글자가 탭문자거나 스페이스문자이면
		if (letterContent == "\t" || letterContent == " ")
		{
			//7.8.1 마지막 글자 위치를 구한다.
			rowLastContentLength = rowContentLength - 1;
			//7.8.2 마지막 글자위치보다 작은동안 그리고 탭문자인동안 반복한다.
			while (i < rowLastContentLength && letterContent == "\t")
			{
				//7.8.2.1 현재 글자위치를 증가시킨다.
				currentLetterIndex++;
				i++;
				//7.8.2.2 글자를 저장한다.
				letterContent = realRowContent[i];
			}
			//7.8.3 마지막 글자위치와 같고 탭문자이면
			if (i == rowLastContentLength && letterContent == "\t")
			{
				//7.8.3.1 현재 글자위치를 증가시킨다.
				currentLetterIndex++;
				i++;
			}
			//7.8.4 마지막 글자위치보다 작은동안 그리고 스페이스문자인동안 반복한다.
			while (i < rowLastContentLength && letterContent == " ")
			{
				//7.8.4.1 현재 글자위치를 증가시킨다.
				currentLetterIndex++;
				i++;
				//7.8.4.2 글자를 저장한다.
				letterContent = realRowContent[i];
			}
			//7.8.5 마지막 글자위치와 같고 스페이스문자이면
			if (i == rowLastContentLength && letterContent == " ")
			{
				//7.8.5.1 현재 글자위치를 증가시킨다.
				currentLetterIndex++;
				i++;
			}
		}
	}
	//8. 그게 아니면
	else
	{
		//8.1 한글이면
		if ((realRowContent[i] & 0x80))//한글이면(2byte문자)
		{
			//8.1.1 2byte로 저장한다.
			letterContent = realRowContent[i];
			i++;
			letterContent += realRowContent[i];
		}
		//8.2 한글이 아니면
		else
		{
			//8.2.1 1byte로 저장한다
			letterContent = realRowContent[i];
		}
		//8.3 마지막 글자위치보다 작은동안 그리고 탭문자와 스페이스문자가 아닌동안 반복한다.
		while (i < rowLastContentLength && letterContent != " " && letterContent != "\t")
		{
			//8.3.1 현재 글자위치를 증가시킨다.
			currentLetterIndex++;
			i++;
			//8.3.2 줄에서 i번째 읽을 글자가 한글이면
			if ((realRowContent[i] & 0x80))//한글이면(2byte문자)
			{
				//8.3.2.1 2byte로 저장한다.
				letterContent = realRowContent[i];
				i++;
				letterContent += realRowContent[i];
			}
			//8.3.3 한글이 아니면
			else
			{
				//8.3.3.1 1byte로 저장한다
				letterContent = realRowContent[i];
			}
		}
		//8.4 마지막 글자위치와 같고, 탭문자와 스페이스 문자가 아니면
		if (i == rowLastContentLength && letterContent != " " && letterContent != "\t")
		{
			//8.4.1 현재 글자위치를 증가시킨다.
			currentLetterIndex++;
			i++;
		}
		//8.5 마지막 글자위치보다 작은동안 그리고 탭문자인동안 반복한다.
		while (i < rowLastContentLength && letterContent == "\t")
		{
			//8.5.1 현재 글자위치를 증가시킨다.
			currentLetterIndex++;
			i++;
			//8.5.2 글자를 저장한다.
			letterContent = realRowContent[i];
		}
		//8.6 마지막 글자위치와 같고 탭문자이면
		if (i == rowLastContentLength && letterContent == "\t")
		{
			//8.6.1 현재 글자위치를 증가시킨다.
			currentLetterIndex++;
			i++;
		}
		//8.7 마지막 글자위치보다 작은동안 그리고 스페이스문자인동안 반복한다.
		while (i < rowLastContentLength && letterContent == " ")
		{
			//8.7.1 현재 글자위치를 증가시킨다.
			currentLetterIndex++;
			i++;
			//8.7.2 글자를 저장한다.
			letterContent = realRowContent[i];
		}
		//8.8 마지막 글자위치와 같고 스페이스문자이면
		if (i == rowLastContentLength && letterContent == " ")
		{
			//8.8.1 현재 글자위치를 증가시킨다.
			currentLetterIndex++;
			i++;
		}
	}
	//9. 현재 줄을 구한다.
	Glyph* currentRow = this->GetAt(currentRowIndex);
	//10. 현재 줄의 content를 구한다.
	j = 0;
	string currentRowContent = "";
	while (j < currentRow->GetLength())
	{
		//10.1 글자를 구한다.
		letter = currentRow->GetAt(j);
		letterContent = letter->GetContent();
		//10.2 줄의 content에 더해준다.
		currentRowContent += letterContent;
		j++;
	}
	//11. 현재 줄의 content의 length를 구한다.
	Long currentRowContentLength = currentRowContent.length();
	//12. i가 현재 줄의 content의 length보다 크거나 같은동안 반복한다.
	//아까 앞에서 부분자동개행을 취소하는 것처럼 가짜줄의 content들을 진짜줄의 content에
	//추가시켰지만 사실상 현재 메모장에서는 자동개행이 되어 있는 상태이기 때문에
	//자동개행이 된 메모장에서 찾을 문자열을 찾아서 선택을 해야하기 때문에 아까 합쳤던 각각의 
	//가짜줄들을 다시 개별적으로 구해서 합친 줄에서 읽은 글자위치 i와 비교해서 현재 줄의 위치를
	//다시 구해준다.
	string nextRowContent = "";
	while (i > currentRowContentLength)
	{
		//12.1 현재 줄의 위치를 증가시킨다.
		//자동개행이 취소된 상태에서 해당 문자열을 찾았는데 최종적으로 읽은 글자가
		//현재 개별적인 줄의 길이보다 길기 때문에 다음 줄로 넘어가서 다시 비교해야한다.
		currentRowIndex++;
		//12.2 현재 글자위치도 자동개행이 취소된 상태의 글자위치기 때문에 현재 글자 
		//위치가 현재 줄의 개수보다 크기 때문에 그만큼 빼서 자동개행이 된 상태로 조정한다
		//앞에서 currentLetterIndex++로 인해 1이 증가된 상태이기 때문에 +1을 따로 안해줘도됨!
		currentLetterIndex -= currentRow->GetLength();
		//12.3 현재 줄의 다음줄을 구한다.
		currentRow = this->GetAt(currentRowIndex);
		//12.4 다음 줄의 content를 저장한다.
		j = 0;
		nextRowContent = "";
		while (j < currentRow->GetLength())
		{
			//12.4.1 글자를 구한다.
			letter = currentRow->GetAt(j);
			letterContent = letter->GetContent();
			//12.4.2 줄의 content에 더해준다.
			nextRowContent += letterContent;
			j++;
		}
		//12.5 다음 줄의 content를 현재 줄의 content에 더해준다.(누적)
		currentRowContent += nextRowContent;
		//12.6 누적된 줄의 content의 길이를 구한다.
		currentRowContentLength = currentRowContent.length();
	}
	//13. 현재 줄의 위치와 글자위치를 다시 조정해준다.
	currentRowIndex = this->Move(currentRowIndex);
	currentRow = this->GetAt(currentRowIndex);
	currentLetterIndex = currentRow->Move(currentLetterIndex);
	//14. 현재 줄이 마지막 줄이 아니면
	Long lastRowIndex = this->GetLength() - 1;
	if (currentRowIndex < lastRowIndex)
	{
		//14.1 다음 줄을 구한다.
		nextRow = this->GetAt(currentRowIndex + 1);
		//14.2 다음 줄이 가짜 줄이면
		if (dynamic_cast<DummyRow*>(nextRow))
		{
			//14.2.1 현재 글자위치가 글자의 마지막에 있으면
			Long letterIndex = currentRow->GetLength();
			if (currentLetterIndex == letterIndex)
			{
				//14.2.1.1 다음 줄로 이동시킨다.
				currentRowIndex = this->Next();
				//14.2.1.2 현재 줄을 다음으로 이동한 줄로 변경한다.
				currentRow = this->GetAt(currentRowIndex);
				//14.2.1.3 캐럿의 현재 위치를 처음으로 보낸다.(현재 줄이 다음 줄로 옮겨졌기 때문에
				//캐럿은 이전 줄의 마지막 위치에서 다음 줄의 처음 위치로 이동한다.)
				currentLetterIndex = currentRow->First();
			}
		}
	}
}

//메모장에서 자동개행이 진행 중인 상태에서 캐럿을 단어단위로 왼쪽으로 이동시키기
void Note::PreviousWordOnRowAutoChange(Long currentRowIndex, Long currentLetterIndex)
{
	//1. 메모장의 현재 줄에서 처음 글자부터 현재 글자까지의 content를 구한다.
	Long i = 0;
	Glyph* letter = 0;
	string letterContent = "";
	string currentLetters = "";
	Glyph* row = this->GetAt(currentRowIndex);
	while (i < currentLetterIndex)
	{
		//1.1 글자를 구한다.
		letter = row->GetAt(i);
		letterContent = letter->GetContent();
		//1.2 줄의 content에 더해준다.
		currentLetters += letterContent;
		i++;
	}
	//2. 메모장의 현재 줄에서 처음글자부터 현재 글자까지의 content의 길이를 구한다.
	Long currentLettersLength = currentLetters.length();
	//3. 줄의 content를 구한다.
	i = 0;
	string rowContent = "";
	while (i < currentLetterIndex)
	{
		//3.1 글자를 구한다.
		letter = row->GetAt(i);
		letterContent = letter->GetContent();
		//3.2 줄의 content에 더해준다.
		rowContent += letterContent;
		i++;
	}
	//4. 현재 줄이 가짜 줄이면(현재 줄의 위치가 0이면 가짜 줄일 수 X)
	if (dynamic_cast<DummyRow*>(row))
	{
		//4.1 이전 줄로 이동한다.
		Long previousRowIndex = currentRowIndex - 1;
		//4.2 이전 줄을 구한다
		Glyph* previousRow = this->GetAt(previousRowIndex);
		//4.3 이전 줄이 가짜줄인동안 반복한다.
		string previousRowContent = "";
		while (dynamic_cast<DummyRow*>(previousRow))
		{
			//4.3.1 이전 줄의 content를 구한다.
			i = 0;
			previousRowContent = "";
			while (i < previousRow->GetLength())
			{
				//4.3.1.1 글자를 구한다.
				letter = previousRow->GetAt(i);
				letterContent = letter->GetContent();
				//4.3.1.2 줄의 content에 더해준다.
				previousRowContent += letterContent;
				i++;
			}
			//4.3.2 이전 줄의 content에 현재 줄의 cotent를 더해준다.(누적)
			previousRowContent += rowContent;
			//4.3.3 더해준 값을 옮겨 저장한다.(반복 구조에서 제대로 누적시키기 위해서)
			rowContent = previousRowContent;
			//4.3.4 메모장의 현재 글자위치에 이전 줄의 글자개수를 더해준다.
			currentLetterIndex += previousRow->GetLength();
			//4.3.5 이전으로 이동한다.
			previousRowIndex--;
			//4.3.6 이전 줄을 구한다.
			previousRow = this->GetAt(previousRowIndex);
		}
		//이전으로 이동했을 때 진짜 줄이 나와서 반복구조를 나왔기 때문에 지금 이전 줄이 진짜 줄이다.
		//따라서 이전 줄(진짜 줄)의 content를 구해서 아까 반복구조에서 구했던 가짜줄의 content를 더해준다
		//4.4 진짜 줄의 위치를 현재 줄의 위치로 설정한다.
		currentRowIndex = previousRowIndex;
		//4.5 진짜 줄의 content를 구한다.
		i = 0;
		previousRowContent = "";
		while (i < previousRow->GetLength())
		{
			//4.5.1 글자를 구한다.
			letter = previousRow->GetAt(i);
			letterContent = letter->GetContent();
			//4.5.2 줄의 content에 더해준다.
			previousRowContent += letterContent;
			i++;
		}
		//4.6 진짜 줄의 content를 누적시킨다.
		previousRowContent += rowContent;
		//4.7 더해준 값을 옮겨 저장한다.(반복 구조에서 제대로 누적시키기 위해서)
		rowContent = previousRowContent;
		//4.8 메모장의 현재 글자위치에 이전 줄의 글자개수를 더해준다.
		currentLetterIndex += previousRow->GetLength();
		//4.9 현재 줄의 길이를 구한다.(자동개행 취소된 마지막 글자위치에서 시작)
		currentLettersLength = rowContent.length();
	}
	//5. 진짜 줄의 위치를 저장한다.
	Long realRowIndex = currentRowIndex;
	string koreanCompositionTemp;//역순으로 거슬러 올라가기 때문에 역순으로 한글을 조립하면 한글이 제대로
	//조립되지 않기 때문에 한글을 임시로 저장하고 원래 순서대로 조립할 임시 저장소가 필요함.
	//6. 현재 글자 위치가 0보다 크면(현재 글자 위치가 0이면 단어단위로 이동할 수 X)
	if (currentLetterIndex > 0)
	{
		//6.1 줄에서 읽어야 할 글자 한 칸 만큼 감소시킨다.
		i = currentLettersLength - 1;
		//currentLetterIndex--;
		//6.2 줄에서 i번째 읽을 글자가 한글이면
		if ((rowContent[i] & 0x80))//한글이면(2byte문자)
		{
			//6.2.1 2byte로 저장한다.
			letterContent = rowContent[i];
			//역순으로 거슬러 가기 때문에 제대로 한글을 조립하기 위해서 임시저장을 해야함.
			koreanCompositionTemp = letterContent;
			i--;
			letterContent = rowContent[i];
			letterContent += koreanCompositionTemp;
		}
		//6.3 한글이 아니면
		else
		{
			//6.3.1 1byte로 저장한다
			letterContent = rowContent[i];
		}
		//6.4 현재 글자 위치가 1보다 크고 읽은 글자가 스페이스(공백)문자인동안 반복한다.
		while (currentLetterIndex > 1 && letterContent == " ")
		{
			//6.4.1 현재 글자위치를 감소시킨다.
			currentLetterIndex--;
			i--;
			//6.4.2 글자를 저장한다.
			letterContent = rowContent[i];
		}
		//6.5 현재 글자 위치가 1이고 읽은 글자가 스페이스(공백)문자이면
		if (currentLetterIndex == 1 && letterContent == " ")
		{
			//6.5.1 현재 글자위치를 감소시킨다.
			currentLetterIndex--;
			i--;
		}
		//6.6 현재 글자 위치가 1보다 크고 읽은 글자가 탭문자인동안 반복한다.
		while (currentLetterIndex > 1 && letterContent == "\t")
		{
			//6.6.1 현재 글자위치를 감소시킨다.
			currentLetterIndex--;
			i--;
			//6.6.2 글자를 저장한다.
			letterContent = rowContent[i];
		}
		//6.7 현재 글자 위치가 1이고 읽은 글자가 탭문자이면
		if (currentLetterIndex == 1 && letterContent == "\t")
		{
			//6.7.1 현재 글자위치를 감소시킨다.
			currentLetterIndex--;
			i--;
		}
		//6.8 현재 글자 위치가 1보다 크고 읽은 글자가 스페이스와 탭문자가 아닌동안 반복한다.
		if (currentLetterIndex > 1 && letterContent != " " && letterContent != "\t")
		{
			//6.8.1 현재 읽은 글자가 한글이면
			if ((rowContent[i] & 0x80))//한글이면(2byte문자)
			{
				//6.8.1 2byte로 저장한다.
				letterContent = rowContent[i];
				//역순으로 거슬러 가기 때문에 제대로 한글을 조립하기 위해서 임시저장을 해야함.
				koreanCompositionTemp = letterContent;
				i--;
				letterContent = rowContent[i];
				letterContent += koreanCompositionTemp;
			}
			//6.8.2 한글이 아니면
			else
			{
				//6.8.2.1 1byte로 저장한다
				letterContent = rowContent[i];
			}
		}
		//6.9 현재 글자 위치가 1보다 크고 읽은 글자가 스페이스(공백)문자 그리고
		//탭문자가 아닌동안 반복한다.
		while (currentLetterIndex > 1 && letterContent != " " && letterContent != "\t")
		{
			//6.9.1 현재 글자 위치를 감소시킨다.
			currentLetterIndex--;
			i--;
			//6.9.2 현재 읽은 글자가 한글이면
			if ((rowContent[i] & 0x80))//한글이면(2byte문자)
			{
				//6.9.2.1 2byte로 저장한다.
				letterContent = rowContent[i];
				//역순으로 거슬러 가기 때문에 제대로 한글을 조립하기 위해서 임시저장을 해야함.
				koreanCompositionTemp = letterContent;
				i--;
				letterContent = rowContent[i];
				letterContent += koreanCompositionTemp;
			}
			//6.9.3 한글이 아니면
			else
			{
				//6.9.3.1 1byte로 저장한다
				letterContent = rowContent[i];
			}
		}
		//6.10 현재 캐럿의 가로 위치가 1이고 읽은 글자가 스페이스(공백) 문자와 탭문자가 아니면
		if (currentLetterIndex == 1 && letterContent != " " && letterContent != "\t")
		{
			//6.10.1 현재 읽은 글자가 한글이면
			if ((rowContent[i] & 0x80))//한글이면(2byte문자)
			{
				//6.10.1.1 2byte로 저장한다.
				letterContent = rowContent[i];
				//역순으로 거슬러 가기 때문에 제대로 한글을 조립하기 위해서 임시저장을 해야함.
				koreanCompositionTemp = letterContent;
				i--;
				letterContent = rowContent[i];
				letterContent += koreanCompositionTemp;
			}
			//6.10.2 한글이 아니면
			else
			{
				//6.10.2.1 1byte로 저장한다
				letterContent = rowContent[i];
			}
			//6.10.3 현재 글자 위치를 감소시킨다.
			currentLetterIndex--;
			i--;
		}
		//6.11 현재 글자 위치가 0이면
		if (currentLetterIndex == 0)
		{
			//6.11.1 글자를 저장한다.
			//letterContent = rowContent[i];
			//6.11.2 현재 줄의 첫번째 글자가 스페이스 또는 탭문자이면
			if (letterContent == "\t" || letterContent == " ")
			{
				//6.11.2.1 현재 줄의 위치를 1만큼 감소시킨다.(이전 줄로 이동시킨다.)
				currentRowIndex--;
				//6.11.2.2 현재 줄의 위치가 underflow이면
				if (currentRowIndex < 0)
				{
					//6.11.2.2.1 현재 줄의 위치를 최소값으로 변경한다.
					currentRowIndex = 0;
				}
				//6.11.2.3 현재 줄의 위치가 underflow가 아니면(this->current >=0)
				else
				{
					//6.11.2.3.1 현재 캐럿의 가로 위치를 마지막으로 이동시킨다.
					currentRowIndex = this->Move(currentRowIndex);
					row = this->GetAt(currentRowIndex);
					currentLetterIndex = row->Last();
				}
			}
		}
	}
	//7. 현재 글자 위치가 제일 처음이면(0이면)
	else if (currentLetterIndex == 0)
	{
		//7.1 현재 줄의 위치를 1만큼 감소시킨다.(이전 줄로 이동시킨다.)
		currentRowIndex--;
		//7.2 현재 줄의 위치가 underflow이면
		if (currentRowIndex < 0)
		{
			//7.2.1 현재 줄의 위치를 최소값으로 변경한다.
			currentRowIndex = 0;
		}
		//7.3 현재 줄의 위치가 underflow가 아니면(this->current >=0)
		else
		{
			//7.3.1 현재 캐럿의 가로 위치를 마지막으로 이동시킨다.
			currentRowIndex = this->Move(currentRowIndex);
			row = this->GetAt(currentRowIndex);
			currentLetterIndex = row->Last();
		}
	}
	//8. 메모장에서 현재 줄을 구한다.(진짜 줄)
	Glyph* currentRow = this->GetAt(currentRowIndex);
	//9. 현재 줄의 content를 구한다.
	Long k = 0;
	string currentRowContent = "";
	while (k < currentRow->GetLength())
	{
		//9.1 글자를 구한다.
		letter = currentRow->GetAt(k);
		letterContent = letter->GetContent();
		//9.2 줄의 content에 더해준다.
		currentRowContent += letterContent;
		k++;
	}
	//10. 현재 줄의 length를 구한다.
	Long currentRowContentLength = currentRowContent.length();
	//11. i가 현재 줄의 length보다 크거나 같은동안 반복한다.
	while (i >= currentRowContentLength)
	{
		//11.1 현재 줄의 길이를 뺀다.
		i -= currentRowContentLength;
		//11.2 현재 줄의 개수를 뺀다.
		currentLetterIndex -= currentRow->GetLength();
		//11.3 다음 줄로 이동한다.
		currentRowIndex++;
		//11.4 다음 줄을 구한다.
		currentRow = this->GetAt(currentRowIndex);
		//11.5 다음 줄의 content를 구한다.
		k = 0;
		currentRowContent = "";
		while (k < currentRow->GetLength())
		{
			//11.5.1 글자를 구한다.
			letter = currentRow->GetAt(k);
			letterContent = letter->GetContent();
			//11.5.2 줄의 content에 더해준다.
			currentRowContent += letterContent;
			k++;
		}
		//11.6 다음 줄의 content의 length를 구한다.
		currentRowContentLength = currentRowContent.length();
	}
	//12. 현재 줄의 위치와 글자위치를 다시 조정해준다.
	currentRowIndex = this->Move(currentRowIndex);
	currentRow = this->GetAt(currentRowIndex);
	currentLetterIndex = currentRow->Move(currentLetterIndex);
	//13. 현재 줄의 위치가 현재 줄의 마지막 글자 위치이면
	if (currentLetterIndex == currentRow->GetLength())
	{
		//13.1 다음 줄을 구한다.
		Glyph* nextRow = this->GetAt(currentRowIndex + 1);
		//13.2 다음 줄이 진짜 줄이고 현재 줄이 가짜 줄이면
		if (!dynamic_cast<DummyRow*>(nextRow) && dynamic_cast<DummyRow*>(currentRow))
		{
			//13.2.1 줄이 이전으로 이동했기 때문에 캐럿의 현재 위치를 마지막으로 변경한다.
			currentLetterIndex = currentRow->Last();
		}
		//13.3 다음 줄이 진짜 줄이고 현재 줄이 진짜 줄이면
		else if (!dynamic_cast<DummyRow*>(nextRow) && !dynamic_cast<DummyRow*>(currentRow))
		{
			//13.3.1 줄이 이전으로 이동했기 때문에 캐럿의 현재 위치를 마지막으로 변경한다.
			currentLetterIndex = currentRow->Last();
		}
		//13.4 그 이외에는
		else
		{
			//13.4.1 현재 줄의 위치와 글자위치를 다시 조정해준다.
			currentRowIndex = this->Move(currentRowIndex + 1);
			currentRow = this->GetAt(currentRowIndex);
			currentLetterIndex = currentRow->First();
		}
	}
}

//노트에서 캐럿을 단어단위로 왼쪽으로 이동시키기
Long Note::PreviousWord()
{
	string letter;//글자를 담을 공간
	//1. 현재 캐럿의 위치를 구한다.
	Long index = this->GetAt(this->current)->GetCurrent();
	//2. 현재 캐럿의 위치가 0보다 크면
	if (index > 0)
	{
		//2.1 현재 줄의 WordPrevious를 호출한다.
		index = this->GetAt(this->current)->PreviousWord();
		//2.2 index가 0이면
		if (index == 0)
		{
			//2.2.1 현재 줄의 제일 첫번째 글자를 읽는다.
			letter = this->GetAt(this->current)->GetAt(index)->GetContent();
			//필요없음
			//2.2.2 현재 줄의 첫번째 글자가 스페이스와 탭문자가 아니면(이미 위치가 0이므로 이동안하면됨)
			//(한글이나 영문, 특수문자, 숫자이면)
			//if (letter != "\t" && letter != " ")
			//{
				//2.2.2.1 현재 캐럿의 가로 위치를 0으로 이동시킨다.
			//	index = this->GetAt(this->current)->First();
			//}
			//2.2.3 현재 줄의 첫번째 글자가 스페이스 또는 탭문자이면
			//else
			if (letter == "\t" || letter == " ")
			{
				//2.2.3.1 현재 줄의 위치를 1만큼 감소시킨다.(이전 줄로 이동시킨다.)
				this->current--;
				//2.2.3.2 현재 줄의 위치가 underflow이면
				if (this->current < 0)
				{
					//2.2.3.2.1 현재 줄의 위치를 최소값으로 변경한다.
					this->current = 0;
				}
				//2.2.3.3 현재 줄의 위치가 underflow가 아니면(this->current >=0)
				else
				{
					//2.2.3.3.1 현재 캐럿의 가로 위치를 마지막으로 이동시킨다.
					index = this->GetAt(this->current)->Last();
				}
			}
		}
	}
	//3. 이동하기전 캐럿의 가로 위치가 0이면
	else if (index == 0)
	{
		//3.1 캐럿의 현재 세로 위치를 1만큼 감소시킨다.(이전 줄로 이동시킨다.)
		this->current--;
		//3.2 캐럿의 현재 세로 위치가 underflow이면
		if (this->current < 0)
		{
			//3.2.1 캐럿의 현재 세로 위치를 최소값으로 변경한다.
			this->current = 0;
		}
		//3.3 캐럿의 현재 세로 위치가 underflow가 아니면(this->current >=0)
		else
		{
			//3.3.1 현재 캐럿의 가로 위치를 마지막으로 이동시킨다.
			index = this->GetAt(this->current)->Last();
		}
	}
	return index;
}

void Note::CalculateSelectedRange(Long* startingRowPos, Long* startingLetterPos,
	Long* endingRowPos, Long* endingLetterPos)
{
	//1. 첫줄, 첫칸으로 보낸다.
	*startingRowPos = 0;
	*startingLetterPos = 0;
	*endingRowPos = 0;
	*endingLetterPos = 0;
	Long rowIndex = 0;
	Long letterIndex = 0;
	Long letterCount = 0;
	Long rowCount = this->length;
	Glyph* row = 0;
	Glyph* letter = 0;
	bool isSelected = false;
	//2. 줄의 위치가 줄의 개수보다 작은동안 그리고 false인동안 반복한다.
	while (rowIndex < rowCount && isSelected == false)
	{
		//2.1 줄을 구한다.
		row = this->GetAt(rowIndex);
		//2.2 줄의 글자개수를 구한다.
		letterCount = row->GetLength();
		//2.2 글자 위치를 원위치시킨다.
		letterIndex = 0;
		//2.4 글자위치가 글자개수보다 작은동안 그리고 false인동안 반복한다.
		while (letterIndex < letterCount && isSelected == false)
		{
			//2.4.1 글자를 구한다.
			letter = row->GetAt(letterIndex);
			//2.4.2 글자가 선택되어 있으면
			if (letter->IsSelected() == true)
			{
				//2.4.2.1 isSelected를 true로 바꾼다.
				isSelected = true;
				//2.4.2.2 시작 줄의 위치를 저장한다.
				*startingRowPos = rowIndex;
				//2.4.2.3 시작 글자의 위치를 저장한다.
				*startingLetterPos = letterIndex;
			}
			//2.4.3 글자 위치를 증가시킨다.
			letterIndex++;
		}
		//2.5 줄의 위치를 증가시킨다.
		rowIndex++;
	}
	//3. 현재 줄과 글자를 원위치시켜준다.
	rowIndex = *startingRowPos;
	Long index = *startingLetterPos;
	//3. 줄의 위치가 줄의 개수보다 작은동안 그리고 true인동안 반복한다.
	while (rowIndex < rowCount && isSelected == true)
	{
		//3.1 줄을 구한다.
		row = this->GetAt(rowIndex);
		//3.2 줄의 글자개수를 구한다.
		letterCount = row->GetLength();
		//3.3 글자 위치를 원위치시킨다.
		letterIndex = index;
		//3.4 글자위치가 글자개수보다 작은동안 그리고 true인동안 반복한다.
		while (letterIndex < letterCount && isSelected == true)
		{
			//3.4.1 글자를 구한다.
			letter = row->GetAt(letterIndex);
			//3.4.2 글자가 선택되어 있으면
			if (letter->IsSelected() == true)
			{
				//3.4.2.1 마지막 줄의 위치를 저장한다.
				*endingRowPos = rowIndex;
				//3.4.2.2 마지막 글자의 위치를 저장한다.
				*endingLetterPos = letterIndex;
			}
			//3.4.3 글자가 선택되어 있지 않으면
			else
			{
				//3.4.3.1 isSelected를 false로 바꾼다.
				isSelected = false;
			}
			//3.4.3 글자 위치를 증가시킨다.
			letterIndex++;
		}
		//3.5 줄의 위치를 증가시킨다.
		rowIndex++;
		index = 0;
	}
}

//Visitor Pattern
void Note::Accept(GlyphVisitor* glyphVisitor)
{
	glyphVisitor->VisitNote(this);
}