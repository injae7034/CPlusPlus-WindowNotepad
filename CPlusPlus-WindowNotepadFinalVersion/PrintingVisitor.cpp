#include "PrintingVisitor.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "TextExtent.h"
#include "Font.h"
#include "PrintInformation.h"
#include "PageSetUpInformation.h"

//디폴트생성자
PrintingVisitor::PrintingVisitor(NotepadForm* notepadForm, CDC* dc,
	Long glyphXPos, Long glyphYPos)
	:GlyphVisitor(notepadForm, dc, glyphXPos, glyphYPos)
{

}

//복사생성자
PrintingVisitor::PrintingVisitor(const PrintingVisitor& source)
	:GlyphVisitor(source)
{

}

//치환연산자
PrintingVisitor& PrintingVisitor::operator=(const PrintingVisitor& source)
{
	GlyphVisitor::operator=(source);

	return *this;
}

//Note
void PrintingVisitor::VisitNote(Glyph* note)
{
	//1. 프린트할 노트를 구한다.
	Glyph* printNote = this->notepadForm->printInformation->GetPrintNote();
	//2. 프린트 다이얼로그의 hdc에서 printDC를 구한다.
	CDC* printDC = CDC::FromHandle(this->notepadForm->printInformation->GetHDC());
	//3. 한 페이지당 인쇄할 줄의 수를 구한다.
	Long pageRowCount = this->notepadForm->printInformation->GetPageRowCount();
	//4. 프린트 가능한 영역을 구한다. 
	CRect rect(0, 0, printDC->GetDeviceCaps(HORZRES), printDC->GetDeviceCaps(VERTRES));
	//5. 페이지 설정 정보가 있으면
	Glyph* row = 0;
	Long rowIndex = 0;
	Long i = 0;
	Long glyphXPos = 0;
	if (this->notepadForm->pageSetUpInformation != NULL)
	{
		//5.1 여백을 제외하고 프린트 가능한 영역의 왼쪽 위치를 저장한다.
		glyphXPos = this->notepadForm->pageSetUpInformation->GetPrintableRect().left;
		//5.2 프린트에 맞는 글꼴의 정보를 구해서 적용한다.
		CFont font;
		HFONT oldFont;
		font.CreateFontIndirect(&this->notepadForm->printInformation->GetPrintLogFont());
		oldFont = (HFONT)printDC->SelectObject(font);
		//5.3 프린트에 지정된 글꼴 정보를 구한다.
		TEXTMETRIC text;
		printDC->GetTextMetrics(&text);
		Long letterWidth = 0;
		Long rowCountOfPrintNote = printNote->GetLength();
		//5.4 프린트할 노트의 줄의 개수보다 작은동안 반복한다.
		row = 0;
		rowIndex = 0;
		i = 0;
		while (rowIndex < printNote->GetLength())
		{
			//5.4.1 i를 초기화한다.
			i = 0;
			//5.4.2 인쇄할 한 페이지를 연다.
			printDC->StartPage();
			//5.4.3 머리글이 있으면
			if (this->notepadForm->pageSetUpInformation->GetHeader().Compare("") != 0)
			{
				//5.4.3.1 머리글의 길이를 구한다.
				letterWidth = printDC->GetTextExtent
				(this->notepadForm->pageSetUpInformation->GetHeader()).cx;
				//5.4.3.2 머리글을 출력한다.
				//뒤에서 strechBlt로 축소하기 때문에 축소되지 않은 프린트가 가능한 영역의 절반에서
				//머리글의 절반 값을 뺀 곳에서 시작한다.
				printDC->TextOut(rect.Width() / 2 - letterWidth / 2,
					this->notepadForm->pageSetUpInformation->GetPrintableRect().top,
					this->notepadForm->pageSetUpInformation->GetHeader());
			}
			//5.4.4 바닥글이 있으면
			if (this->notepadForm->pageSetUpInformation->GetFooter().Compare("") != 0)
			{
				//5.4.4.1 바닥글의 길이를 구한다.
				letterWidth = printDC->GetTextExtent
				(this->notepadForm->pageSetUpInformation->GetFooter()).cx;
				//5.4.4.2 바닥글을 출력한다.
				//뒤에서 strechBlt로 축소하기 때문에 축소되지 않은 프린트가 가능한 영역의 절반에서
				//머리글의 절반 값을 뺀 곳에서 시작한다
				printDC->TextOut(rect.Width() / 2 - letterWidth / 2,
					this->notepadForm->pageSetUpInformation->GetPrintableRect().bottom
					- text.tmHeight,
					this->notepadForm->pageSetUpInformation->GetFooter());
			}
			//5.4.5 한 페이지당 인쇄할 줄의 개수보다 작은동안 그리고 프리트할 노트의 줄의 개수보다 작은동안 반복한다.
			while (i < pageRowCount && rowIndex < printNote->GetLength())
			{
				//5.4.5.1 현재 줄을 구한다.
				row = note->GetAt(rowIndex);
				//5.4.5.2 출력될 부분의 위치를 업데이트해준다.
				this->glyphXPos = glyphXPos;
				this->glyphYPos = i;
				//5.4.5.3 한페이지에 줄의 texts를 출력한다.
				row->Accept(this);
				//5.4.5.4 줄의 위치를 증가시킨다.
				rowIndex++;
				i++;
			}
			//5.4.6 한 페이지에 줄이 다찼기 때문에 현재 페이지를 닫는다.
			printDC->EndPage();
		}
	}
	//6. 페이지 설정 정보가 없으면
	else
	{
		//6.1 프린트할 노트의 줄의 개수보다 작은동안 반복한다.
		row = 0;
		rowIndex = 0;
		i = 0;
		while (rowIndex < printNote->GetLength())
		{
			//6.1.1 i를 초기화한다.
			i = 0;
			//6.1.2 인쇄할 한 페이지를 연다.
			printDC->StartPage();
			//6.1.3 한 페이지당 인쇄할 줄의 개수보다 작은동안 그리고 프리트할 노트의 줄의 개수보다 작은동안 반복한다.
			while (i < pageRowCount && rowIndex < printNote->GetLength())
			{
				//6.1.3.1 현재 줄을 구한다.
				row = note->GetAt(rowIndex);
				//6.1.3.2 출력될 부분의 위치를 업데이트해준다.
				this->glyphXPos = glyphXPos;
				this->glyphYPos = i;
				//6.1.3.3 한페이지에 줄의 texts를 출력한다.
				row->Accept(this);
				//6.1.3.4 줄의 위치를 증가시킨다.
				rowIndex++;
				i++;
			}
			//6.1.4 한 페이지에 줄이 다찼기 때문에 현재 페이지를 닫는다.
			printDC->EndPage();
		}
	}
}

