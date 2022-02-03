#ifndef _SINGLEBYTELETTER_H
#define _SINGLEBYTELETTER_H

#include "Letter.h"
#include "Glyph.h"

class SingleByteLetter :public Letter
{
public:
	SingleByteLetter();//����Ʈ������
	SingleByteLetter(char script);//�Ű�����1��������
	SingleByteLetter(const SingleByteLetter& source);//���������
	SingleByteLetter& operator=(const SingleByteLetter& source);//ġȯ������
	virtual ~SingleByteLetter();//�Ҹ���
	virtual Glyph* Clone();//Prototype Pattern Clone
	virtual string GetContent();
	virtual void Accept(GlyphVisitor* glyphVisitor);
	//�ζ����Լ�
	char GetScript() const;

private:
	char script;
};

//�ζ����Լ� ����
inline char SingleByteLetter::GetScript() const
{
	return this->script;
}

#endif // !_SINGLEBYTELETTER_H