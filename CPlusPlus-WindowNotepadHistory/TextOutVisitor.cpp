#include "TextOutVisitor.h"

//디폴트생성자
TextOutVisitor::TextOutVisitor(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
}

void TextOutVisitor::VisitNoteTextOut(Glyph *note)
{

}

void TextOutVisitor::VisitRowTextOut(Glyph* row)
{

}

void TextOutVisitor::VisitLetterTextOut(Glyph* letter)
{

}

//소멸자
TextOutVisitor::~TextOutVisitor()
{

}