//Row
void PrintingVisitor::VisitRow(Glyph* row)
{
	Glyph* letter = 0;//글자 주소
	string content;//글자 내용을 담을 공간
	Long letterWidth = 0;//글자너비
	Long letterIndex = 0;//줄에서 글자 위치
	//1. 프린트에 맞는 글꼴의 정보를 구해서 적용한다.
	CFont font;
	HFONT oldFont;
	font.CreateFontIndirect(&this->notepadForm->printInformation->GetPrintLogFont());
	oldFont = (HFONT)this->dc->SelectObject(font);
	//2. 프린트에 지정된 글꼴의 정보를 얻는다.
	TEXTMETRIC text;
	this->dc->GetTextMetrics(&text);
	//3. 줄의 글자개수보다 작은동안 반복한다.
	while (letterIndex < row->GetLength())
	{
		//3.1 현재 글자를 구한다.
		letter = row->GetAt(letterIndex);
		//3.2 현재 글자의 내용을 구한다.
		content = CString(letter->GetContent().c_str());
		//3.3 현재 글자의 너비를 기반으로 해서 메모장에서 글자의 X좌표 위치를 업데이트해준다.
		this->glyphXPos += letterWidth;
		letterWidth = this->dc->GetTextExtent(content.c_str()).cx;
		//3.4 현재 글자를 출력한다.
		letter->Accept(this);
		//3.5 글자의 위치를 증가시킨다.
		letterIndex++;
	}
}

//DummyRow
void PrintingVisitor::VisitDummyRow(Glyph* dummyRow)
{
	Glyph* letter = 0;//글자 주소
	string content;//글자 내용을 담을 공간
	Long letterWidth = 0;//글자너비
	Long letterIndex = 0;//줄에서 글자 위치
	//1. 프린트에 맞는 글꼴의 정보를 구해서 적용한다.
	CFont font;
	HFONT oldFont;
	font.CreateFontIndirect(&this->notepadForm->printInformation->GetPrintLogFont());
	oldFont = (HFONT)this->dc->SelectObject(font);
	//2. 프린트에 지정된 글꼴의 정보를 얻는다.
	TEXTMETRIC text;
	this->dc->GetTextMetrics(&text);
	//3. 줄의 글자개수보다 작은동안 반복한다.
	while (letterIndex < dummyRow->GetLength())
	{
		//3.1 현재 글자를 구한다.
		letter = dummyRow->GetAt(letterIndex);
		//3.2 현재 글자의 내용을 구한다.
		content = CString(letter->GetContent().c_str());
		//3.3 현재 글자의 너비를 기반으로 해서 메모장에서 글자의 X좌표 위치를 업데이트해준다.
		this->glyphXPos += letterWidth;
		letterWidth = this->dc->GetTextExtent(content.c_str()).cx;
		//3.4 현재 글자를 출력한다.
		letter->Accept(this);
		//3.5 글자의 위치를 증가시킨다.
		letterIndex++;
	}
}

