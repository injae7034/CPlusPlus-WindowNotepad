#include "TextingOutVisitor.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "TextExtent.h"
#include "Font.h"

//����Ʈ������
TextingOutVisitor::TextingOutVisitor(NotepadForm* notepadForm, CDC* dc,
	Long glyphXPos, Long glyphYPos)
	:GlyphVisitor(notepadForm, dc, glyphXPos, glyphYPos)
{

}

//���������
TextingOutVisitor::TextingOutVisitor(const TextingOutVisitor& source)
	:GlyphVisitor(source)
{

}

//ġȯ������
TextingOutVisitor& TextingOutVisitor::operator=(const TextingOutVisitor& source)
{
	GlyphVisitor::operator=(source);

	return *this;
}

//Note
void TextingOutVisitor::VisitNote(Glyph* note)
{
	//1. ������ũ���� ���� ��ġ�� ���Ѵ�.
	Long currentScrollYPos = this->notepadForm->GetScrollPos(SB_VERT);
	//2. ���� ������ ���̸� ���Ѵ�.
	Long letterHeight = this->notepadForm->textExtent->GetHeight();
	//3. ȭ�鿡�� ����� ���۵Ǵ� ���� ��ġ�� ���Ѵ�.
	Long startRowIndex = currentScrollYPos / letterHeight;
	//4. ���� ȭ���� ũ�⸦ ���Ѵ�.
	CRect rect;
	this->notepadForm->GetClientRect(&rect);
	//5. ȭ�鿡�� ����� ������ ���� ��ġ�� ���Ѵ�.
	Long endRowIndex = (currentScrollYPos + rect.Height()) / letterHeight;
	//6. ����ũ���� ���� ��ġ�� ���Ѵ�.
	Long currentScollXPos = this->notepadForm->GetScrollPos(SB_HORZ);
	//7. ȭ�鿡�� ����� ���۵Ǵ� ���� ��Ʈ�� ���� �������� �������� �׸���
	//ȭ�鿡�� ����� ������ ���� ��ġ���� �۰ų� �������� �ݺ��Ѵ�.
	Glyph* row = 0;
	Long i = startRowIndex;
	while (i < note->GetLength() && i <= endRowIndex)
	{
		//7.1 ���� ���Ѵ�.
		row = note->GetAt(i);
		//7.2 ��µ� �κ��� ��ġ�� ������Ʈ���ش�.
		this->glyphXPos = currentScollXPos;
		this->glyphYPos = i;
		//7.3  �ٿ��� ������ �ȵ� texts�� ����Ѵ�.
		row->Accept(this);
		//7.4 ���� ��ġ�� ������Ų��.
		i++;
	}
}

//Row
void TextingOutVisitor::VisitRow(Glyph* row)
{
	//1. �ٿ��� ������ �ȵ� texts�� ����Ѵ�.
	//1.1 ���� ù���ں��� �����ؼ� ���� ���ڰ������� �������� �ݺ��Ѵ�.
	Glyph* letter = 0;//���� �ּ�
	string content;//���� ������ ���� ����
	Long letterWidth = 0;//���ڳʺ�
	Long letterIndex = 0;//�ٿ��� ���� ��ġ
	while (letterIndex < row->GetLength())
	{
		//1.1.1 ���� ���ڸ� ���Ѵ�.
		letter = row->GetAt(letterIndex);
		//1.1.2 ���� ������ ������ ���Ѵ�.
		content = CString(letter->GetContent().c_str());
		//1.1.3 ���� ������ �ʺ� ������� �ؼ� �޸��忡�� ������ X��ǥ ��ġ�� ������Ʈ���ش�.
		this->glyphXPos += letterWidth;
		letterWidth = this->notepadForm->textExtent->GetTextWidth((string)content);
		//1.1.4 ���� ���ڸ� ����Ѵ�.
		letter->Accept(this);
		//1.1.5 ������ ��ġ�� ������Ų��.
		letterIndex++;
	}
}

//DummyRow
void TextingOutVisitor::VisitDummyRow(Glyph* dummyRow)
{
	//1. �ٿ��� ������ �ȵ� texts�� ����Ѵ�.
	//1.1 ���� ù���ں��� �����ؼ� ���� ���ڰ������� �������� �ݺ��Ѵ�.
	Glyph* letter = 0;//���� �ּ�
	string content;//���� ������ ���� ����
	Long letterWidth = 0;//���ڳʺ�
	Long letterIndex = 0;//�ٿ��� ���� ��ġ
	while (letterIndex < dummyRow->GetLength())
	{
		//1.1.1 ���� ���ڸ� ���Ѵ�.
		letter = dummyRow->GetAt(letterIndex);
		//1.1.2 ���� ������ ������ ���Ѵ�.
		content = CString(letter->GetContent().c_str());
		//1.1.3 ���� ������ �ʺ� ������� �ؼ� �޸��忡�� ������ X��ǥ ��ġ�� ������Ʈ���ش�.
		this->glyphXPos += letterWidth;
		letterWidth = this->notepadForm->textExtent->GetTextWidth((string)content);
		//1.1.4 ���� ���ڸ� ����Ѵ�.
		letter->Accept(this);
		//1.1.5 ������ ��ġ�� ������Ų��.
		letterIndex++;
	}
}

