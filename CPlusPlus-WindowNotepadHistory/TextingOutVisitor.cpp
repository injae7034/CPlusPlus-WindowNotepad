#include "TextingOutVisitor.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "TextExtent.h"
#include "Font.h"
#include "PrintInformation.h"

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
	Glyph* row = 0;
	Long rowIndex = 0;
	//1. dc가 프린트 관련 dc가 아니면
	if (this->dc->IsPrinting() == 0)
	{
		//1. 선택된 범위를 구한다.
		Long startingRowPos = 0;
		Long startingLetterPos = 0;
		Long endingRowPos = 0;
		Long endingLetterPos = 0;
		note->CalculateSelectedRange(&startingRowPos,
			&startingLetterPos, &endingRowPos, &endingLetterPos);
		//2. 메모장에서 선택이 안된 texts를 출력한다.
		//2.1 메모장의 첫 줄부터 선택된 시작되는 줄까지 반복한다.
		while (rowIndex <= startingRowPos)
		{
			//2.1.1 현재 줄을 구한다.
			row = note->GetAt(rowIndex);
			//2.1.2 출력될 부분의 위치를 업데이트해준다.
			this->glyphXPos = 0;
			this->glyphYPos = rowIndex;
			//2.1.3 줄에서 선택이 안된 texts를 출력한다.
			row->Accept(this);
			//2.1.4 줄의 위치를 증가시킨다.
			rowIndex++;
		}
		//2.2 줄의 위치를 선택이 끝나는 줄로 이동시킨다.
		rowIndex = endingRowPos;
		//2.3 선택이 끝나는 줄부터 메모장의 마지막줄까지 반복한다.
		while (rowIndex < note->GetLength())
		{
			//2.3.1 현재 줄을 구한다.
			row = note->GetAt(rowIndex);
			//2.3.2 출력될 부분의 위치를 업데이트해준다.
			this->glyphXPos = 0;
			this->glyphYPos = rowIndex;
			//2.3.3  줄에서 선택이 안된 texts를 출력한다.
			row->Accept(this);
			//2.3.4 줄의 위치를 증가시킨다.
			rowIndex++;
		}
	}
	//2. dc가 프린트 관련 dc가 아니면
	else
	{
		Glyph* printNote = this->notepadForm->printInformation->GetPrintNote();
		CDC* printDC = this->notepadForm->printInformation->GetPrintDC();
		Long pageRowCount = this->notepadForm->printInformation->GetPageRowCount();
		Long i = 0;
		while (rowIndex < printNote->GetLength())
		{
			i = 0;
			printDC->StartPage();
			while (i < pageRowCount && rowIndex < printNote->GetLength())
			{
				//2.1.1 현재 줄을 구한다.
				row = note->GetAt(rowIndex);
				//2.1.2 출력될 부분의 위치를 업데이트해준다.
				this->glyphXPos = 0;
				this->glyphYPos = i;
				//2.1.3 줄에서 선택이 안된 texts를 출력한다.
				row->Accept(this);
				//2.1.4 줄의 위치를 증가시킨다.
				rowIndex++;
				i++;
			}
			printDC->EndPage();
		}
	}
}

