#include "Composite.h"
#include "Row.h"

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
	:glyphs(source.glyphs)//source의 배열요소들인 주소들을 복사해옴.
	//주소들을 복사했기때문에 this의 배열요소들은 source의 배열요소들과 똑같은 내용을 가리킴.
{
	Glyph* glyph;
	Long i = 0;
	//1. source의 length보다 작은동안 반복한다.
	while (i < source.length)
	{
		//1.1 Composite의 각 배열요소의 glyph를 구한다.
		glyph = const_cast<Composite&>(source).glyphs.GetAt(i);
		//1.2 this의 배열요소들이 지금 현재는 source와 같은 내용들을 가리키고 있기 때문에
		//그 배열요소들을 힙에 새로 할당한 내용들로 바꿔준다.(깊은 복사)
		this->glyphs.Modify(i, glyph->Clone());
		//1.3 배열첨자 및 반복제어변수를 증가시킨다.
		i++;
	}
	//2. capacity를 변경한다.
	this->capacity = source.capacity;
	//3. length를 변경한다.
	this->length = source.length;
	//4. current를 변경한다.
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

//Add(Row기준)
Long Composite::Add(Glyph* glyph)
{
	//1. 추가할 글자의 주소를 입력받는다.
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
	//5. 다음 글자가 쓰여질 위치(캐럿의 현재 가로 위치)를 저장한다.(Row기준)
	this->current = index + 1;
	//6. 현재 글자의 위치를 출력한다.(Row기준)
	return index;
}

//Add(Row기준)(Insert)
Long Composite::Add(Long index, Glyph* glyph)
{
	//1. 끼워 쓸 글자의 index와 글자 주소를 입력받는다.
	//2. 글자를 배열 요소의 index번쨰에 끼워 넣는다.
	index = this->glyphs.Insert(index, glyph);
	//3. 할당량을 증가시킨다.
	this->capacity++;
	//4. 사용량을 증가시킨다.
	this->length++;
	//5. 다음 글자가 쓰여질 위치(캐럿의 현재 가로 위치)를 저장한다.(Row기준)
	this->current = index + 1;
	//6. 현재 글자의 위치를 출력한다.
	return index;
}

//Remove
Long Composite::Remove(Long index)
{
	//1. 지울 글자의 위치를 입력받는다.
	//2. 글자를 지우고 나면 지울 위치가 다음 글자를 추가할 위치(캐럿의 현재 가로 위치)가 되기 때문에 current에 저장한다.
	this->current = index;
	//3. 해당배열요소가 주소를 저장하고 있기 때문에 해당배열요소의 힙에 할당된 내용(글자)을 먼저 지워야함.
	Glyph* glyph = this->glyphs[index];
	if (glyph != 0)
	{
		delete glyph;
	}
	//4. 해당위치의 배열요소의 힙에 할당된 내용(글자)을 지웠으니 이제 그 주소를 지운다.
	index = this->glyphs.Delete(index);
	//5. 할당량을 감소시킨다.
	this->capacity--;
	//6. 사용량을 감소시킨다.
	this->length--;
	//7. index를 출력한다.
	return index;
}

//Split
Glyph* Composite::Split(Long index)
{
	//1. 현재 줄에서 분리할 위치를 입력받는다.
	//2. 새로운 줄을 생성한다.
	Glyph* row = new Row();
	//3. 현재 줄에서 분리할 글자들을 새로 만든 줄에 복사한다.(깊은 복사)
	//Long i = 0;//배열첨자
	Glyph* letter;//분리할 글자를 담을 공간
	//3.1 입력받은 위치가 현재 줄의 글자개수보다 작은동안 반복한다.
	while (index < this->GetLength())
	{
		//3.1.1 현재 줄에서 입력받은 위치의 글자를 구한다.
		letter = this->glyphs.GetAt(index);
		//3.1.2 새로 만든 줄에는 저장된 배열요소가 아무것도 없기 때문에 Modify라는 개념은 어울리지X
		//새로 만든 줄에 아무것도 배열요소가 없는 경우에 추가할 때는 Add가 어울인다.
		//새로 만든 줄에 분리할 글자들을 담는다.(깊은 복사)
		row->Add(letter->Clone());
		//row->glyphs.Modify(i, letter->Clone()); glyphs는 protected이고 Composite의 자식들만
		//이용할 수 있음, Glyph는 Composite의 자식이 아니기 때문에 glyphs에 접근할 수 X
		//3.1.3 현재 줄의 글자들 중에서 새로 만들 줄에 담은 글자를 없앤다.
		//기존 줄(this)에 있는 글자를 새로운 줄에 옮겼기 때문에(깊은 복사) 이제 기존 줄(this)에 있는
		//내용(글자)와 그 내용을 저장하고 있는 주소들을 할당해제해줘야한다.
		//Split의 뜻은 "분리하다", "떼어내다"라는 뜻이다. 그런데 새로운 줄에 내용(글자)만 새로 만들어서
		//옮기고 기존 줄에서 그 내용들을 안없애주면 "떼어내다"가 아니라 "복사하다(copy)"가 되어버린다.
		//따라서 Split연산의 의미에 맞게 새로운 줄에 기존줄의 내용들을 옮겼으면 기존줄에서는 그내용들을
		//없애줘야 떼어낸다는 개념적 의미가 맞다!
		//3.1.3.1 letter가 0이 아니면
		if (letter != 0)
		{
			//3.1.3.1.1 letter의 내용(글자)을 할당해제한다.
			delete letter;
		}
		//3.1.3.2 letter의 내용을 담고 있던 주소를 할당해제한다.
		this->glyphs.Delete(index);
		//3.1.3.3 배열에서 배열요소를 없앴기 때문에 사용량과 할당량을 감소시켜준다.
		this->capacity--;
		this->length--;
		//i++; Add를 사용하면 처음부터 순차적으로 배열요소에 저장하기 때문에 배열첨자를 따로 세줄 필요가X
		//index++;//입력받은 위치를 증가시킨다. 다음 글자를 담기 위해서
		//index를 증가시켜줄 필요가 없음 왜냐하면 Delete를 통해서 계속 배열요소가 지워지고 있기때문에
		//index는 계속 같은 위치에서 계속 다음 글자를 읽으면된다.
		//글자가 배열요소에서 지워지면서 계속 앞으로 밀려오기때문에 index는 가만히 있으면 됨!.
		//index++하면 뻑이남
	}
	//4. 새로 만든 row를 출력한다.
	return row;
	//5. 끝내다.
}

//GetAt
Glyph* Composite::GetAt(Long index)
{
	//1. 얻을 배열요소의 위치를 입력한다.
	//2. 해당 위치의 배열요소를 출력한다.
	return this->glyphs.GetAt(index);
}

//캐럿의 현재 위치 가로로 이동
//캐럿의 현재 가로(세로) 위치 맨 앞으로 이동시키기
Long Composite::First()
{
	//1. 캐럿의 현재 가로(세로) 위치를 줄의 맨 앞으로 이동시킨다.
	this->current = 0;
	//2. 캐럿의 현재 가로(세로) 위치를 출력한다.
	return this->current;
}

//캐럿의 현재 가로 위치 맨 마지막으로 이동시키기
Long Composite::Last()
{
	//1. 캐럿의 현재 가로 위치를 줄의 맨 마지막으로 이동시킨다.
	this->current = this->length;
	//2. 캐럿의 현재 가로 위치를 출력한다.
	return this->current;
}

//캐럿의 현재 가로(세로) 위치 한 칸(줄) 이전으로 이동시키기
Long Composite::Previous()
{
	//전자의 경우 캐럿의 이동에 초점이 안맞춰져있음 이동을 안하는경우가 생길수 있음.
#if 0
	//1. 캐럿의 현재 가로 위치가 0보다 크면
	if (this->current > 0)
	{
		//1.1 캐럿의 현재 가로 위치를 한 칸 이전으로 이동시킨다.
		this->current--;
	}
#endif
	//이동시킬 때는 항상 먼저 이동을 시키고, 그 다음에 underflow이면 최소값으로 변경해주자.
	//1. 캐럿의 현재 가로(세로) 위치를 한 칸(줄) 이전으로 이동시킨다.
	this->current--;
	//2. 캐럿의 현재 가로(세로) 위치가 underflow이면
	if (this->current == -1)
	{
		//2.1 캐럿의 현재 가로(세로) 위치의 최소값으로 변경한다.
		this->current = 0;
	}
	//3. 캐럿의 현재 가로(세로) 위치를 출력한다.
	return this->current;
}

//캐럿의 현재 가로 위치 한 칸 다음으로 이동시키기
Long Composite::Next()
{
	//전자의 경우 캐럿의 이동에 초점이 안맞춰져있음 이동을 안하는경우가 생길수 있음.
#if 0
	//1. 캐럿의 현재 가로 위치가 줄의 단어 개수보다 작으면
	if (this->current < this->length)
	{
		//1.1 캐럿의 현재 가로 위치를 한 칸 다음으로 이동시킨다.
		this->current++;
	}
#endif
	//이동시킬 때는 항상 먼저 이동을 시키고, 그 다음에 overflow이면 최대값으로 변경해주자.
	//1. 캐럿의 현재 가로 위치를 한 칸 다음으로 이동시킨다.
	this->current++;
	//2. 캐럿의 현재 가로 위치가 overflow이면
	if (this->current >= this->length)
	{
		//2.1 캐럿의 현재 가로 위치를 최대값으로 변경한다.
		this->current = this->length;
	}
	//3. 캐럿의 현재 가로 위치를 출력한다.
	return this->current;
}

//줄에서 캐럿을 단어단위로 오른쪽으로 이동시키기(Row기준)
Long Composite::NextWord()
{
	//1. 캐럿의 현재 가로 위치가 현재 줄의 마지막글자 위치보다 작고,(한 줄 내에서 이동이고)
	//글자가 스페이스와 탭문자가 아닌동안 반복한다.(글자가 한글, 영문, 숫자, 특수문자인 경우 반복해서 이동)
	//한 줄 내에서 이동하는거만 처리해줌. 줄에서 줄단위 이동은  Note->NextWord()에서 처리해줌!
	//(캐럿이 현재줄의 개수가 같은 경우(캐럿이 마지막 글자 뒤에 위치한 경우)는 줄에서 줄로 이동하게 되는데 
	//이는 Note->NextWord()에서 처리해줌!, 여기서는 한줄 내에서만 단어단위 이동하는거만 처리해줌!)
	string letter;//한글자를 담을 공간
	//마지막 글자 위치를 넘어가서 다음수를 읽으면 읽을 글자가 없기 때문에 뻑이남!
	Long lastPositionOfLetter = this->length - 1;//마지막 글자 위치
	//이동하기전에 이동했는지 이동하지 않았는지 체크여부를 위해 현재 위치를 저장함.(이제 필요없음)
	//Long current = this->current;
	letter = this->GetAt(this->current)->GetContent();
	while (this->current < lastPositionOfLetter && letter != " " && letter != "\t")
	{
		//1.1 캐럿의 현재 가로 위치를 1만큼 증가시킨다.
		//this->current=this->length-1일경우 이 반복조건문에 충족해서 들어오게 되고,
		//this->current++을 하면서 this->current는 this->length와 값이 같아지게 되는데
		//this->length에 위치한 글자는 없기때문에 없는 글자를 읽으면 뻑이남!
		//따라서 조건식을 this->length-1보다 작은동안 반복해야 마지막글자를 이미 읽었을 경우에
		//이 반복문을 빠져나가게 되서 없는 글자를 읽는 에러를 수행하지 않는다.
		//this->Next(); Row의 연산내에서 Row의 연산을 쓰면안됨
		this->current++;
		letter = this->GetAt(this->current)->GetContent();
	}
	//캐럿의 현재 가로 위치가 현재줄의 마지막 글자 위치와 같고, 현재 읽은 글자가 스페이스 문자(공백)와
	//탭문자가 아니면
	if (this->current == lastPositionOfLetter && letter != " " && letter != "\t")
	{
		//캐럿을 현재줄의 마지막글자보다 뒤에 위치시킨다.
		//this->Last();
		this->current = this->length;
	}
	//2. 이전의 글자가 무엇이었는지 구하기 위해 캐럿의 현재 가로 위치를 1만큼 감소시킨다. 
	//현재 가장 마지막에 읽은 글자가 탭이나 스페이스인데 그 전에 글자가 무엇인지가 중요!
	//그게 한글인지 영문, 숫자, 특수문자인지에 따라 단어단위 오른쪽 이동방식이 다르기때문에
	//this->current--;
	//3. 스페이스나 탭이 나오기 이전의 글자가 무엇이었는지 구한다.
	//letter = this->GetAt(this->current)->GetContent();
	//4. 이전 글자가 한글인 경우

	//5. 캐럿의 현재 가로 위치가  현재 줄의 마지막글자 위치보다 작고 
	//현재 글자가 탭문자인 동안 반복한다.
	while (this->current < lastPositionOfLetter && letter == "\t")
	{
		//5.1 캐럿의 현재 가로 위치를 1만큼 증가시킨다.
		//this->Next();
		this->current++;
		letter = this->GetAt(this->current)->GetContent();
	}
	//캐럿의 현재 가로 위치가 현재 줄의 마지막 글자 위치와 같고, 
	//현재 글자가 탭문자이면
	if (this->current == lastPositionOfLetter && letter == "\t")
	{
		//캐럿의 현재 가로 위치를 현재 줄의 마지막 글자보다 뒤에 위치시킨다.
		//this->Last();
		this->current = this->length;
	}
	//6. 캐럿의 현재 가로 위치가  현재 줄의 마지막글자 위치보다 작고
	//현재 글자가 공백(스페이스바)문자인동안 반복한다.
	while (this->current < lastPositionOfLetter && letter == " ")
	{
		//6.1 캐럿의 현재 가로 위치를 1만큼 증가시킨다.
		//this->Next();
		this->current++;
		letter = this->GetAt(this->current)->GetContent();
	}
	//캐럿의 현재 가로 위치가 현재 줄의 마지막 글자 위치와 같고, 
	//현재 글자가 스페이스문자(공백)이면
	if (this->current == lastPositionOfLetter && letter == " ")
	{
		//캐럿의 현재 가로 위치를 현재 줄의 마지막 글자보다 뒤에 위치시킨다.
		//this->Last();
		this->current = this->length;
	}
	//8. 현재 캐럿의 가로 위치를 출력한다.
	return this->current;
}

//줄에서 캐럿을 단어단위로 왼쪽으로 이동시키기(Row기준)
Long Composite::PreviousWord()
{
#if 0
	Long index = this->current - 1;
	string letter = this->GetAt(index)->GetContent();
	//1.3.3 현재 캐럿의 가로 위치가 0보다 크고 읽은 글자가 스페이스(공백)문자인동안 반복한다.
	while (index > 0 && letter == " ")
	{
		//1.3.3.1 현재 캐럿의 가로 위치를 이전으로 이동시킨다.
		//index = this->GetAt(this->current)->Previous();
		//index = this->current--;
		index--;
		//index = this->current;
		//1.3.3.2 현재 캐럿의 가로 위치에 해당하는 글자를 읽는다.
		//letter = this->GetAt(this->current)->GetAt(index)->GetContent();
		letter = this->GetAt(index)->GetContent();
	}
	//1.3.5 현재 캐럿의 가로 위치가 0보다 크고 읽은 글자가 탭문자인동안 반복한다.
	while (index > 0 && letter == "\t")
	{
		//1.3.5.1 현재 캐럿의 가로 위치를 이전으로 이동시킨다.
		//index = this->GetAt(this->current)->Previous();
		//index = this->current--;
		index--;
		//index = this->current;
		//1.3.5.2 현재 캐럿의 가로 위치에 해당하는 글자를 읽는다.
		//letter = this->GetAt(this->current)->GetAt(index)->GetContent();
		letter = this->GetAt(index)->GetContent();
	}
	//1.3.6 현재 캐럿의 가로 위치가 0보다 크고 읽은 글자가 스페이스(공백)문자 그리고
	//탭문자가 아닌동안 반복한다.
	while (index > 0 && letter != " " && letter != "\t")
	{
		//isStopped == true;
		//1.3.6.1 현재 캐럿의 가로 위치를 이전으로 이동시킨다.
		//index = this->GetAt(this->current)->Previous();
		//index = this->current--;
		index--;
		//index = this->current;
		//1.3.6.2 현재 캐럿의 가로 위치에 해당하는 글자를 읽는다.
		//letter = this->GetAt(this->current)->GetAt(index)->GetContent();
		letter = this->GetAt(index)->GetContent();
	}
#endif
#if 0
	//1. 캐럿의 현재 가로위치를 1만큼 감소시킨다.
	this->current--;
	string letter;
	//2. 캐럿의 현재 가로 위치가 0보다 크고 현재 글자가 스페이스 또는 탭인동안 반복한다.
	if (this->current > 0)
	{
		letter = this->GetAt(this->current)->GetContent();
	}
	while (this->current > 0 && letter == " " || letter == "\t")
	{
		//2.1 캐럿의 현재 위치를 감소시킨다.
		this->current--;
	}
	//3. 현재 캐럿의 위치가 0보다 크거나 같고 현재 글자가 스페이스가 아니고 탭도 아니면
	if (this->current >= 0)
	{
		letter = this->GetAt(this->current)->GetContent();
	}
	if (this->current >= 0 && letter != " " && letter != "\t")
	{
		if (this->current > 0)
		{
			letter = this->GetAt(this->current)->GetContent();
		}
		//3.1 캐럿의 현재 가로 위치가 0보다 크고 현재 글자가 스페이스와 탭이 아닌동안 반복한다.
		while (this->current > 0 && letter != " " && letter != "\t")
		{
			this->current--;
		}
		//3.2 현재 글자가 스페이스 또는 탭이면
		letter = this->GetAt(this->current)->GetContent();
		if (letter == " " || letter == "\t")
		{
			//3.2.1 캐럿의 현재 가로위치를 1만큼 증가시킨다.
			this->current++;
		}
	}
	//4. 현재 캐럿의 위치가 0이고 글자가 스페이스 또는 탭이면
	else if (this->current == 0 && letter == " " || letter == "\t")
	{
		//4.1 캐럿의 현재 가로 위치를 1만큼 감소시킨다.
		this->current--;
	}
	//5. 캐럿의 현재 가로 위치를 출력한다.
	return this->current;
#endif
#if 0
	//1. 캐럿의 현재 가로 위치가 현재 줄의 첫번째 글자 위치보다 크고,(한 줄 내에서 이동이고)
	//글자가 스페이스문자인동안 반복한다. 한 줄 내에서 이동하는거만 처리해줌.
	//줄에서 줄단위 이동은  Note->PreviousWord()에서 처리해줌!
	//(캐럿이 현재줄의 개수가 같은 경우(캐럿이 마지막 글자 뒤에 위치한 경우)는 줄에서 줄로 이동하게 되는데 
	//이는 Note->PreviousWord()에서 처리해줌!, 여기서는 한줄 내에서만 단어단위 이동하는거만 처리해줌!)
	string letter;//한 글자를 담을 공간
	this->Previous();//캐럿은 다음글자위치를 저장하고 있는데 이동은 왼쪽으로 하기 때문에
	//현재 해당 글자를 읽으려면 -1을 해줘야 현재 해당 글자를 읽고 이동할 수 있다.
	letter = this->GetAt(this->current)->GetContent();
	while (this->current > 0 && letter == " ")
	{
		//6..1 캐럿의 현재 가로 위치를 1만큼 감소시킨다.
		this->Previous();
		letter = this->GetAt(this->current)->GetContent();
	}
	//캐럿의 현재 가로 위치가 현재 줄의 첫번째 글자 위치와 같고, 
	//현재 글자가 스페이스문자(공백)이면
	if (this->current == 0 && letter == " ")
	{
		//캐럿의 현재 가로 위치를 현재 줄의 마지막 글자보다 뒤에 위치시킨다.
		this->Last();
	}
#endif

	//1. 현재 캐럿의 가로 위치를 감소시킨다.
	//Long index = this->GetAt(index)->Previous(); this가 Row이므로 이표현은 글자에 접근하게됨
	//Row의 연산안에서 Row의 함수를 사용하면 안됨!
	//Long index = this->current--; 이렇게 하면 index에 저장되는 값은 current가 -1이 되기 전의
	//값이 저장되고 this->current만 -1이 감소된 값이 저장되서 index와 this->current의 값이 1차이가남
	//Long previousCaretIndex = this->current;
	//bool isStopped = false;
	//Long index = this->current;
	//1.3.2 현재 캐럿의 가로 위치에 해당하는 글자를 읽는다.
	//GetAt을 두번하면 X
	//string letter = this->GetAt(this->current)->GetAt(index)->GetContent();
#if 0
	//1. 글자를 읽기 위해 현재 캐럿의 가로 위치를 1만큼 감소시킨다. 
	this->current--;
	//2. 현재 캐럿의 가로 위치에 해당하는 글자를 읽는다.
	string letter = this->GetAt(this->current)->GetContent();
	//3. 현재 캐럿의 가로 위치가 0보다 크고 읽은 글자가 스페이스(공백)문자인동안 반복한다.
	while (this->current > 0 && letter == " ")
	{
		//1.3.3.1 현재 캐럿의 가로 위치를 이전으로 이동시킨다.
		//index = this->GetAt(this->current)->Previous();
		//index = this->current--;
		this->current--;
		//index = this->current;
		//1.3.3.2 현재 캐럿의 가로 위치에 해당하는 글자를 읽는다.
		//letter = this->GetAt(this->current)->GetAt(index)->GetContent();
		letter = this->GetAt(this->current)->GetContent();
	}
	//1.3.5 현재 캐럿의 가로 위치가 0보다 크고 읽은 글자가 탭문자인동안 반복한다.
	while (this->current > 0 && letter == "\t")
	{
		//1.3.5.1 현재 캐럿의 가로 위치를 이전으로 이동시킨다.
		//index = this->GetAt(this->current)->Previous();
		//index = this->current--;
		this->current--;
		//index = this->current;
		//1.3.5.2 현재 캐럿의 가로 위치에 해당하는 글자를 읽는다.
		//letter = this->GetAt(this->current)->GetAt(index)->GetContent();
		letter = this->GetAt(this->current)->GetContent();
	}
	//1.3.6 현재 캐럿의 가로 위치가 0보다 크고 읽은 글자가 스페이스(공백)문자 그리고
	//탭문자가 아닌동안 반복한다.
	while (this->current > 0 && letter != " " && letter != "\t")
	{
		isStopped == true;
		//1.3.6.1 현재 캐럿의 가로 위치를 이전으로 이동시킨다.
		//index = this->GetAt(this->current)->Previous();
		//index = this->current--;
		this->current--;
		//index = this->current;
		//1.3.6.2 현재 캐럿의 가로 위치에 해당하는 글자를 읽는다.
		//letter = this->GetAt(this->current)->GetAt(index)->GetContent();
		letter = this->GetAt(this->current)->GetContent();
	}
	//1.3.7 현재 캐럿의 가로 위치가 0보다 크면
#endif

	//1. 현재 캐럿의 가로 위치 이전에 해당하는 글자를 읽는다.
	string letter = this->GetAt(this->current - 1)->GetContent();
	//2. 현재 캐럿의 가로 위치가 1보다 크고 읽은 글자가 스페이스(공백)문자인동안 반복한다.
	while (this->current > 1 && letter == " ")
	{
		//2.1 현재 캐럿의 가로 위치를 한칸 이전으로 이동시킨다.
		this->current--;
		//2.2 현재 캐럿의 가로 위치 이전에 해당하는 글자를 읽는다.
		letter = this->GetAt(this->current - 1)->GetContent();
	}
	//3. 현재 캐럿의 가로 위치가 1이고 읽은 글자가 스페이스(공백)문자이면
	if (this->current == 1 && letter == " ")
	{
		//3.1 현재 캐럿의 가로 위치를 0으로 이동시킨다.
		this->current = 0;
	}
	//4. 현재 캐럿의 가로 위치가 1보다 크고 읽은 글자가 탭문자인동안 반복한다.
	while (this->current > 1 && letter == "\t")
	{
		//4.1 현재 캐럿의 가로 위치를 한칸 이전으로 이동시킨다.
		this->current--;
		//4.2 현재 캐럿의 가로 위치 이전에 해당하는 글자를 읽는다.
		letter = this->GetAt(this->current - 1)->GetContent();
	}
	//5. 현재 캐럿의 가로 위치가 1이고 읽은 글자가 탭문자이면
	if (this->current == 1 && letter == "\t")
	{
		//5.1 현재 캐럿의 가로 위치를 0으로 이동시킨다.
		this->current = 0;
	}
	//6. 현재 캐럿의 가로 위치가 1보다 크고 읽은 글자가 스페이스(공백)문자 그리고
	//탭문자가 아닌동안 반복한다.
	while (this->current > 1 && letter != " " && letter != "\t")
	{
		//6.1 현재 캐럿의 가로 위치를 이전으로 이동시킨다.
		this->current--;
		//6.2 현재 캐럿의 가로 위치에 해당하는 글자를 읽는다.
		letter = this->GetAt(this->current - 1)->GetContent();
	}
	//7. 현재 캐럿의 가로 위치가 1이고 읽은 글자가 스페이스(공백) 문자와 탭문자가 아니면
	if (this->current == 1 && letter != " " && letter != "\t")
	{
		//7.1 현재 캐럿의 가로 위치를 0으로 이동시킨다.
		this->current = 0;
	}
	//8. 현재 캐럿의 가로 위치를 출력한다.
	return this->current;
}