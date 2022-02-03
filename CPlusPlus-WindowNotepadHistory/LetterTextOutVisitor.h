#ifndef _LETTERTEXTOUTVISITOR_H
#define _LETTERTEXTOUTVISITOR_H

#include "TextOutVisitor.h"
class Glyph;

class LetterTextOutVisitor :public TextOutVisitor
{
public:
	LetterTextOutVisitor(NotepadForm* notepadForm = 0);//디폴트생성자
	virtual void VisitRowTextOut(Glyph* row);
	virtual void VisitLetterTextOut(Glyph* letter);
	virtual ~LetterTextOutVisitor();//소멸자
};

#endif // !_LETTERTEXTOUTVISITOR_H
