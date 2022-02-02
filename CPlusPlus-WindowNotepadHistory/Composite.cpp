#include "Composite.h"

//디폴트생성자
Composite::Composite(Long capacity)
	:glyphs(capacity)
{
	this->capacity = capacity;
	this->length = 0;
	this->current = 0;
}

//소멸자
Composite::~Composite()
{
	Glyph* glyph;
	Long i = 0;
	while (i < this->length)
	{
		glyph = this->GetAt(i);
		if (glyph != 0)
		{
			delete glyph;
		}
		i++;
	}
}

//복사생성자
Composite::Composite(const Composite& source)
	:glyphs(source.glyphs)
{
	Glyph* glyph;
	Long i = 0;
	while (i < source.length)
	{
		glyph = const_cast<Composite&>(source).glyphs.GetAt(i);
		this->glyphs.Modify(i, glyph->Clone());
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
	this->current = source.current;
}

//치환연산자
Composite& Composite::operator=(const Composite& source)
{
	Glyph* glyph;
	Long i = 0;
	//1. 기존의 this(Composite)할당해제
	while (i < this->length)
	{
		glyph = this->glyphs.GetAt(i);
		if (glyph != 0)
		{
			delete glyph;
		}
		i++;
	}
	//2. Array 치환연산자 호출
	this->glyphs = source.glyphs;
	//3. source의 glyphs 와 같은 내용을 새로 힙에 할당하고 this->glyphs에 주소를 저장함(변경)
	i = 0;
	while (i < source.length)
	{
		glyph = const_cast<Composite&>(source).glyphs.GetAt(i);
		this->glyphs.Modify(i, glyph->Clone());
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
	this->current = source.current;

	return *this;
}

//Add
Long Composite::Add(Glyph* glyph)
{
	//1. glyph링크를 입력받는다.
	//2. 사용량이 할당량보다 작으면
	Long index;
	if (this->length < this->capacity)
	{
		index = this->glyphs.Store(this->length, glyph);
	}
	//3. 그렇지 않으면
	else
	{
		index = this->glyphs.AppendFromRear(glyph);
		//3.1 할당량을 증가시킨다.
		this->capacity++;
	}
	//4. 사용량을 증가시킨다.
	this->length++;
	//5. 다음 줄 또는 다음 글자가 쓰여질 위치를 저장한다.
	this->current = index + 1;
	//6. 현재 줄의 위치 또는 현재 글자의 위치를 출력한다.
	return index;
}

//Remove
Long Composite::Remove(Long index)
{
	//1. 지울 위치를 입력받는다.
	//2. 글자나 줄을 지우고 나면 지울 위치가 다음 글자를 추가할 위치가 되기 때문에 current에 저장한다.
	this->current = index;
	//2. 해당위치의 배열요소를 지운다.
	index = this->glyphs.Delete(index);
	//3. 할당량을 감소시킨다.
	this->capacity--;
	//4. 사용량을 감소시킨다.
	this->length--;
	//5. index를 출력한다.
	return index;
}

//GetAt
Glyph* Composite::GetAt(Long index)
{
	//1. 얻을 배열요소의 위치를 입력한다.
	//2. 해당 위치의 배열요소를 출력한다.
	return this->glyphs.GetAt(index);
}