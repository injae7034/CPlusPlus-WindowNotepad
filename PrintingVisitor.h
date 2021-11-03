#ifndef _PRINTINGVISITOR_H
#define _PRINTINGVISITOR_H

#include "GlyphVisitor.h"

class PrintingVisitor :public GlyphVisitor
{
public:
	PrintingVisitor(NotepadForm* notepadForm, CDC* dc,
		Long glyphXPos, Long glyphYPos);//����Ʈ������
	PrintingVisitor(const PrintingVisitor& source);//���������
	PrintingVisitor& operator=(const PrintingVisitor& source);//ġȯ������
	virtual void VisitNote(Glyph* note);
	virtual void VisitRow(Glyph* row);
	virtual void VisitDummyRow(Glyph* dummyRow);
	virtual void VisitSingleByteLetter(Glyph* singleByteLetter);
	virtual void VisitDoubleByteLetter(Glyph* doubleByteLetter);
	~PrintingVisitor();//�Ҹ���
};
#endif // !_PRINTINGVISITOR_H
