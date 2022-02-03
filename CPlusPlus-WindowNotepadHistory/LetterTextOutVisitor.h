#ifndef _LETTERTEXTOUTVISITOR_H
#define _LETTERTEXTOUTVISITOR_H

#include "TextOutVisitor.h"
class Glyph;

class LetterTextOutVisitor :public TextOutVisitor
{
public:
	LetterTextOutVisitor(NotepadForm* notepadForm = 0);//����Ʈ������
	virtual void VisitRowTextOut(Glyph* row);
	virtual void VisitLetterTextOut(Glyph* letter);
	virtual ~LetterTextOutVisitor();//�Ҹ���
};

#endif // !_LETTERTEXTOUTVISITOR_H
