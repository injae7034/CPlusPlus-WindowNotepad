#include "SelectingVisitor.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "TextExtent.h"
#include "Font.h"

//디폴트생성자
SelectingVisitor::SelectingVisitor(NotepadForm* notepadForm, CDC* dc,
	Long glyphXPos, Long glyphYPos)
	:GlyphVisitor(notepadForm, dc, glyphXPos, glyphYPos)
{
	
}

//복사생성자
SelectingVisitor::SelectingVisitor(const SelectingVisitor& source)
	:GlyphVisitor(source)
{

}

//치환연산자
SelectingVisitor& SelectingVisitor::operator=(const SelectingVisitor& source)
{
	GlyphVisitor::operator=(source);

	return *this;
}

//Note
void SelectingVisitor::VisitNote(Glyph* note)
{
	//1. 선택된 범위를 구한다.
	Long startingRowPos = 0;
	Long startingLetterPos = 0;
	Long endingRowPos = 0;
	Long endingLetterPos = 0;
	note->CalculateSelectedRange(&startingRowPos,
		&startingLetterPos, &endingRowPos, &endingLetterPos);
	//2. 메모장에서 선택된 texts를 출력한다.
	//2.1 선택이 시작되는 줄부터 시작해서 선택이 끝나는 줄까지 반복한다.
	Glyph* row = 0;
	Long rowIndex = startingRowPos;
	while (rowIndex <= endingRowPos)
	{
		//2.1.1 현재 줄을 구한다.
		row = note->GetAt(rowIndex);
		//2.1.2 출력될 부분의 위치를 업데이트해준다.
		this->glyphXPos = 0;
		this->glyphYPos = rowIndex;
		//2.1.3 줄에서 선택이 된 texts를 출력한다.
		row->Accept(this);
		//2.1.4 줄의 위치를 증가시킨다.
		rowIndex++;
	}
}

//Row
void SelectingVisitor::VisitRow(Glyph* row)
{
	//1. 줄에서 선택된 부분을 출력한다.
	//1.1 줄의 첫글자부터 시작해서 줄의 글자개수보다 작은동안 반복한다.
	Glyph* letter = 0;//글자 주소
	string content;//글자 내용을 담을 공간
	Long letterWidth = 0;//글자너비
	Long letterIndex = 0;//줄에서 글자 위치
	while (letterIndex < row->GetLength())
	{
		//1.1.1 현재 글자를 구한다.
		letter= row->GetAt(letterIndex);
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
void SelectingVisitor::VisitDummyRow(Glyph* dummyRow)
{
	//1. 줄에서 선택된 부분을 출력한다.
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
void SelectingVisitor::VisitSingleByteLetter(Glyph* singleByteLetter)
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
	//7. 현재 글자가 선택이 되어있으면
	if (singleByteLetter->IsSelected() == true)
	{
		//4. 배경색을 청색으로 설정한다.
		this->dc->SetBkColor(GetSysColor(COLOR_HIGHLIGHT));//red, green, blue 세개 색깔 
		//5. 글자색을 흰색으로 설정한다.
		this->dc->SetTextColor(RGB(255, 255, 255));
		//글자를 출력한다.
		this->dc->TextOut(this->glyphXPos - currentXPos, this->glyphYPos * text.tmHeight
			- currentYPos, content);
		
	}
	
	//font가 폰트공통대화상자에서 변경되었을때 기존 font를 지워야 새로 변경된 font로 적용할 수 있음.
	this->dc->SelectObject(oldFont);
	font.DeleteObject();
}

//DobuleByteLetter
void SelectingVisitor::VisitDoubleByteLetter(Glyph* doubleByteLetter)
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
	//7. 현재 글자가 선택이 되어있으면
	if (doubleByteLetter->IsSelected() == true)
	{
		//4. 배경색을 청색으로 설정한다.
		this->dc->SetBkColor(GetSysColor(COLOR_HIGHLIGHT));//red, green, blue 세개 색깔 
		//5. 글자색을 흰색으로 설정한다.
		this->dc->SetTextColor(RGB(255, 255, 255));
		//글자를 출력한다.
		this->dc->TextOut(this->glyphXPos - currentXPos, this->glyphYPos * text.tmHeight
			- currentYPos, content);

	}

	//font가 폰트공통대화상자에서 변경되었을때 기존 font를 지워야 새로 변경된 font로 적용할 수 있음.
	this->dc->SelectObject(oldFont);
	font.DeleteObject();
}


//소멸자
SelectingVisitor::~SelectingVisitor()
{

}