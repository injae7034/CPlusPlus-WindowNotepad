#include "Clipboard.h"

//디폴트생성자
Clipboard::Clipboard(Long capacity)
	:Composite(capacity)
{

}

//복사생성자
Clipboard::Clipboard(const Clipboard& source)
	:Composite(source)
{

}

//치환연산자
Clipboard& Clipboard::operator=(const Clipboard& source)
{
	//this->Composite::operator=(source);
	Composite::operator=(source);

	return *this;
}

//소멸자
Clipboard::~Clipboard()
{

}

//Prototype Patter Clone
Glyph* Clipboard::Clone()
{
	return new Clipboard(*this);
}

//GetContent
string Clipboard::GetContent()
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


//Clipboard기준에 맞게 Add, Remove 재정의(오버라이딩)
Long Clipboard::Add(Glyph* glyph)
{
	//1. 추가할 노트의 주소를 입력받는다.
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
	//5. 클립보드에 저장된 노트리스트에서 현재 노트 위치를 저장한다.
	this->current = index;
	//6. 현재 노트의 위치를 출력한다.
	return index;
}

Long Clipboard::Remove(Long index)
{
	//1. 지울 노트의 위치를 입력받는다.
	//2. 노트리스트에서 노트를 지우고 나면 지우는 노트의 위치보다 이전 위치가 현재 위치가 되기 때문에 
	//현재 노트의 위치를 index-1로 저장한다.
	this->current = index - 1;
	//3. 해당배열요소가 주소를 저장하고 있기 때문에 해당배열요소의 힙에 할당된 내용(노트)을 먼저 지워야함.
	Glyph* glyph = this->glyphs[index];
	if (glyph != 0)
	{
		delete glyph;
	}
	//4. 해당위치의 배열요소의 힙에 할당된 내용(노트)을 지웠으니 이제 그 주소를 지운다.
	index = this->glyphs.Delete(index);
	//5. 할당량을 감소시킨다.
	this->capacity--;
	//6. 사용량을 감소시킨다.
	this->length--;
	//7. index를 출력한다.
	return index;
}

Long Clipboard::RemoveAll()
{
	//1. 클립보드에 있는 복사리스트를 모두 지운다.
	Glyph* glyph = 0;
	Long i = 0;
	while (i < this->length)
	{
		//1.1 해당배열요소가 주소를 저장하고 있기 때문에 해당배열요소의 힙에 할당된 내용(노트)을
		//먼저 지운다.
		glyph = this->glyphs[i];
		if (glyph != 0)
		{
			delete glyph;
		}
		//1.2 해당위치의 배열요소의 힙에 할당된 내용(노트)을 지웠으니 이제 그 주소를 지운다.
		i = this->glyphs.Delete(i);
		//1.3 할당량을 감소시킨다.
		this->capacity--;
		//1.4 사용량을 감소시킨다.
		this->length--;
	}
	//2. 다 지워졌으면
	if (this->length == 0)
	{
		i = -1;
	}
	//3. i를 출력한다.
	return i;
}