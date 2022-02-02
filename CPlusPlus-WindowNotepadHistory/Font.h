#ifndef _FONT_H
#define _FONT_H

typedef signed long int Long;
#include<string>
using namespace std;

class Font
{
public:
	Font();//����Ʈ������
	Font(Long size, string style);//�Ű������� 2�� ������
	Font(const Font& source);//���������
	~Font();//�Ҹ���
	Font& operator=(const Font& source);//ġȯ������
	//�ζ����Լ�
	Long Getsize() const;
	string& GetStyle() const;
private:
	Long size;
	string style;
};

//�ζ����Լ�����
inline Long Font::Getsize() const
{
	return this->size;
}
inline string& Font::GetStyle() const
{
	return const_cast<string&>(this->style);
}

#endif // !_FONT_H

