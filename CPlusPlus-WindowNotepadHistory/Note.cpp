#include "Note.h"
#include "Letter.h"
#include "GlyphVisitor.h"
#include "SingleByteLetter.h"
#include "DoubleByteLetter.h"

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

//FindString
void Note::FindString(Long* rowIndex, Long* letterIndex, string keyword,
	Long* keyWordLetterCount)
{
	//1. 찾기 시작할 줄의 위치와 글자 위치, 찾을 문자열을 입력받는다.
	//2. 메모장에서 note의 row개수보다 작은동안 그리고 문자열을 찾기 전까지 반복한다.
	Long i = 0;
	Long keywordLenth = keyword.length();//찾을 문자열의 길이
	Long letterLength = 0;//한글이면 2, 영문이나 특수문자이면 1
	Long matched = 0;//일치하는 글자개수
	Glyph* row = 0;
	Glyph* letter = 0;
	string letterContent;//메모장의 글자의 content를 담을 임시 buffer
	string partKeyword;//찾을 문자열의 한글자를 담을 임시 buffer
	*keyWordLetterCount = 0;//한글, 영문 구분없이 찾을 문자열의 총 글자 개수
	bool isFindingKeyword = false;//찾을 문자열을 찾았는지 여부
	while (*rowIndex < this->length && isFindingKeyword == false)
	{
		//2.1 row를 구한다.
		row = this->GetAt(*rowIndex);
		//2.2 row의 letter개수보다 작은동안 그리고 문자열을 찾기 전까지 반복한다.
		while (*letterIndex < row->GetLength() && isFindingKeyword == false)
		{
			//2.2.1 letter를 구한다.
			letter = row->GetAt(*letterIndex);
			//2.2.2 letter의 content를 구한다.
			letterContent = letter->GetContent();
			//2.2.3 letter가 singleByte이면
			if (dynamic_cast<SingleByteLetter*>(letter))
			{
				//2.2.3.1 찾고자 하는 단어의 부분을 저장한다.
				partKeyword = keyword[i];
				//2.2.3.2 일치하는 글자 수를 정한다.(SingleByteLetter는 길이가 1이기 때문에)
				letterLength = 1;
			}
			//2.2.4 letter가 doubleByte이면
			else if (dynamic_cast<DoubleByteLetter*>(letter))
			{
				//2.2.4.1 찾고자 하는 단어의 부분을 저장한다.
				partKeyword = keyword[i];
				i++;
				partKeyword += keyword[i];
				//2.2.4.2 일치하는 글자 수를 정한다.(DoubleByteLetter는 길이가 2이기 때문에)
				letterLength = 2;
			}
			//2.2.5 letterContent와 찾을 문자열(key)의 배열요소가 서로 같으면
			if (letterContent == partKeyword)
			{
				//2.2.5.1 일치하는 단어 개수(matched)를 증가시킨다.
				matched += letterLength;
				//2.2.5.2 찾을 문자열의 다음 배열요소로 넘어간다.
				i++;
				//2.2.5.3 찾은 문자열의 글자 개수를 세준다.
				(*keyWordLetterCount)++;
			}
			//2.2.6 letterContent와 찾을 문자열(key)의 배열요소가 서로 다르면
			else
			{
				//2.2.6.1 찾을 문자열의 배열요소 위치를 초기화시킨다.
				i = 0;
				//2.2.6.2 찾은 문자열의 글자 개수를 초기화시킨다.
				*keyWordLetterCount = 0;
			}
			//2.2.7 해당 문자열을 찾았으면
			if (matched == keywordLenth)
			{
				//2.2.7.1 찾았다고 표시한다.
				isFindingKeyword = true;
			}
			//2.2.8 다음 글자로 넘어간다.
			(*letterIndex)++;
		}
		//2.3 다음 줄로 넘어간다.
		(*rowIndex)++;
	}
	//3. 해당 문자열을 찾았으면
	if (matched == keywordLenth)
	{
		//3.1 줄의 위치와 글자의 위치를 조정한다.
		(*rowIndex)--;
		(*letterIndex)--;
	}
	//4. 해당 문자열을 못찾았으면
	else
	{
		*rowIndex = -1;
		*letterIndex = -1;
	}
	//5. 끝내다.
}