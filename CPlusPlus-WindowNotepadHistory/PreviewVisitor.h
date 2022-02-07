#ifndef _PREVIEWVISITOR_H
#define _PREVIEWVISITOR_H

#include "GlyphVisitor.h"

typedef signed long int Long;

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
	Long maxRowWidth;
	bool isRowWidthOverRectWidth;
};
#endif // !_PREVIEWVISITOR_H
