#include "TextingOutVisitor.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "TextExtent.h"
#include "Font.h"

//디폴트생성자
TextingOutVisitor::TextingOutVisitor(NotepadForm* notepadForm, CPaintDC* dc,
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
	//1. 선택이 안된 범위를 구한다.
	Long startingRowPos = 0;
	Long startingLetterPos = 0;
	Long endingRowPos = 0;
	Long endingLetterPos = 0;
	note->CalculateSelectedRange(&startingRowPos,
		&startingLetterPos, &endingRowPos, &endingLetterPos);
	//2. 메모장에서 선택이 안된 부분을 출력한다.
	//2.1 선택된 부분전까지 줄단위로 출력한다.
	Glyph* row = 0;
	Long rowIndex = 0;
	while (rowIndex < startingRowPos)
	{
		//2.1 현재 줄을 구한다.
		row = note->GetAt(rowIndex);
		//2.2 출력될 부분의 위치를 업데이트해준다.
		this->glyphXPos = 0;
		this->glyphYPos = rowIndex;
		//2.3 줄단위로 선택없이 출력한다.
		row->Accept(this);
		//2.4 줄의 위치를 증가시킨다.
		rowIndex++;
	}
	//2.2 선택이 시작되는 줄에서 선택이 안된 글자들을 글자단위로 출력한다.
	rowIndex = startingRowPos;
	Glyph* letter = 0;//글자주소를 담을 공간
	string content;//글자의 내용을 담을 공간
	Long letterWidth = 0;//글자너비
	//2.2.1 현재 줄을 구한다.
	row = note->GetAt(rowIndex);
	Long letterIndex = 0;
	//2.2.2 선택이 시작되는 줄의 첫글자가 선택이 시작되는 글자의 위치보다 작은동안 반복한다.
	while (letterIndex < startingLetterPos)
	{
		//2.2.2.1 현재 글자를 구한다.
		letter = row->GetAt(letterIndex);
		//2.2.2.2 현재 글자의 내용을 구한다.
		content = CString(letter->GetContent().c_str());
		//2.2.2.3 현재 글자의 너비를 기반으로 해서 메모장에서 글자의 X좌표를 업데이트해준다.
		this->glyphXPos += letterWidth;
		letterWidth = this->notepadForm->textExtent->GetTextWidth((string)content);
		//2.2.2.4 현재 줄의 위치를 기반으로 해서 메모장에서 글자의 Y좌표를 업데이트해준다.
		this->glyphYPos = rowIndex;
		//2.2.2.5 글자단위로 선택없이 출력한다.
		letter->Accept(this);
		//2.2.2.6 글자의 위치를 증가시킨다.
		letterIndex++;
		
	}
	//2.3 선택이 끝나는 줄의 처음 글자부터 선택이 끝나는 줄의 마지막 선택 글자위치까지 반복한다.
	rowIndex = endingRowPos;
	letterIndex = 0;//글자위치 원위치
	letterWidth = 0;//글자너비 원위치
	//2.3.1 현재 줄을 구한다.
	row = note->GetAt(rowIndex);
	//2.3.2 선택이 끝나는 줄의 처음글자위치부터 시작해서 선택이 끝나는 줄의 마지막 글자위치보다
	//작거나 같은 동안 반복한다.
	while (letterIndex <= endingLetterPos)
	{
		//2.3.2.1 현재 글자를 구한다.
		letter = row->GetAt(letterIndex);
		//2.3.2.2 현재 글자의 내용을 구한다.
		content = CString(letter->GetContent().c_str());
		//2.3.2.3 현재 글자의 너비를 기반으로 해서 메모장에서 글자의 X좌표를 업데이트해준다.
		this->glyphXPos += letterWidth;
		letterWidth = this->notepadForm->textExtent->GetTextWidth((string)content);
		//2.3.2.4 현재 줄의 위치를 기반으로 해서 메모장에서 글자의 Y좌표를 업데이트해준다.
		this->glyphYPos = rowIndex;
		//2.3.2.5 글자의 위치를 증가시킨다.
		letterIndex++;
	}
	//2.4 선택이 끝나는 줄의 선택이 끝나는 글자다음부터 
	//줄의 마지막글자까지 선택이 안된 글자들을 글자단위로 출력한다.
	letterIndex = endingLetterPos + 1;
	//2.4.1 선택이 끝나는 줄의 마지막 다음 글자가 선택이 끝나는 줄의 개수보다 작은동안 반복한다.
	while (letterIndex < row->GetLength())
	{
		//2.4.1.1 현재 글자를 구한다.
		letter = row->GetAt(letterIndex);
		//2.4.1.2 현재 글자의 내용을 구한다.
		content = CString(letter->GetContent().c_str());
		//2.4.1.3 현재 글자의 너비를 기반으로 해서 메모장에서 글자의 X좌표를 업데이트해준다.
		this->glyphXPos += letterWidth;
		letterWidth = this->notepadForm->textExtent->GetTextWidth((string)content);
		//2.4.1.4 현재 줄의 위치를 기반으로 해서 메모장에서 글자의 Y좌표를 업데이트해준다.
		this->glyphYPos = rowIndex;
		//2.4.1.5 글자단위로 선택없이 출력한다.
		letter->Accept(this);
		//2.4.1.6 글자의 위치를 증가시킨다.
		letterIndex++;
	}
	//2.5 마지막줄까지 줄단위로 출력한다.
	while (rowIndex < note->GetLength())
	{
		//2.2.1 현재 줄을 구한다.
		row = note->GetAt(rowIndex);
		//2.2.2 출력될 부분의 위치를 업데이트해준다.
		this->glyphXPos = 0;
		this->glyphYPos = rowIndex;
		//2.2.3 줄단위로 선택없이 출력한다.
		row->Accept(this);
		//2.2.4 줄의 위치를 증가시킨다.
		rowIndex++;
	}
}