//SingleByteLetter
void PrintingVisitor::VisitSingleByteLetter(Glyph* singleByteLetter)
{
	//1. 프린트에 맞는 글꼴의 정보를 구해서 적용한다.
	CFont font;
	HFONT oldFont;
	font.CreateFontIndirect(&this->notepadForm->printInformation->GetPrintLogFont());
	oldFont = (HFONT)this->dc->SelectObject(font);
	//2. 프린트에 지정된 글꼴 정보를 구한다.
	TEXTMETRIC text;
	this->dc->GetTextMetrics(&text);
	//3. 글자의 내용을 구한다.
	CString content = CString(singleByteLetter->GetContent().c_str());
	//4. 만약에 글자가 탭문자이면 내용을 띄어쓰기 8개로 바꿔준다.
	if (content == "\t")
	{
		content = "        ";
	}
	//5. 배경색을 흰색으로 설정한다.
	this->dc->SetBkColor(RGB(255, 255, 255));
	//6. 글꼴정보를 받아 글자색을 정한다.
	this->dc->SetTextColor(this->notepadForm->font.GetColor());
	//7. 페이지 설정 정보가 있으면
	Long glyphYPos = 0;
	if (this->notepadForm->pageSetUpInformation != NULL)
	{
		//7.1 페이지 설정 정보에서 위의 여백을 저장한다.
		glyphYPos = this->notepadForm->pageSetUpInformation->GetPrintableRect().top;
		//7.2 머리글이 있으면
		if (this->notepadForm->pageSetUpInformation->GetHeader().Compare("") != 0)
		{
			//7.2.1 머리글의 길이를 더해준다.
			glyphYPos += text.tmHeight;
		}
	}
	//8. 글자를 출력한다.
	this->dc->TextOut(this->glyphXPos, this->glyphYPos * text.tmHeight + glyphYPos, content);
	//9. font가 폰트공통대화상자에서 변경되었을때 기존 font를 지워야 새로 변경된 font로 적용할 수 있음.
	this->dc->SelectObject(oldFont);
	font.DeleteObject();
}

//DobuleByteLetter
void PrintingVisitor::VisitDoubleByteLetter(Glyph* doubleByteLetter)
{
	//1. 프린트에 맞는 글꼴의 정보를 구해서 적용한다.
	CFont font;
	HFONT oldFont;
	font.CreateFontIndirect(&this->notepadForm->printInformation->GetPrintLogFont());
	oldFont = (HFONT)this->dc->SelectObject(font);
	//2. 프린트에 지정된 글꼴 정보를 구한다.
	TEXTMETRIC text;
	this->dc->GetTextMetrics(&text);
	//3. 글자의 내용을 구한다.
	CString content = CString(doubleByteLetter->GetContent().c_str());
	//4. 배경색을 흰색으로 설정한다.
	this->dc->SetBkColor(RGB(255, 255, 255));
	//5. 글꼴정보를 받아 글자색을 정한다.
	this->dc->SetTextColor(this->notepadForm->font.GetColor());
	//6. 페이지 설정 정보가 있으면
	Long glyphYPos = 0;
	if (this->notepadForm->pageSetUpInformation != NULL)
	{
		//6.1 페이지 설정 정보에서 위의 여백을 저장한다.
		glyphYPos = this->notepadForm->pageSetUpInformation->GetPrintableRect().top;
		//6.2 머리글이 있으면
		if (this->notepadForm->pageSetUpInformation->GetHeader().Compare("") != 0)
		{
			//6.2.1 머리글의 길이를 더해준다.
			glyphYPos += text.tmHeight;
		}
	}
	//7. 글자를 출력한다.
	this->dc->TextOut(this->glyphXPos, this->glyphYPos * text.tmHeight + glyphYPos, content);
	//8. font가 폰트공통대화상자에서 변경되었을때 기존 font를 지워야 새로 변경된 font로 적용할 수 있음.
	this->dc->SelectObject(oldFont);
	font.DeleteObject();
}

//소멸자
PrintingVisitor::~PrintingVisitor()
{

}