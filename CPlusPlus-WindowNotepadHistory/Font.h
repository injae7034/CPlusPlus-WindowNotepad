#ifndef _FONT_H
#define _FONT_H

typedef signed long int Long;
#include<string>
using namespace std;

class Font
{
public:
	Font();//디폴트생성자
	Font(Long size, string style);//매개변수를 2개 생성자
	Font(const Font& source);//복사생성자
	~Font();//소멸자
	Font& operator=(const Font& source);//치환연산자
	//인라인함수
	Long Getsize() const;
	string& GetStyle() const;
private:
	Long size;
	string style;
};

//인라인함수정의
inline Long Font::Getsize() const
{
	return this->size;
}
inline string& Font::GetStyle() const
{
	return const_cast<string&>(this->style);
}

#endif // !_FONT_H

