#ifndef _TEXTINGOUTVISITOR_H
#define _TEXTINGOUTVISITOR_H

#include "GlyphVisitor.h"

class TextingOutVisitor :public GlyphVisitor
{
public:
	TextingOutVisitor(NotepadForm* notepadForm, CDC* dc,
		Long glyphXPos, Long glyphYPos);//����Ʈ������
	TextingOutVisitor(const TextingOutVisitor& source);//���������
	TextingOutVisitor& operator=(const TextingOutVisitor& source);//ġȯ������
	virtual void VisitNote(Glyph* note);
	virtual void VisitRow(Glyph* row);
	virtual void VisitDummyRow(Glyph* dummyRow);
	virtual void VisitSingleByteLetter(Glyph* singleByteLetter);
	virtual void VisitDoubleByteLetter(Glyph* doubleByteLetter);
	~TextingOutVisitor();//�Ҹ���
};

#endif // !_TEXTINGOUTVISITOR_H