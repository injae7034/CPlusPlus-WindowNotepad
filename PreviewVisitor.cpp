#include "PreviewVisitor.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "TextExtent.h"
#include "Font.h"
#include "PrintInformation.h"
#include "PreviewForm.h"
#include "Glyph.h"
#include "PageSetUpInformation.h"

//디폴트생성자
PreviewVisitor::PreviewVisitor(NotepadForm* notepadForm, CDC* dc,
	Long glyphXPos, Long glyphYPos)
	:GlyphVisitor(notepadForm, dc, glyphXPos, glyphYPos)//공통적인 부분 처리
{
	this->tempDC = 0;//previewVisitor에만 있는 예외 부분 처리
}

//복사생성자
PreviewVisitor::PreviewVisitor(const PreviewVisitor& source)
	:GlyphVisitor(source)//공통적인 부분 처리
{
	this->tempDC = source.tempDC;//previewVisitor에만 있는 예외 부분 처리
}

//치환연산자
PreviewVisitor& PreviewVisitor::operator=(const PreviewVisitor& source)
{
	GlyphVisitor::operator=(source);//공통적인 부분 처리

	this->tempDC = source.tempDC;//previewVisitor에만 있는 예외 부분 처리

	return *this;
}

//Note 미리보기는 한페이지씩만 보여주면 된다. 다음페이지는 다음페이지 버튼을 누르면
//다음페이지 한페이지만 보여주면 된다.
void PreviewVisitor::VisitNote(Glyph* note)
{
	//1. 프린트 다이얼로그의 hdc에서 cdc를 구한다.
	CDC* cdc = CDC::FromHandle(this->notepadForm->printInformation->GetHDC());
	//2. 프린트 가능한 영역을 구한다. 
	CRect rect(0, 0, cdc->GetDeviceCaps(HORZRES), cdc->GetDeviceCaps(VERTRES));
	//3. 글자를 출력할 비트맵을 생성한다.(프린트 화면이 메모장 화면보다 더크기 때문에 메모장 화면에서
	//프린트 화면을 제대로 보여주기 위해서는 축소를 시켜줘야하는데 축소시키기 위해서는 비트맵을 생성해야한다.)
	CDC dcText;
	dcText.CreateCompatibleDC(this->dc);
	HBITMAP hbmp = ::CreateCompatibleBitmap(dcText, rect.Width(), rect.Height());
	HBITMAP oldBMP = (HBITMAP)dcText.SelectObject(hbmp);
	//4. 텍스트가 출력될 부분의 배경을 흰색으로 칠한다.
	dcText.FillRect(&rect, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	//5. 글자가 출력될 비트맵은 dc를 저장한다.
	this->tempDC = &dcText;
	//6. 미리보기 폼의 정중앙을 구한다.
	CRect clientRect;
	this->notepadForm->previewForm->GetClientRect(&clientRect);
	CPoint centerPoint = clientRect.CenterPoint();
	//7. 클라이언트 영역의 크기가 바뀐 만큼의 비율을 구한다. 
	double changedRectWidth = clientRect.Width();
	double changedRectHeight = clientRect.Height();
	double firstRectWidth = this->notepadForm->previewForm->GetFirstClientRect().Width();
	double firstRectHeight = this->notepadForm->previewForm->GetFirstClientRect().Height();
	double multiplicandWidth = changedRectWidth / firstRectWidth;
	double multiplicandHeight = changedRectHeight / firstRectHeight;
	//8. 비트맵 축소 크기를 구한다.
	double downsizedWidth = rect.Width() / 10;
	double downsizedHeight = rect.Height() / 10;
	//9. 페이지 설정 정보가 있으면
	Long glyphXPos = 0;
	if (this->notepadForm->pageSetUpInformation != NULL)
	{
		//9.1 여백을 제외하고 프린트 가능한 영역의 왼쪽 위치를 저장한다.
		glyphXPos = this->notepadForm->pageSetUpInformation->GetPrintableRect().left;
		//9.2 프린트에 맞는 글꼴의 정보를 구해서 적용한다.
		CFont font;
		HFONT oldFont;
		TEXTMETRIC text;
		font.CreateFontIndirect(&this->notepadForm->printInformation->GetPrintLogFont());
		oldFont = (HFONT)dcText.SelectObject(font);
		dcText.GetTextMetrics(&text);
		Long letterWidth = 0;
		//9.3 머리글이 있으면
		if (this->notepadForm->pageSetUpInformation->GetHeader().Compare("") != 0)
		{
			//9.3.1 머리글의 길이를 구한다.
			letterWidth = dcText.GetTextExtent
			(this->notepadForm->pageSetUpInformation->GetHeader()).cx;
			//9.3.2 머리글을 출력한다.
			//뒤에서 strechBlt로 축소하기 때문에 축소되지 않은 프린트가 가능한 영역의 절반에서
			//머리글의 절반 값을 뺀 곳에서 시작한다.
			dcText.TextOut(rect.Width() / 2 - letterWidth / 2,
				this->notepadForm->pageSetUpInformation->GetPrintableRect().top,
				this->notepadForm->pageSetUpInformation->GetHeader());
		}
		//9.4 바닥글이 있으면
		if (this->notepadForm->pageSetUpInformation->GetFooter().Compare("") != 0)
		{
			//9.4.1 바닥글의 길이를 구한다.
			letterWidth = dcText.GetTextExtent
			(this->notepadForm->pageSetUpInformation->GetFooter()).cx;
			//9.4.2 바닥글을 출력한다.
			//뒤에서 strechBlt로 축소하기 때문에 축소되지 않은 프린트가 가능한 영역의 절반에서
			//머리글의 절반 값을 뺀 곳에서 시작한다.
			dcText.TextOut(rect.Width() / 2 - letterWidth / 2,
				this->notepadForm->pageSetUpInformation->GetPrintableRect().bottom 
				- text.tmHeight,
				this->notepadForm->pageSetUpInformation->GetFooter());
		}
	}
	//10. 미리보기 폼에서 한 페이지당 출력될 줄의 개수보다 작은동안 반복한다.
	Glyph* row = 0;
	Long rowIndex = 0;
	while (rowIndex < this->notepadForm->previewForm->GetNote()->GetLength())
	{
		//10.1 현재 줄을 구한다.
		row = note->GetAt(rowIndex);
		//10.2 출력될 부분의 위치를 업데이트해준다.
		this->glyphXPos = glyphXPos;
		this->glyphYPos = rowIndex;
		//10.3 한페이지에 줄의 texts를 출력한다.
		row->Accept(this);
		//10.4 줄의 위치를 증가시킨다.
		rowIndex++;
	}
	//11. 비트맵이 축소되면서 글자가 뭉개지는걸 방지하기 위한 조치
	this->dc->SetStretchBltMode(HALFTONE);
	//12. 미리보기폼에서 클라이언트영역의 7/10을 회색으로 칠한다.
	CRect fillRect(0, 0, changedRectWidth / 10 * 7, changedRectHeight);
	this->dc->FillRect(&fillRect, CBrush::FromHandle((HBRUSH)GetStockObject(GRAY_BRUSH)));
	//13. 페이지 설정 정보가 있고, 페이지 방향이 가로이면 또는 인쇄가능영역의 너비가 높이보다 크면
	if ((this->notepadForm->pageSetUpInformation != 0 &&
		this->notepadForm->pageSetUpInformation->GetOrientation() != 1)
		|| rect.Width() > rect.Height())
	{
		//13.1 가로방향에 맞게 축소된 크기를 재조정해준다.
		downsizedWidth = rect.Width() / 7;
		downsizedHeight = rect.Height() / 7;
		//13.2 프린트화면 크기에 맞게 생성한 글자비트맵을 현재 화면크기에 맞게 축소시켜서
		//클라이언트의 10/7 영역에 출력한다.
		this->dc->StretchBlt(changedRectWidth / 10 * 7 / 2 - (downsizedHeight / 2
			* multiplicandWidth * 7016 / rect.Height()),
			centerPoint.y - (downsizedWidth / 2 * multiplicandHeight * 4958 / rect.Width()),
			downsizedHeight * multiplicandWidth * 7016 / rect.Height(),
			downsizedWidth * multiplicandHeight * 4958 / rect.Width(),
			&dcText, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
	}
	//14. 페이지 설정 정보가 없거나 인쇄 가능역역의 너비가 높이보다 작거나 같으면
	else
	{
		//14.1 프린트화면 크기에 맞게 생성한 글자비트맵을 현재 화면크기에 맞게 축소시켜서
		//클라이언트영역의 10/7 영역에 출력한다.
		Long textStart = changedRectWidth / 10 * 7 / 2 - (downsizedWidth / 2
			* multiplicandWidth * 4958 / rect.Width());
		Long textEnd = downsizedWidth * multiplicandWidth * 4958 / rect.Width();
		this->dc->StretchBlt(textStart,
			centerPoint.y - (downsizedHeight / 2 * multiplicandHeight * 7016 / rect.Height()),
			textEnd,
			downsizedHeight * multiplicandHeight * 7016 / rect.Height(),
			&dcText, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
	}
	//15. 텍스트를 출력한 dc를 제거한다.
	dcText.SelectObject(oldBMP);
	::DeleteObject(hbmp);
	dcText.DeleteDC();
	//16. 미리보기 페이지 설정영역을 흰색으로 칠한다.
	fillRect = CRect(changedRectWidth / 10 * 7 / 2 + downsizedHeight / 2
		* multiplicandWidth + changedRectWidth / 10 * 7 / 2 - downsizedHeight / 2
		* multiplicandWidth, 0,
		changedRectWidth, changedRectHeight);
	this->dc->FillRect(&fillRect, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
}

//Row
void PreviewVisitor::VisitRow(Glyph* row)
{
	Glyph* letter = 0;//글자 주소
	string content;//글자 내용을 담을 공간
	Long letterWidth = 0;//글자너비
	Long letterIndex = 0;//줄에서 글자 위치
	//1. 프린트에 맞는 글꼴의 정보를 구해서 적용한다.
	CFont font;
	HFONT oldFont;
	font.CreateFontIndirect(&this->notepadForm->printInformation->GetPrintLogFont());
	oldFont = (HFONT)this->tempDC->SelectObject(font);
	//2. 프린트에 지정된 글꼴의 정보를 얻는다.
	TEXTMETRIC text;
	this->tempDC->GetTextMetrics(&text);
	//3. 줄의 글자개수보다 작은동안 반복한다.
	while (letterIndex < row->GetLength())
	{
		//3.1 현재 글자를 구한다.
		letter = row->GetAt(letterIndex);
		//3.2 현재 글자의 내용을 구한다.
		content = CString(letter->GetContent().c_str());
		//3.3 현재 글자의 너비를 기반으로 해서 메모장에서 글자의 X좌표 위치를 업데이트해준다.
		this->glyphXPos += letterWidth;
		letterWidth = this->tempDC->GetTextExtent(content.c_str()).cx;
		//3.4 현재 글자를 출력한다.
		letter->Accept(this);
		//3.5 글자의 위치를 증가시킨다.
		letterIndex++;
	}
}

//DummyRow
void PreviewVisitor::VisitDummyRow(Glyph* dummyRow)
{
	Glyph* letter = 0;//글자 주소
	string content;//글자 내용을 담을 공간
	Long letterWidth = 0;//글자너비
	Long letterIndex = 0;//줄에서 글자 위치
	//1. 프린트에 맞는 글꼴의 정보를 구해서 적용한다.
	CFont font;
	HFONT oldFont;
	font.CreateFontIndirect(&this->notepadForm->printInformation->GetPrintLogFont());
	oldFont = (HFONT)this->tempDC->SelectObject(font);
	//2. 프린트에 지정된 글꼴의 정보를 얻는다.
	TEXTMETRIC text;
	this->tempDC->GetTextMetrics(&text);
	//3. 줄의 글자개수보다 작은동안 반복한다.
	while (letterIndex < dummyRow->GetLength())
	{
		//3.1 현재 글자를 구한다.
		letter = dummyRow->GetAt(letterIndex);
		//3.2 현재 글자의 내용을 구한다.
		content = CString(letter->GetContent().c_str());
		//3.3 현재 글자의 너비를 기반으로 해서 메모장에서 글자의 X좌표 위치를 업데이트해준다.
		this->glyphXPos += letterWidth;
		letterWidth = this->tempDC->GetTextExtent(content.c_str()).cx;
		//3.4 현재 글자를 출력한다.
		letter->Accept(this);
		//3.5 글자의 위치를 증가시킨다.
		letterIndex++;
	}
}

//SingleByteLetter
void PreviewVisitor::VisitSingleByteLetter(Glyph* singleByteLetter)
{
	//1. 프린트에 맞는 글꼴의 정보를 구해서 적용한다.
	CFont font;
	HFONT oldFont;
	font.CreateFontIndirect(&this->notepadForm->printInformation->GetPrintLogFont());
	oldFont = (HFONT)this->tempDC->SelectObject(font);
	//2. 프린트에 지정된 글꼴 정보를 구한다.
	TEXTMETRIC text;
	this->tempDC->GetTextMetrics(&text);
	//3. 글자의 내용을 구한다.
	CString content = CString(singleByteLetter->GetContent().c_str());
	//4. 만약에 글자가 탭문자이면 내용을 띄어쓰기 8개로 바꿔준다.
	if (content == "\t")
	{
		content = "        ";
	}
	//5. 배경색을 흰색으로 설정한다.
	this->tempDC->SetBkColor(RGB(255, 255, 255));
	//6. 글꼴정보를 받아 글자색을 정한다.
	this->tempDC->SetTextColor(this->notepadForm->font.GetColor());
	//7. 페이지설정 정보가 있으면
	Long glyphYPos = 0;//텍스트가 출력될 y좌표
	if (this->notepadForm->pageSetUpInformation != NULL)
	{
		//7.1 페이지 설정에서 위의 여백 설정 정보를 y좌표에 저장한다.
		glyphYPos = this->notepadForm->pageSetUpInformation->GetPrintableRect().top;
		//7.2 페이지 설정에서 머리글이 설정되어 있으면
		if (this->notepadForm->pageSetUpInformation->GetHeader().Compare("") != 0)
		{
			//7.2.1 머리글 글자 길이만큼 추가로 y좌표를 증가시킨다.
			glyphYPos += text.tmHeight;
		}
	}
	//8. 글자를 출력한다.
	this->tempDC->TextOut(this->glyphXPos, this->glyphYPos * text.tmHeight + glyphYPos,
		content);
	//9. font가 폰트공통대화상자에서 변경되었을때 기존 font를 지워야 새로 변경된 font로 적용할 수 있음.
	this->tempDC->SelectObject(oldFont);
	font.DeleteObject();
}

//DobuleByteLetter
void PreviewVisitor::VisitDoubleByteLetter(Glyph* doubleByteLetter)
{
	//1. 프린트에 맞는 글꼴의 정보를 구해서 적용한다.
	CFont font;
	HFONT oldFont;
	font.CreateFontIndirect(&this->notepadForm->printInformation->GetPrintLogFont());
	oldFont = (HFONT)this->tempDC->SelectObject(font);
	//2. 프린트에 지정된 글꼴 정보를 구한다.
	TEXTMETRIC text;
	this->tempDC->GetTextMetrics(&text);
	//3. 글자의 내용을 구한다.
	CString content = CString(doubleByteLetter->GetContent().c_str());
	//4. 배경색을 흰색으로 설정한다.
	this->tempDC->SetBkColor(RGB(255, 255, 255));
	//5. 글꼴정보를 받아 글자색을 정한다.
	this->tempDC->SetTextColor(this->notepadForm->font.GetColor());
	//6. 페이지설정 정보가 있으면
	Long glyphYPos = 0;//텍스트가 출력될 y좌표
	if (this->notepadForm->pageSetUpInformation != NULL)
	{
		//6.1 페이지 설정에서 위의 여백 설정 정보를 y좌표에 저장한다.
		glyphYPos = this->notepadForm->pageSetUpInformation->GetPrintableRect().top;
		//6.2 페이지 설정에서 머리글이 설정되어 있으면
		if (this->notepadForm->pageSetUpInformation->GetHeader().Compare("") != 0)
		{
			//6.2.1 머리글 글자 길이만큼 추가로 y좌표를 증가시킨다.
			glyphYPos += text.tmHeight;
		}
	}
	//7. 글자를 출력한다.
	this->tempDC->TextOut(this->glyphXPos, this->glyphYPos * text.tmHeight + glyphYPos,
		content);
	//8. font가 폰트공통대화상자에서 변경되었을때 기존 font를 지워야 새로 변경된 font로 적용할 수 있음.
	this->tempDC->SelectObject(oldFont);
	font.DeleteObject();
}

//소멸자
PreviewVisitor::~PreviewVisitor()
{

}