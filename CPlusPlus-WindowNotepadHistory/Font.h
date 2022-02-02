#ifndef _FONT_H
#define _FONT_H

typedef signed long int Long;
#include<string>
#include<afxwin.h>//COLORREF�� ����ϱ� ����
using namespace std;

class Font
{
public:
	Font();//����Ʈ������
	Font(LOGFONT logFont, COLORREF color);//�Ű������� 4�� ������
	Font(const Font& source);//���������
	~Font();//�Ҹ���
	Font& operator=(const Font& source);//ġȯ������
	//�ζ����Լ�
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

//�ζ����Լ�����
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

