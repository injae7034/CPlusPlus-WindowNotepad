#include "TextingOutVisitor.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "TextExtent.h"
#include "Font.h"

//디폴트생성자
TextingOutVisitor::TextingOutVisitor(NotepadForm* notepadForm, CDC* dc,
	Long glyphXPos, Long glyphYPos)
	:GlyphVisitor(notepadForm, dc, glyphXPos, glyphYPos)
{

}

//복사생성자
TextingOutVisitor::TextingOutVisitor(const TextingOutVisitor& source)
	:GlyphVisitor(source)
{

}

//치환연산자
TextingOutVisitor& TextingOutVisitor::operator=(const TextingOutVisitor& source)
{
	GlyphVisitor::operator=(source);

	return *this;
}

//Note
void TextingOutVisitor::VisitNote(Glyph* note)
{
	//1. 수직스크롤의 현재 위치를 구한다.
	Long currentScrollYPos = this->notepadForm->GetScrollPos(SB_VERT);
	//2. 현재 글자의 높이를 구한다.
	Long letterHeight = this->notepadForm->textExtent->GetHeight();
	//3. 화면에서 출력이 시작되는 줄의 위치를 구한다.
	Long startRowIndex = currentScrollYPos / letterHeight;
	//4. 현재 화면의 크기를 구한다.
	CRect rect;
	this->notepadForm->GetClientRect(&rect);
	//5. 화면에서 출력이 끝나는 줄의 위치를 구한다.
	Long endRowIndex = (currentScrollYPos + rect.Height()) / letterHeight;
	//6. 수평스크롤의 현재 위치를 구한다.
	Long currentScollXPos = this->notepadForm->GetScrollPos(SB_HORZ);
	//7. 화면에서 출력이 시작되는 줄이 노트의 줄의 개수보다 작은동안 그리고
	//화면에서 출력이 끝나는 줄의 위치보다 작거나 같은동안 반복한다.
	Glyph* row = 0;
	Long i = startRowIndex;
	while (i < note->GetLength() && i <= endRowIndex)
	{
		//7.1 줄을 구한다.
		row = note->GetAt(i);
		//7.2 출력될 부분의 위치를 업데이트해준다.
		this->glyphXPos = currentScollXPos;
		this->glyphYPos = i;
		//7.3  줄에서 선택이 안된 texts를 출력한다.
		row->Accept(this);
		//7.4 줄의 위치를 증가시킨다.
		i++;
	}
}

//Row
void TextingOutVisitor::VisitRow(Glyph* row)
{
	//1. 줄에서 선택이 안된 texts를 출력한다.
	//1.1 줄의 첫글자부터 시작해서 줄의 글자개수보다 작은동안 반복한다.
	Glyph* letter = 0;//글자 주소
	string content;//글자 내용을 담을 공간
	Long letterWidth = 0;//글자너비
	Long letterIndex = 0;//줄에서 글자 위치
	while (letterIndex < row->GetLength())
	{
		//1.1.1 현재 글자를 구한다.
		letter = row->GetAt(letterIndex);
		//1.1.2 현재 글자의 내용을 구한다.
		content = CString(letter->GetContent().c_str());
		//1.1.3 현재 글자의 너비를 기반으로 해서 메모장에서 글자의 X좌표 위치를 업데이트해준다.
		this->glyphXPos += letterWidth;
		letterWidth = this->notepadForm->textExtent->GetTextWidth((string)content);
		//1.1.4 현재 글자를 출력한다.
		letter->Accept(this);
		//1.1.5 글자의 위치를 증가시킨다.
		letterIndex++;
	}
}

//DummyRow
void TextingOutVisitor::VisitDummyRow(Glyph* dummyRow)
{
	//1. 줄에서 선택이 안된 texts를 출력한다.
	//1.1 줄의 첫글자부터 시작해서 줄의 글자개수보다 작은동안 반복한다.
	Glyph* letter = 0;//글자 주소
	string content;//글자 내용을 담을 공간
	Long letterWidth = 0;//글자너비
	Long letterIndex = 0;//줄에서 글자 위치
	while (letterIndex < dummyRow->GetLength())
	{
		//1.1.1 현재 글자를 구한다.
		letter = dummyRow->GetAt(letterIndex);
		//1.1.2 현재 글자의 내용을 구한다.
		content = CString(letter->GetContent().c_str());
		//1.1.3 현재 글자의 너비를 기반으로 해서 메모장에서 글자의 X좌표 위치를 업데이트해준다.
		this->glyphXPos += letterWidth;
		letterWidth = this->notepadForm->textExtent->GetTextWidth((string)content);
		//1.1.4 현재 글자를 출력한다.
		letter->Accept(this);
		//1.1.5 글자의 위치를 증가시킨다.
		letterIndex++;
	}
}

