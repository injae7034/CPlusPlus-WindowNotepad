#ifndef _SELECTINGVISITOR_H
#define _SELECTINGVISITOR_H

#include "GlyphVisitor.h"

class SelectingVisitor :public GlyphVisitor
{
public:
	SelectingVisitor(NotepadForm* notepadForm, CDC* dc,
		Long glyphXPos, Long glyphYPos);//����Ʈ������
	SelectingVisitor(const SelectingVisitor& source);//���������
	SelectingVisitor& operator=(const SelectingVisitor& source);//ġȯ������
	virtual void VisitNote(Glyph* note);
	virtual void VisitRow(Glyph* row);
	virtual void VisitDummyRow(Glyph* dummyRow);
	virtual void VisitSingleByteLetter(Glyph* singleByteLetter);
	virtual void VisitDoubleByteLetter(Glyph* doubleByteLetter);
	~SelectingVisitor();//�Ҹ���
};

#endif // !_SELECTINGVISITOR_H
