#include "Note.h"
#include "Letter.h"

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
	string content;
	Long index = 0;
	while (index < this->length)
	{
		content += this->glyphs.GetAt(index)->GetContent();
		content += '\n';
		index++;
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
			//2.2.2 현재 줄의 첫번째 글자가 스페이스와 탭문자가 아니면
			//(한글이나 영문, 특숨문자, 숫자이면)
			if (letter != "\t" && letter != " ")
			{
				//2.2.2.1 현재 캐럿의 가로 위치를 0으로 이동시킨다.
				index = this->GetAt(this->current)->First();
			}
			//2.2.3 현재 줄의 첫번째 글자가 스페이스 또는 탭문자이면
			else
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
		//2.1 줄의 글자개수를 구한다.
		letterCount = this->GetAt(rowIndex)->GetLength();
		//2.2 글자 위치를 원위치시킨다.
		letterIndex = 0;
		//2.3 줄을 구한다.
		row = this->GetAt(rowIndex);
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