//Row
void TextingOutVisitor::VisitRow(Glyph* row)
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
	//4. 배경색을 흰색으로 설정한다.
	this->dc->SetBkColor(RGB(255, 255, 255));
	//5. 글꼴의 정보를 얻어 글자색을 설정한다.
	this->dc->SetTextColor(this->notepadForm->font.GetColor());
	//6. 스크롤 정보를 구한다.
	Long currentXPos = this->notepadForm->GetScrollPos(SB_HORZ);
	Long currentYPos = this->notepadForm->GetScrollPos(SB_VERT);
	//7. 줄의 내용을 구한다.
	CString content = CString(row->GetContent().c_str());
	//8. 줄을 메모장에 출력한다.
	this->dc->TextOut(this->glyphXPos - currentXPos, 
		this->glyphYPos * text.tmHeight - currentYPos, content);
	
	//font가 폰트공통대화상자에서 변경되었을때 기존 font를 지워야 새로 변경된 font로 적용할 수 있음.
	this->dc->SelectObject(oldFont);
	font.DeleteObject();
}

//DummyRow
void TextingOutVisitor::VisitDummyRow(Glyph* dummyRow)
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
	//4. 배경색을 흰색으로 설정한다.
	this->dc->SetBkColor(RGB(255, 255, 255));
	//5. 글꼴의 정보를 얻어 글자색을 설정한다.
	this->dc->SetTextColor(this->notepadForm->font.GetColor());
	//6. 스크롤 정보를 구한다.
	Long currentXPos = this->notepadForm->GetScrollPos(SB_HORZ);
	Long currentYPos = this->notepadForm->GetScrollPos(SB_VERT);
	//7. 줄의 내용을 구한다.
	CString content = CString(dummyRow->GetContent().c_str());
	//8. 줄을 메모장에 출력한다.
	this->dc->TextOut(this->glyphXPos - currentXPos,
		this->glyphYPos * text.tmHeight - currentYPos, content);

	//font가 폰트공통대화상자에서 변경되었을때 기존 font를 지워야 새로 변경된 font로 적용할 수 있음.
	this->dc->SelectObject(oldFont);
	font.DeleteObject();
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
	//7. 배경색을 흰색으로 설정한다.
	this->dc->SetBkColor(RGB(255, 255, 255));
	//8. 글꼴정보를 받아 글자색을 정한다.
	this->dc->SetTextColor(this->notepadForm->font.GetColor());
	//9. 글자단위로 출력한다.
	this->dc->TextOut(this->glyphXPos - currentXPos, this->glyphYPos * text.tmHeight
		- currentYPos, content);

	//font가 폰트공통대화상자에서 변경되었을때 기존 font를 지워야 새로 변경된 font로 적용할 수 있음.
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
	//6. 만약에 글자가 탭문자이면 내용을 띄어쓰기 8개로 바꿔준다.
	if (content == "\t")
	{
		content = "        ";
	}
	//7. 배경색을 흰색으로 설정한다.
	this->dc->SetBkColor(RGB(255, 255, 255));
	//8. 글꼴정보를 받아 글자색을 정한다.
	this->dc->SetTextColor(this->notepadForm->font.GetColor());
	//9. 글자단위로 출력한다.
	this->dc->TextOut(this->glyphXPos - currentXPos, this->glyphYPos * text.tmHeight
		- currentYPos, content);

	//font가 폰트공통대화상자에서 변경되었을때 기존 font를 지워야 새로 변경된 font로 적용할 수 있음.
	this->dc->SelectObject(oldFont);
	font.DeleteObject();
}

//소멸자
TextingOutVisitor::~TextingOutVisitor()
{

}