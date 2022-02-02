#ifndef _GLYPH_H
#define _GLYPH_H

#include<string>
typedef signed long int Long;

using namespace std;

class Glyph//interface(Component)이므로 복사생성자나 치환연산자가 필요없음.
{
public:
	Glyph();//디폴트생성자
	virtual Long Add(Glyph* glyph);
	virtual Long Remove(Long index);
	virtual Glyph* GetAt(Long index);
	virtual ~Glyph() = 0;//추상클래스 소멸자
	virtual Glyph* Clone() = 0;//Prototype Pattern Clone
	virtual string GetContent() = 0;//dynamic_cast나 형변환을 안하기 위해서 만듬
	//SingleByteLetter랑 DoubleByteLetter에서도 만들어서 main에서 형변환을 안하게 만들어줘야함.
	//Glyph에 GetScript가 없기때문에 이런일이 발생함 GetScript는 반환값이 single이랑 double
	//두개가 달라서 여기서 GetScript를 정의할 수 없기때문에 나중에 main함수에서 Glyph*자료형일때는
	//GetScript를 사용할 수 없다
	virtual string GetPartOfContent(Long current);
	virtual Long First();
	virtual Long Last();
	virtual Long Previous();
	virtual Long Next();
	virtual Long NextWord();
	virtual Long PreviousWord();
	//인라인함수
	//여기서 실제로는 안쓰이고 자식에게 넘겨주는 역할을 하기때문에 virtual을 붙여줘야함
	virtual Long GetCapacity() const;
	virtual Long GetLength() const;
	virtual Long GetCurrent() const;
};

//인라인함수정의
//GetScript는 반환값이 char이나 char*로 다르기때문에 여기서 인라인함수정의를 하지X
inline Long Glyph::GetCapacity() const
{
	return -1;//Glyph에서는 처리할게 없음 Composite가서 처리하면 됨.
}
inline Long Glyph::GetLength() const
{
	return -1;//Glyph에서는 처리할게 없음 Composite가서 처리하면 됨.
}
inline Long Glyph::GetCurrent() const
{
	return -1;
}

#endif // !_GLYPH_H
