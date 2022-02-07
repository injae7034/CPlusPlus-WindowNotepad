#ifndef _GLYPHVISITOR_H
#define _GLYPHVISITOR_H

class Glyph;
class CDC;
class NotepadForm;

typedef signed long int Long;

class GlyphVisitor
{
public:
	NotepadForm* notepadForm;
	CDC *dc;
	Long glyphXPos;
	Long glyphYPos;
public:
	GlyphVisitor(NotepadForm* notepadForm = 0, CDC* dc = 0,
		Long glyphXPos = 0, Long glyphYPos = 0);//����Ʈ������
	GlyphVisitor(const GlyphVisitor& source);//���������
	GlyphVisitor& operator=(const GlyphVisitor& source);//ġȯ������
	virtual void VisitNote(Glyph* note) = 0;
	virtual void VisitRow(Glyph* row) = 0;
	virtual void VisitDummyRow(Glyph* dummyRow) = 0;
	virtual void VisitSingleByteLetter(Glyph* singleByteLetter) = 0;
	virtual void VisitDoubleByteLetter(Glyph* doubleByteLetter) = 0;
	virtual ~GlyphVisitor() = 0;
};

#endif // !_GLYPHVISITOR_H