//SingleByteLetter
void TextingOutVisitor::VisitSingleByteLetter(Glyph* singleByteLetter)
{
	//1. 글씨크기와 글씨체를 정하다.
	CFont font;
	font.CreateFontIndirect(&this->notepadForm->font.GetLogFont());
	//2. 폰트를 dc에 지정한다.
	HFONT oldFont;
	oldFont = (HFONT)this->dc->SelectObject(font);
	//3. dc에 지정된 글꼴의 정보를 얻는다.
	TEXTMETRIC text;
	this->dc->GetTextMetrics(&text);
	//4. 스크롤 정보를 구한다.
	Long currentXPos = this->notepadForm->GetScrollPos(SB_HORZ);
	Long currentYPos = this->notepadForm->GetScrollPos(SB_VERT);
	//5. 글자의 내용을 구한다.
	CString content = CString(singleByteLetter->GetContent().c_str());
	//6. 만약에 글자가 탭문자이면 내용을 띄어쓰기 8개로 바꿔준다.
	if (content == "\t")
	{
		content = "        ";
	}
	//7. 현재 글자가 선택이 안되어있으면
	if (singleByteLetter->IsSelected() == false)
	{
		//7.1 배경색을 흰색으로 설정한다.
		this->dc->SetBkColor(RGB(255, 255, 255));
		//7.2 글꼴정보를 받아 글자색을 정한다.
		this->dc->SetTextColor(this->notepadForm->font.GetColor());
		//7.3 글자를 출력한다.
		this->dc->TextOut(this->glyphXPos - currentXPos * 2, this->glyphYPos * text.tmHeight
			- currentYPos, content);
	}
	//8. 현재 글자가 선택이 되어있으면
	else
	{
		//8.1 배경색을 청색으로 설정한다.
		this->dc->SetBkColor(GetSysColor(COLOR_HIGHLIGHT));//red, green, blue 세개 색깔 
		//8.2 글자색을 흰색으로 설정한다.
		this->dc->SetTextColor(RGB(255, 255, 255));
		//8.3 글자를 출력한다.
		this->dc->TextOut(this->glyphXPos - currentXPos * 2, this->glyphYPos * text.tmHeight
			- currentYPos, content);
	}
	//9. font가 폰트공통대화상자에서 변경되었을때 기존 font를 지워야 새로 변경된 font로 적용할 수 있음.
	this->dc->SelectObject(oldFont);
	font.DeleteObject();
}

//DobuleByteLetter
void TextingOutVisitor::VisitDoubleByteLetter(Glyph* doubleByteLetter)
{
	//1. 글씨크기와 글씨체를 정하다.
	CFont font;
	font.CreateFontIndirect(&this->notepadForm->font.GetLogFont());
	//2. 폰트를 dc에 지정한다.
	HFONT oldFont;
	oldFont = (HFONT)this->dc->SelectObject(font);
	//3. dc에 지정된 글꼴의 정보를 얻는다.
	TEXTMETRIC text;
	this->dc->GetTextMetrics(&text);
	//4. 스크롤 정보를 구한다.
	Long currentXPos = this->notepadForm->GetScrollPos(SB_HORZ);
	Long currentYPos = this->notepadForm->GetScrollPos(SB_VERT);
	//5. 글자의 내용을 구한다.
	CString content = CString(doubleByteLetter->GetContent().c_str());
	//6. 현재 글자가 선택이 안되어있으면
	if (doubleByteLetter->IsSelected() == false)
	{
		//6.1 배경색을 흰색으로 설정한다.
		this->dc->SetBkColor(RGB(255, 255, 255));
		//6.2 글꼴정보를 받아 글자색을 정한다.
		this->dc->SetTextColor(this->notepadForm->font.GetColor());
		//6.3 글자를 출력한다.
		this->dc->TextOut(this->glyphXPos - currentXPos * 2, this->glyphYPos * text.tmHeight
			- currentYPos, content);
	}
	//7. 현재 글자가 선택이 되어있으면
	else
	{
		//7.1 배경색을 청색으로 설정한다.
		this->dc->SetBkColor(GetSysColor(COLOR_HIGHLIGHT));//red, green, blue 세개 색깔 
		//7.2 글자색을 흰색으로 설정한다.
		this->dc->SetTextColor(RGB(255, 255, 255));
		//7.3 글자를 출력한다.
		this->dc->TextOut(this->glyphXPos - currentXPos * 2, this->glyphYPos * text.tmHeight
			- currentYPos, content);
	}
	//8. font가 폰트공통대화상자에서 변경되었을때 기존 font를 지워야 새로 변경된 font로 적용할 수 있음.
	this->dc->SelectObject(oldFont);
	font.DeleteObject();
}

//소멸자
TextingOutVisitor::~TextingOutVisitor()
{

}