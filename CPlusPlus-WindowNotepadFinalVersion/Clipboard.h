#ifndef _CLIPBOARD_H
#define _CLIPBOARD_H

#include "Composite.h"

class Clipboard :public Composite
{
public:
	Clipboard(Long capacity = 256);//����Ʈ������
	Clipboard(const Clipboard& source);//���������
	Clipboard& operator=(const Clipboard& source);//ġȯ������
	virtual ~Clipboard();//�Ҹ���
	virtual Glyph* Clone();//Prototype Pattern Clone
	virtual Long Add(Glyph* glyph);//������ ������ ����Ʈ�� ������.
	virtual Long Remove(Long index);//������ ������ ����Ʈ���� ����.
	virtual string GetContent();
	virtual Long RemoveAll();
};

#endif // !_CLIPBOARD_H
