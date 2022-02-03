#ifndef _TEXTOUTVISITOR_H
#define _TEXTOUTVISITOR_H

class Glyph;
class NotepadForm;

class TextOutVisitor
{
public:
	NotepadForm* notepadForm;
public:
	TextOutVisitor(NotepadForm* notepadForm = 0);//디폴트생성자
	virtual void VisitRowTextOut(Glyph* row) = 0;
	virtual void VisitLetterTextOut(Glyph* letter) = 0;
	virtual ~TextOutVisitor() = 0;//소멸자
};

#endif // !_TEXTOUTVISITOR_H