//SingleByteLetter
void TextingOutVisitor::VisitSingleByteLetter(Glyph* singleByteLetter)
{
	//1. �۾�ũ��� �۾�ü�� ���ϴ�.
	CFont font;
	font.CreateFontIndirect(&this->notepadForm->font.GetLogFont());
	//2. ��Ʈ�� dc�� �����Ѵ�.
	HFONT oldFont;
	oldFont = (HFONT)this->dc->SelectObject(font);
	//3. dc�� ������ �۲��� ������ ��´�.
	TEXTMETRIC text;
	this->dc->GetTextMetrics(&text);
	//4. ��ũ�� ������ ���Ѵ�.
	Long currentXPos = this->notepadForm->GetScrollPos(SB_HORZ);
	Long currentYPos = this->notepadForm->GetScrollPos(SB_VERT);
	//5. ������ ������ ���Ѵ�.
	CString content = CString(singleByteLetter->GetContent().c_str());
	//6. ���࿡ ���ڰ� �ǹ����̸� ������ ���� 8���� �ٲ��ش�.
	if (content == "\t")
	{
		content = "        ";
	}
	//7. ���� ���ڰ� ������ �ȵǾ�������
	if (singleByteLetter->IsSelected() == false)
	{
		//7.1 ������ ������� �����Ѵ�.
		this->dc->SetBkColor(RGB(255, 255, 255));
		//7.2 �۲������� �޾� ���ڻ��� ���Ѵ�.
		this->dc->SetTextColor(this->notepadForm->font.GetColor());
		//7.3 ���ڸ� ����Ѵ�.
		this->dc->TextOut(this->glyphXPos - currentXPos * 2, this->glyphYPos * text.tmHeight
			- currentYPos, content);
	}
	//8. ���� ���ڰ� ������ �Ǿ�������
	else
	{
		//8.1 ������ û������ �����Ѵ�.
		this->dc->SetBkColor(GetSysColor(COLOR_HIGHLIGHT));//red, green, blue ���� ���� 
		//8.2 ���ڻ��� ������� �����Ѵ�.
		this->dc->SetTextColor(RGB(255, 255, 255));
		//8.3 ���ڸ� ����Ѵ�.
		this->dc->TextOut(this->glyphXPos - currentXPos * 2, this->glyphYPos * text.tmHeight
			- currentYPos, content);
	}
	//9. font�� ��Ʈ�����ȭ���ڿ��� ����Ǿ����� ���� font�� ������ ���� ����� font�� ������ �� ����.
	this->dc->SelectObject(oldFont);
	font.DeleteObject();
}

//DobuleByteLetter
void TextingOutVisitor::VisitDoubleByteLetter(Glyph* doubleByteLetter)
{
	//1. �۾�ũ��� �۾�ü�� ���ϴ�.
	CFont font;
	font.CreateFontIndirect(&this->notepadForm->font.GetLogFont());
	//2. ��Ʈ�� dc�� �����Ѵ�.
	HFONT oldFont;
	oldFont = (HFONT)this->dc->SelectObject(font);
	//3. dc�� ������ �۲��� ������ ��´�.
	TEXTMETRIC text;
	this->dc->GetTextMetrics(&text);
	//4. ��ũ�� ������ ���Ѵ�.
	Long currentXPos = this->notepadForm->GetScrollPos(SB_HORZ);
	Long currentYPos = this->notepadForm->GetScrollPos(SB_VERT);
	//5. ������ ������ ���Ѵ�.
	CString content = CString(doubleByteLetter->GetContent().c_str());
	//6. ���� ���ڰ� ������ �ȵǾ�������
	if (doubleByteLetter->IsSelected() == false)
	{
		//6.1 ������ ������� �����Ѵ�.
		this->dc->SetBkColor(RGB(255, 255, 255));
		//6.2 �۲������� �޾� ���ڻ��� ���Ѵ�.
		this->dc->SetTextColor(this->notepadForm->font.GetColor());
		//6.3 ���ڸ� ����Ѵ�.
		this->dc->TextOut(this->glyphXPos - currentXPos * 2, this->glyphYPos * text.tmHeight
			- currentYPos, content);
	}
	//7. ���� ���ڰ� ������ �Ǿ�������
	else
	{
		//7.1 ������ û������ �����Ѵ�.
		this->dc->SetBkColor(GetSysColor(COLOR_HIGHLIGHT));//red, green, blue ���� ���� 
		//7.2 ���ڻ��� ������� �����Ѵ�.
		this->dc->SetTextColor(RGB(255, 255, 255));
		//7.3 ���ڸ� ����Ѵ�.
		this->dc->TextOut(this->glyphXPos - currentXPos * 2, this->glyphYPos * text.tmHeight
			- currentYPos, content);
	}
	//8. font�� ��Ʈ�����ȭ���ڿ��� ����Ǿ����� ���� font�� ������ ���� ����� font�� ������ �� ����.
	this->dc->SelectObject(oldFont);
	font.DeleteObject();
}

//�Ҹ���
TextingOutVisitor::~TextingOutVisitor()
{

}