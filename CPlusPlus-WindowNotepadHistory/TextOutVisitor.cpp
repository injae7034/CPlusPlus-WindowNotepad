#include "TextOutVisitor.h"

//����Ʈ������
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

//�Ҹ���
TextOutVisitor::~TextOutVisitor()
{

}