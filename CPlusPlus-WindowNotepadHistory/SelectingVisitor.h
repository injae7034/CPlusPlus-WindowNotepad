#ifndef _SELECTINGVISITOR_H
#define _SELECTINGVISITOR_H

#include "GlyphVisitor.h"

class SelectingVisitor :public GlyphVisitor
{
public:
	SelectingVisitor(NotepadForm* notepadForm, CDC* dc,
		Long glyphXPos, Long glyphYPos);//디폴트생성자
	SelectingVisitor(const SelectingVisitor& source);//복사생성자
	SelectingVisitor& operator=(const SelectingVisitor& source);//치환연산자
	virtual void VisitNote(Glyph* note);
	virtual void VisitRow(Glyph* row);
	virtual void VisitDummyRow(Glyph* dummyRow);
	virtual void VisitSingleByteLetter(Glyph* singleByteLetter);
	virtual void VisitDoubleByteLetter(Glyph* doubleByteLetter);
	~SelectingVisitor();//소멸자
};

#endif // !_SELECTINGVISITOR_H
