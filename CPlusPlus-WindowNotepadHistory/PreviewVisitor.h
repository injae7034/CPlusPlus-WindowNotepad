#ifndef _PREVIEWVISITOR_H
#define _PREVIEWVISITOR_H

#include "GlyphVisitor.h"

typedef signed long int Long;

class PreviewVisitor :public GlyphVisitor
{
public:
	PreviewVisitor(NotepadForm* notepadForm, CDC* dc,
		Long glyphXPos, Long glyphYPos);//����Ʈ������
	PreviewVisitor(const PreviewVisitor& source);//���������
	PreviewVisitor& operator=(const PreviewVisitor& source);//ġȯ������
	virtual void VisitNote(Glyph* note);
	virtual void VisitRow(Glyph* row);
	virtual void VisitDummyRow(Glyph* dummyRow);
	virtual void VisitSingleByteLetter(Glyph* singleByteLetter);
	virtual void VisitDoubleByteLetter(Glyph* doubleByteLetter);
	~PreviewVisitor();//�Ҹ���
private:
	CDC* tempDC;
	Long maxRowWidth;
	bool isRowWidthOverRectWidth;
};
#endif // !_PREVIEWVISITOR_H
