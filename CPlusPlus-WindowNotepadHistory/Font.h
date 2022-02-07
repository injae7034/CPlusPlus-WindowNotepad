#ifndef _FONT_H
#define _FONT_H

typedef signed long int Long;
//#include<string>
#include<afxwin.h>//COLORREF를 사용하기 위해
//using namespace std;

class Font
{
public:
	Font();//디폴트생성자
	Font(LOGFONT logFont, Long size, COLORREF color);//매개변수를 2개 생성자
	Font(const Font& source);//복사생성자
	LOGFONT FindPrintingLogFont(CDC* printerDC);
	LOGFONT FindPreviewLogFont(CDC* previewDC);
	~Font();//소멸자
	Font& operator=(const Font& source);//치환연산자
	//인라인함수
	LOGFONT& GetLogFont() const;
	Long GetSize() const;
	//string& GetFaceName() const;
	COLORREF GetColor() const;
private:
	LOGFONT logFont;
	Long size;
	COLORREF color;
};

//인라인함수정의
inline LOGFONT& Font::GetLogFont() const
{
	return const_cast<LOGFONT&>(this->logFont);
}

inline Long Font::GetSize() const
{
	return this->size;
}

inline COLORREF Font::GetColor() const
{
	return this->color;
}
#endif // !_FONT_H