//Row
void TextingOutVisitor::VisitRow(Glyph* row)
{	
	Glyph* letter = 0;//글자 주소
	string content;//글자 내용을 담을 공간
	Long letterWidth = 0;//글자너비
	Long letterIndex = 0;//줄에서 글자 위치
	//1. dc가 프린트 관련 dc가 아니면
	if (this->dc->IsPrinting() == 0)
	{
		//1.1 줄의 첫글자부터 시작해서 줄의 글자개수보다 작은동안 반복한다.
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
	//2. dc가 프린트 관련 dc이면
	else
	{
		//1. 글씨크기와 글씨체를 정하다.
		CFont font;
		//2. 폰트를 dc에 지정한다.
		HFONT oldFont;
		font.CreateFontIndirect(&this->notepadForm->printInformation->GetPrintLogFont());
		oldFont = (HFONT)this->dc->SelectObject(font);
		//3. dc에 지정된 글꼴의 정보를 얻는다.
		TEXTMETRIC text;
		this->dc->GetTextMetrics(&text);
		while (letterIndex < row->GetLength())
		{
			//1.1.1 현재 글자를 구한다.
			letter = row->GetAt(letterIndex);
			//1.1.2 현재 글자의 내용을 구한다.
			content = CString(letter->GetContent().c_str());
			//1.1.3 현재 글자의 너비를 기반으로 해서 메모장에서 글자의 X좌표 위치를 업데이트해준다.
			this->glyphXPos += letterWidth;
			letterWidth = this->dc->GetTextExtent(content.c_str()).cx;
			//1.1.4 현재 글자를 출력한다.
			letter->Accept(this);
			//1.1.5 글자의 위치를 증가시킨다.
			letterIndex++;
		}
	}
}

//DummyRow
void TextingOutVisitor::VisitDummyRow(Glyph* dummyRow)
{
	Glyph* letter = 0;//글자 주소
	string content;//글자 내용을 담을 공간
	Long letterWidth = 0;//글자너비
	Long letterIndex = 0;//줄에서 글자 위치
	//1. dc가 프린트 관련 dc가 아니면
	if (this->dc->IsPrinting() == 0)
	{
		//1.1 줄의 첫글자부터 시작해서 줄의 글자개수보다 작은동안 반복한다.
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
	//2. dc가 프린트 관련 dc이면
	else
	{
		//1. 글씨크기와 글씨체를 정하다.
		CFont font;
		//2. 폰트를 dc에 지정한다.
		HFONT oldFont;
		font.CreateFontIndirect(&this->notepadForm->printInformation->GetPrintLogFont());
		oldFont = (HFONT)this->dc->SelectObject(font);
		//3. dc에 지정된 글꼴의 정보를 얻는다.
		TEXTMETRIC text;
		this->dc->GetTextMetrics(&text);
		while (letterIndex < dummyRow->GetLength())
		{
			//1.1.1 현재 글자를 구한다.
			letter = dummyRow->GetAt(letterIndex);
			//1.1.2 현재 글자의 내용을 구한다.
			content = CString(letter->GetContent().c_str());
			//1.1.3 현재 글자의 너비를 기반으로 해서 메모장에서 글자의 X좌표 위치를 업데이트해준다.
			this->glyphXPos += letterWidth;
			letterWidth = this->dc->GetTextExtent(content.c_str()).cx;
			//1.1.4 현재 글자를 출력한다.
			letter->Accept(this);
			//1.1.5 글자의 위치를 증가시킨다.
			letterIndex++;
		}
	}
}

//SingleByteLetter
void TextingOutVisitor::VisitSingleByteLetter(Glyph* singleByteLetter)
{
	CFont font;
	HFONT oldFont;
	Long currentXPos = 0;
	Long currentYPos = 0;
	//1. dc가 프린트관련 dc가 아니면
	if (this->dc->IsPrinting() == 0)
	{
		//1.1 메모장의 font정보를 통해 font를 생성한다.
		font.CreateFontIndirect(&this->notepadForm->font.GetLogFont());
		oldFont = (HFONT)this->dc->SelectObject(font);
		//1.2 스크롤의 정보를 구한다.
		currentXPos = this->notepadForm->GetScrollPos(SB_HORZ);
		currentYPos = this->notepadForm->GetScrollPos(SB_VERT);
	}
	//2. dc가 프린트관련 dc이면
	else
	{
		//2.1 프린트 비율로 비율을 맞춰준다.
		font.CreateFontIndirect(&this->notepadForm->printInformation->GetPrintLogFont());
		oldFont = (HFONT)this->dc->SelectObject(font);
	}
	//3. 글꼴 정보를 구한다.
	TEXTMETRIC text;
	this->dc->GetTextMetrics(&text);
	//4. 글자의 내용을 구한다.
	CString content = CString(singleByteLetter->GetContent().c_str());
	//5. 만약에 글자가 탭문자이면 내용을 띄어쓰기 8개로 바꿔준다.
	if (content == "\t")
	{
		content = "        ";
	}
	//6. 현재 글자가 선택이 안되어있으면
	if (singleByteLetter->IsSelected() == false)
	{
		//6.1 배경색을 흰색으로 설정한다.
		this->dc->SetBkColor(RGB(255, 255, 255));
		//6.2 글꼴정보를 받아 글자색을 정한다.
		this->dc->SetTextColor(this->notepadForm->font.GetColor());
		//6.3 글자를 출력한다.
		this->dc->TextOut(this->glyphXPos - currentXPos, this->glyphYPos * text.tmHeight
			- currentYPos, content);

	}
	//font가 폰트공통대화상자에서 변경되었을때 기존 font를 지워야 새로 변경된 font로 적용할 수 있음.
	this->dc->SelectObject(oldFont);
	font.DeleteObject();
}

//DobuleByteLetter
void TextingOutVisitor::VisitDoubleByteLetter(Glyph* doubleByteLetter)
{
	CFont font;
	HFONT oldFont;
	Long currentXPos = 0;
	Long currentYPos = 0;
	//1. dc가 프린트관련 dc가 아니면
	if (this->dc->IsPrinting() == 0)
	{
		//1.1 메모장의 font정보를 통해 font를 생성한다.
		font.CreateFontIndirect(&this->notepadForm->font.GetLogFont());
		oldFont = (HFONT)this->dc->SelectObject(font);
		//1.2 스크롤의 정보를 구한다.
		currentXPos = this->notepadForm->GetScrollPos(SB_HORZ);
		currentYPos = this->notepadForm->GetScrollPos(SB_VERT);
	}
	//2. dc가 프린트관련 dc이면
	else
	{
		//2.1 프린트 비율로 비율을 맞춰준다.
		font.CreateFontIndirect(&this->notepadForm->printInformation->GetPrintLogFont());
		oldFont = (HFONT)this->dc->SelectObject(font);
	}
	//3. 글꼴 정보를 구한다.
	TEXTMETRIC text;
	this->dc->GetTextMetrics(&text);
	//4. 글자의 내용을 구한다.
	CString content = CString(doubleByteLetter->GetContent().c_str());
	//5. 만약에 글자가 탭문자이면 내용을 띄어쓰기 8개로 바꿔준다.
	if (content == "\t")
	{
		content = "        ";
	}
	//6. 현재 글자가 선택이 안되어있으면
	if (doubleByteLetter->IsSelected() == false)
	{
		//6.1 배경색을 흰색으로 설정한다.
		this->dc->SetBkColor(RGB(255, 255, 255));
		//6.2 글꼴정보를 받아 글자색을 정한다.
		this->dc->SetTextColor(this->notepadForm->font.GetColor());
		//6.3 글자를 출력한다.
		this->dc->TextOut(this->glyphXPos - currentXPos, this->glyphYPos * text.tmHeight
			- currentYPos, content);
	}
	//font가 폰트공통대화상자에서 변경되었을때 기존 font를 지워야 새로 변경된 font로 적용할 수 있음.
	this->dc->SelectObject(oldFont);
	font.DeleteObject();
}

//소멸자
TextingOutVisitor::~TextingOutVisitor()
{

}