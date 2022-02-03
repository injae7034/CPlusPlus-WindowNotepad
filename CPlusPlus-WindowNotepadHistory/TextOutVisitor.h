#ifndef _TEXTOUTVISITOR_H
#define _TEXTOUTVISITOR_H

class Glyph;
class NotepadForm;

class TextOutVisitor
{
public:
	NotepadForm* notepadForm;
public:
	TextOutVisitor(NotepadForm* notepadForm = 0);//����Ʈ������
	virtual void VisitRowTextOut(Glyph* row) = 0;
	virtual void VisitLetterTextOut(Glyph* letter) = 0;
	virtual ~TextOutVisitor() = 0;//�Ҹ���
};

#endif // !_TEXTOUTVISITOR_H
