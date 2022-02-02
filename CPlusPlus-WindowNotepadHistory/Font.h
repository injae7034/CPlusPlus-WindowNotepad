#ifndef _FONT_H
#define _FONT_H

typedef signed long int Long;
#include<string>
#include<afxwin.h>//COLORREF를 사용하기 위해
using namespace std;

class Font
{
public:
	Font();//디폴트생성자
	Font(LOGFONT logFont, COLORREF color);//매개변수를 4개 생성자
	Font(const Font& source);//복사생성자
	~Font();//소멸자
	Font& operator=(const Font& source);//치환연산자
	//인라인함수
	LOGFONT GetLogFont() const;
	//Long GetSize() const;
	//string& GetFaceName() const;
	COLORREF GetColor() const;
private:
	LOGFONT logFont;
	//Long size;
	//string faceName;
	COLORREF color;
};

//인라인함수정의
inline LOGFONT Font::GetLogFont() const
{
	return const_cast<LOGFONT&>(this->logFont);
}
#if 0
inline Long Font::GetSize() const
{
	return this->size;
}
inline string& Font::GetFaceName() const
{
	return const_cast<string&>(this->faceName);
}
#endif
inline COLORREF Font::GetColor() const
{
	return this->color;
}
#endif // !_FONT_H

