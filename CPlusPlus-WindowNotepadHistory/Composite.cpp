#include "Composite.h"

//디폴트생성자
Composite::Composite(Long capacity)
	:glyphs(capacity)
{
	this->capacity = capacity;
	this->length = 0;
}

//소멸자
//Composite는 주소배열(Array<Glyph*>)을 가지고 있으므로 Composite을 바로 소멸하기 전에
//Composite의 멤버로 가지고 있는 주소배열의 각 배열요소가 힙에 할당된 내용(Row일수도 있고,
//SingleByteLetter이거나 DoubleByteLetter일수도 있음)을 먼저 할당해제를 다 시키고 나서
//자기 자신(Composite)을 할당해제시켜야함! 그래서 소멸자에서 반복구조만큼 각 배열요소를 할당해제처리해줘야함!
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
	:glyphs(source.glyphs)//먼저 source의 내용들의 주소를 복사함.(얕은복사)
	//this의 각 배열요소들(주소들)이 source의 각 배열요소들(주소들)과 동일한 내용(힙에할당됨)을 가리킴.
	//얕은 복사의 경우 만약 source의 소멸자를 호출하면 this는 소멸자를 호출하지 않았음에도 불구하고
	//this는 댕글링포인터의 배열요소들만 가지고 있게 되버림. 왜냐하면 source의 소멸자에서 source의
	//배열요소들이 가리키고 있는 힙에 할당된 내용들을 전부 할당해제해버렸기때문에 this의 각 배열요소도 
	//source의 각 배열요소와 동일한 내용을 가리키고 있었기 때문에 this는 할당해제된 곳을 가리키는 주소값
	//댕글링포인터들의 배열요소를 가지고 있게 되기 때문에 진정한 의미에서의 복사라고 할 수 없음
	//그리고 나중에 프로그램을 가동할 때 이것 때문에 에러가 발생할 확률이 높음 그래서 깊은 복사를 해야함!
{
	Glyph* glyph;
	Long i = 0;
	//source의 개수만큼 반복함
	while (i < source.length)
	{
		//source는 this가 아닌데 source가 연산을 사용하기 위해서 const_cast<Composite&>을 사용해야함!
		//source 각 배열요소의 내용들(주소들)을 차례차례 얻어옴.
		glyph = const_cast<Composite&>(source).glyphs.GetAt(i);
		//this의 각 배열요소가 현재는 source의 각 배열요소(주소들)이 가리키는 내용들을 동일하게 가리킴
		//그걸 바꾸는데 바꾸는 게 source의 각 배열요소(주소들)이 가리키는 내용들을 동일하게 복사해서
		//새로 힙에 할당하고 그 주소를 this의 각 배열요소(주소들)들에 저장함!
		//이렇게 깊은 복사를 하고 나면 만약에 source가 소멸하더라도 this는 아무 영향이 없이
		//this의 배열요소들은 자기들만의 힙에 할당된 내용을 가지고 있어서 아무 문제가 없다.
		this->glyphs.Modify(i, glyph->Clone());
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
}

//치환연산자
Composite& Composite::operator=(const Composite& source)
{
	Glyph* glyph;
	Long i = 0;
	//1. 기존의 this(Composite)의 배열요소들(주소들)의 힙에 할당된 내용 및 주소들의배열 할당해제
	while (i < this->length)
	{
		glyph = this->glyphs.GetAt(i);
		if (glyph != 0)
		{
			delete glyph;
		}
		i++;
	}
	//2. Array 치환연산자 호출(얕은 복사)(주소들의 배열만 복사해옴)(힙에 할당된 동일한 내용들을 가리킴)
	this->glyphs = source.glyphs;
	//3. source의 glyphs와 같은 내용을 새로 힙에 할당하고 this->glyphs에 주소를 저장함(변경)(깊은복사)
	i = 0;
	while (i < source.length)
	{
		glyph = const_cast<Composite&>(source).glyphs.GetAt(i);
		this->glyphs.Modify(i, glyph->Clone());
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;

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
		//2.1 배열에 순차적으로 힙에 할당된 glyph(내용)의 주소를 저장한다.
		index = this->glyphs.Store(this->length, glyph);
	}
	//3. 그렇지 않으면
	else
	{
		//3.1 배열에 더이상 저장할 공간이 없기때문에 마지막 한칸을 늘려서 glyph를 저장한다. 
		index = this->glyphs.AppendFromRear(glyph);
		//3.2 할당량을 증가시킨다.
		this->capacity++;
	}
	//4. 사용량을 증가시킨다.
	this->length++;
	//5. 위치를 출력한다.
	return index;
}

//Remove
Long Composite::Remove(Long index)
{
	//1. 지울 위치를 입력받는다.
	//2. 해당배열요소가 주소를 저장하고 있기 때문에 해당배열요소의 힙에 할당된 내용을 먼저 지워야함.
	Glyph* glyph = this->glyphs[index];
	if (glyph != 0)
	{
		delete glyph;
	}
	//3. 해당위치의 배열요소의 힙에 할당된 내용을 지웠으니 이제 그 주소를 지운다.
	index = this->glyphs.Delete(index);
	//4. 할당량을 감소시킨다.
	this->capacity--;
	//5. 사용량을 감소시킨다.
	this->length--;
	//6. index를 출력한다.
	return index;
}

//GetAt
Glyph* Composite::GetAt(Long index)
{
	//1. 얻을 배열요소의 위치를 입력한다.
	//2. 해당 위치의 배열요소를 출력한다.
	return this->glyphs.GetAt(index);
}