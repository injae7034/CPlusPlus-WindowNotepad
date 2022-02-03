#ifndef _GLYPH_H
#define _GLYPH_H

#include<string>
typedef signed long int Long;

using namespace std;
class GlyphVisitor;

class Glyph//interface(Component)�̹Ƿ� ��������ڳ� ġȯ�����ڰ� �ʿ����.
{
public:
	Glyph();//����Ʈ������
	virtual Long Add(Glyph* glyph) { return -1; }
	virtual Long Add(Long index, Glyph* glyph) { return -1; }//Insert
	virtual Long Remove(Long index) { return -1; }
	virtual Glyph* Split(Long index, bool isDummyRow = false) { return 0; }
	virtual void Join(Glyph* row) {}
	virtual Glyph* GetAt(Long index) { return 0; }
	virtual ~Glyph() = 0;//�߻�Ŭ���� �Ҹ���
	virtual Glyph* Clone() = 0;//Prototype Pattern Clone
	virtual string GetContent() = 0;//dynamic_cast�� ����ȯ�� ���ϱ� ���ؼ� ����
	//SingleByteLetter�� DoubleByteLetter������ ���� main���� ����ȯ�� ���ϰ� ����������.
	//Glyph�� GetScript�� ���⶧���� �̷����� �߻��� GetScript�� ��ȯ���� single�̶� double
	//�ΰ��� �޶� ���⼭ GetScript�� ������ �� ���⶧���� ���߿� main�Լ����� Glyph*�ڷ����϶���
	//GetScript�� ����� �� ����
	virtual string GetPartOfContent(Long current) { return "\0"; }
	//Glyph*���� ����Ϸ��� ���� Glyph ��� ���Ͽ� �־���Ѵ�.
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
	//�ζ����Լ�(���⿡ �ڽĵ��� �ζ����Լ��� ������ �ؾ� Glyph*���� ����� �� ����)
	//(�ֳ��ϸ� ���� Glyph*�� �����ؼ� �̿��ϱ� ������!)
	//���⼭ �����δ� �Ⱦ��̰� �ڽĿ��� �Ѱ��ִ� ������ �ϱ⶧���� virtual�� �ٿ������
	virtual bool IsSelected() const { return false; }
	virtual Long GetCapacity() const { return -1; }
	virtual Long GetLength() const { return -1; }
	virtual Long GetCurrent() const { return -1; }
	virtual void Accept(GlyphVisitor* glyphVisitor) {}
};

#endif // !_GLYPH_H
