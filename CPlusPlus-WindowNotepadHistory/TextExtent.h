#ifndef _TEXTEXTENT
#define _TEXTEXTENT

#include<string>
using namespace std;
class NotepadForm;
typedef signed long int Long;

class TextExtent
{
public:
	NotepadForm* notepadForm;
public:
	TextExtent(NotepadForm* notepadForm = 0);//����Ʈ������
	//���� �ۿ��� ĳ���� ���� ���� ��ġ�� �Է¹޾Ƽ� �װ��� ������ Ȱ���� �ݺ��� �������� �ߴµ�
	//�ѱ��� �迭��� 2���� �����ϱ� ������ �̸� Ȱ���� �� ����. ���� string�� length�� �̿���
	//�迭����� ������ ���ϰ� �� length��ŭ �ݺ��� ������ �Ǽ� ���̻� count�� �Է¹��� �ʿ䰡 X
	//Long GetTexrWidth(string content, Long count);
	Long GetTextWidth(string content);
	~TextExtent();//�Ҹ���
	//�ζ��� �Լ� ����
	Long* GetWidths() const;
	Long GetHeight() const;
private:
	Long widths[129];
	Long height;
};

//�ζ����Լ� ����
inline Long* TextExtent::GetWidths() const
{
	return (Long*)this->widths;
}
inline Long TextExtent::GetHeight() const
{
	return this->height;
}

#endif // !_TEXTEXTENT
