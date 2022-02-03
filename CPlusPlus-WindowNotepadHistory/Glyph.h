#ifndef _GLYPH_H
#define _GLYPH_H

#include<string>
typedef signed long int Long;

using namespace std;
class GlyphVisitor;

class Glyph//interface(Component)이므로 복사생성자나 치환연산자가 필요없음.
{
public:
	Glyph();//디폴트생성자
	virtual Long Add(Glyph* glyph) { return -1; }
	virtual Long Add(Long index, Glyph* glyph) { return -1; }//Insert
	virtual Long Remove(Long index) { return -1; }
	virtual Glyph* Split(Long index, bool isDummyRow = false) { return 0; }
	virtual void Join(Glyph* row) {}
	virtual Glyph* GetAt(Long index) { return 0; }
	virtual ~Glyph() = 0;//추상클래스 소멸자
	virtual Glyph* Clone() = 0;//Prototype Pattern Clone
	virtual string GetContent() = 0;//dynamic_cast나 형변환을 안하기 위해서 만듬
	//SingleByteLetter랑 DoubleByteLetter에서도 만들어서 main에서 형변환을 안하게 만들어줘야함.
	//Glyph에 GetScript가 없기때문에 이런일이 발생함 GetScript는 반환값이 single이랑 double
	//두개가 달라서 여기서 GetScript를 정의할 수 없기때문에 나중에 main함수에서 Glyph*자료형일때는
	//GetScript를 사용할 수 없다
	virtual string GetPartOfContent(Long current) { return "\0"; }
	//Glyph*에서 사용하려면 전부 Glyph 헤더 파일에 있어야한다.
	virtual void Select(bool isSelected) {}
	virtual void CalculateSelectedRange(Long* startingRowPos, Long* startingLetterPos,
		Long* endingRowPos, Long* endingLetterPos) {}
	virtual Long First() { return 0; }
	virtual Long Last() { return 0; }
	virtual Long Previous() { return 0; }
	virtual Long Next() { return 0; }
	virtual Long Move(Long index) { return 0; }
	virtual Long NextWord() { return 0; }
	virtual Long PreviousWord() { return 0; }
	//인라인함수(여기에 자식들의 인라인함수도 선언을 해야 Glyph*에서 사용할 수 있음)
	//(왜냐하면 전부 Glyph*로 생성해서 이용하기 때문에!)
	//여기서 실제로는 안쓰이고 자식에게 넘겨주는 역할을 하기때문에 virtual을 붙여줘야함
	virtual bool IsSelected() const { return false; }
	virtual Long GetCapacity() const { return -1; }
	virtual Long GetLength() const { return -1; }
	virtual Long GetCurrent() const { return -1; }
	virtual void Accept(GlyphVisitor* glyphVisitor) {}
};

#endif // !_GLYPH_H
