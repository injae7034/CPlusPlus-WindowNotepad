#ifndef _ROWTEXTOUTVISITOR_H
#define _ROWTEXTOUTVISITOR_H

#include "TextOutVisitor.h"
class Glyph;

class RowTextOutVisitor :public TextOutVisitor
{
public:
	RowTextOutVisitor(NotepadForm* notepadForm = 0);//����Ʈ������
	virtual void VisitRowTextOut(Glyph* row);
	virtual void VisitLetterTextOut(Glyph* letter);
	virtual ~RowTextOutVisitor();//�Ҹ���
};

#endif // !_ROWTEXTOUTVISITOR_H
