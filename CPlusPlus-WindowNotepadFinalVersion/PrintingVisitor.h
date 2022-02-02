#ifndef _PRINTINGVISITOR_H
#define _PRINTINGVISITOR_H

#include "GlyphVisitor.h"

class PrintingVisitor :public GlyphVisitor
{
public:
	PrintingVisitor(NotepadForm* notepadForm, CDC* dc,
		Long glyphXPos, Long glyphYPos);//디폴트생성자
	PrintingVisitor(const PrintingVisitor& source);//복사생성자
	PrintingVisitor& operator=(const PrintingVisitor& source);//치환연산자
	virtual void VisitNote(Glyph* note);
	virtual void VisitRow(Glyph* row);
	virtual void VisitDummyRow(Glyph* dummyRow);
	virtual void VisitSingleByteLetter(Glyph* singleByteLetter);
	virtual void VisitDoubleByteLetter(Glyph* doubleByteLetter);
	~PrintingVisitor();//소멸자
};
#endif // !_PRINTINGVISITOR_H
