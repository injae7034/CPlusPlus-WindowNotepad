#include "GlyphVisitor.h"
#include "Glyph.h"

//디폴트생성자
GlyphVisitor::GlyphVisitor(NotepadForm* notepadForm, CPaintDC* dc,
	Long glyphXPos, Long glyphYPos)
{
	this->notepadForm = notepadForm;
	this->dc = dc;
	this->glyphXPos = glyphXPos;
	this->glyphYPos = glyphYPos;
}

//복사생성자
GlyphVisitor::GlyphVisitor(const GlyphVisitor& source)
{
	this->notepadForm = source.notepadForm;
	this->dc = source.dc;
	this->glyphXPos = source.glyphXPos;
	this->glyphYPos = source.glyphYPos;
}

//치환연산자
GlyphVisitor& GlyphVisitor::operator=(const GlyphVisitor& source)
{
	this->notepadForm = source.notepadForm;
	this->dc = source.dc;
	this->glyphXPos = source.glyphXPos;
	this->glyphYPos = source.glyphYPos;

	return *this;
}

//Note
void GlyphVisitor::VisitNote(Glyph* note)
{
	
}

//Row
void GlyphVisitor::VisitRow(Glyph* row)
{

}

//DummyRow
void GlyphVisitor::VisitDummyRow(Glyph* dummyRow)
{

}

//SingleByteLetter
void GlyphVisitor::VisitSingleByteLetter(Glyph* singleByteLetter)
{

}

//DoubleByteLetter
void GlyphVisitor::VisitDoubleByteLetter(Glyph* doubleByteLetter)
{

}

//소멸자
GlyphVisitor::~GlyphVisitor()
{

}