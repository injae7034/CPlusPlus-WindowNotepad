#ifndef _TEXTINGOUTVISITOR_H
#define _TEXTINGOUTVISITOR_H

#include "GlyphVisitor.h"

class TextingOutVisitor :public GlyphVisitor
{
public:
	TextingOutVisitor(NotepadForm* notepadForm, CDC* dc,
		Long glyphXPos, Long glyphYPos);//디폴트생성자
	TextingOutVisitor(const TextingOutVisitor& source);//복사생성자
	TextingOutVisitor& operator=(const TextingOutVisitor& source);//치환연산자
	virtual void VisitNote(Glyph* note);
	virtual void VisitRow(Glyph* row);
	virtual void VisitDummyRow(Glyph* dummyRow);
	virtual void VisitSingleByteLetter(Glyph* singleByteLetter);
	virtual void VisitDoubleByteLetter(Glyph* doubleByteLetter);
	~TextingOutVisitor();//소멸자
};

#endif // !_TEXTINGOUTVISITOR_H