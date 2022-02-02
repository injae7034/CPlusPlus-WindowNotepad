#ifndef _GLYPH_H
#define _GLYPH_H

#include<string>
typedef signed long int Long;

using namespace std;

class Glyph//interface(Component)�̹Ƿ� ��������ڳ� ġȯ�����ڰ� �ʿ����.
{
public:
	Glyph();//����Ʈ������
	virtual Long Add(Glyph* glyph);
	virtual Long Remove(Long index);
	virtual Glyph* GetAt(Long index);
	virtual ~Glyph() = 0;//�߻�Ŭ���� �Ҹ���
	virtual Glyph* Clone() = 0;//Prototype Pattern Clone
	virtual string GetContent() = 0;//dynamic_cast�� ����ȯ�� ���ϱ� ���ؼ� ����
	//SingleByteLetter�� DoubleByteLetter������ ���� main���� ����ȯ�� ���ϰ� ����������.
	//Glyph�� GetScript�� ���⶧���� �̷����� �߻��� GetScript�� ��ȯ���� single�̶� double
	//�ΰ��� �޶� ���⼭ GetScript�� ������ �� ���⶧���� ���߿� main�Լ����� Glyph*�ڷ����϶���
	//GetScript�� ����� �� ����
	virtual string GetPartOfContent(Long current);
	virtual Long First();
	virtual Long Last();
	virtual Long Previous();
	virtual Long Next();
	virtual Long NextWord();
	virtual Long PreviousWord();
	//�ζ����Լ�
	//���⼭ �����δ� �Ⱦ��̰� �ڽĿ��� �Ѱ��ִ� ������ �ϱ⶧���� virtual�� �ٿ������
	virtual Long GetCapacity() const;
	virtual Long GetLength() const;
	virtual Long GetCurrent() const;
};

//�ζ����Լ�����
//GetScript�� ��ȯ���� char�̳� char*�� �ٸ��⶧���� ���⼭ �ζ����Լ����Ǹ� ����X
inline Long Glyph::GetCapacity() const
{
	return -1;//Glyph������ ó���Ұ� ���� Composite���� ó���ϸ� ��.
}
inline Long Glyph::GetLength() const
{
	return -1;//Glyph������ ó���Ұ� ���� Composite���� ó���ϸ� ��.
}
inline Long Glyph::GetCurrent() const
{
	return -1;
}

#endif // !_GLYPH_H
