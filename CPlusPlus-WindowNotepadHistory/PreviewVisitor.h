#ifndef _PREVIEWVISITOR_H
#define _PREVIEWVISITOR_H

#include "GlyphVisitor.h"

class PreviewVisitor :public GlyphVisitor
{
public:
	PreviewVisitor(NotepadForm* notepadForm, CDC* dc,
		Long glyphXPos, Long glyphYPos);//디폴트생성자
	PreviewVisitor(const PreviewVisitor& source);//복사생성자
	PreviewVisitor& operator=(const PreviewVisitor& source);//치환연산자
	virtual void VisitNote(Glyph* note);
	virtual void VisitRow(Glyph* row);
	virtual void VisitDummyRow(Glyph* dummyRow);
	virtual void VisitSingleByteLetter(Glyph* singleByteLetter);
	virtual void VisitDoubleByteLetter(Glyph* doubleByteLetter);
	~PreviewVisitor();//소멸자
private:
	CDC* tempDC;
};
#endif // !_PREVIEWVISITOR_H
