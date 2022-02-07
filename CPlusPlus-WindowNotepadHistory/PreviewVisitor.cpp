#include "PreviewVisitor.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "TextExtent.h"
#include "Font.h"
#include "PrintInformation.h"
#include "PreviewForm.h"

//디폴트생성자
PreviewVisitor::PreviewVisitor(NotepadForm* notepadForm, CDC* dc,
	Long glyphXPos, Long glyphYPos)
	:GlyphVisitor(notepadForm, dc, glyphXPos, glyphYPos)
{
	this->tempDC = 0;
}

//복사생성자
PreviewVisitor::PreviewVisitor(const PreviewVisitor& source)
	:GlyphVisitor(source)
{
	this->tempDC = source.tempDC;
}

//치환연산자
PreviewVisitor& PreviewVisitor::operator=(const PreviewVisitor& source)
{
	GlyphVisitor::operator=(source);

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
	//3. 프린트가 가능한 영역을 회색으로 칠한다.
	CDC dcBgc;
	dcBgc.CreateCompatibleDC(this->dc);
	HBITMAP hbmpBgc = ::CreateCompatibleBitmap(dcBgc, rect.Width(), rect.Height());
	HBITMAP oldBMPBgc= (HBITMAP)dcBgc.SelectObject(hbmpBgc);
	dcBgc.FillRect(&rect, CBrush::FromHandle((HBRUSH)GetStockObject(GRAY_BRUSH)));
	//4. 프린트할 노트를 구한다.
	Glyph* printNote = this->notepadForm->printInformation->GetPrintNote();
	//5. 한 페이지당 인쇄할 줄의 수를 구한다.
	Long pageRowCount = this->notepadForm->printInformation->GetPageRowCount();
	//6. 비트맵을 생성한다.(프린트 화면이 메모장 화면보다 더크기 때문에 메모장 화면에서 프린트 화면을
	//제대로 보여주기 위해서는 축소를 시켜줘야하는데 축소시키기 위해서는 비트맵을 생성해야한다.)
	CDC dcText;
	dcText.CreateCompatibleDC(this->dc);
	HBITMAP hbmp = ::CreateCompatibleBitmap(dcText, rect.Width(), rect.Height());
	HBITMAP oldBMP = (HBITMAP)dcText.SelectObject(hbmp);
	//텍스트가 출력될 부분을 하얀색으로 칠한다.
	dcText.FillRect(&rect, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	this->tempDC = &dcText;
	dcText.SetMapMode(MM_ANISOTROPIC);
	dcText.SetWindowExt(12, 12);
	dcText.SetViewportExt(5, 5);
	//7. 한 페이지당 인쇄할 줄의 개수보다 작은동안 그리고 프린트할 노트의 줄의 개수보다 작은동안 반복한다.
	Glyph* row = 0;
	Long rowIndex = 0;
	Long i = 0;
	while (i < pageRowCount && rowIndex < printNote->GetLength())
	{
		//7.1 현재 줄을 구한다.
		row = note->GetAt(rowIndex);
		//7.2 출력될 부분의 위치를 업데이트해준다.
		this->glyphXPos = 0;
		this->glyphYPos = i;
		//7.3 한페이지에 줄의 texts를 출력한다.
		row->Accept(this);
		//7.4 줄의 위치를 증가시킨다.
		rowIndex++;
		i++;
	}
	//8. 비트맵이 축소되면서 글자가 뭉개지는걸 방지하기 위한 조치
	this->dc->SetStretchBltMode(HALFTONE);
	//9. 미리보기 폼의 정중앙을 구한다.
	CRect clientRect;
	this->notepadForm->previewForm->GetClientRect(&clientRect);
	CPoint centerPoint = clientRect.CenterPoint();
	//10. 클라이언트 영역의 크기가 바뀐 만큼의 비율을 구한다. 
	double changedRectWidth = clientRect.Width();
	double changedRectHeight = clientRect.Height();
	double multiplicandWidth = changedRectWidth 
		/ this->notepadForm->previewForm->GetOriginalRectWidth();
	double multiplicandHeight = changedRectHeight 
		/ this->notepadForm->previewForm->GetOriginalRectHeight();
	//11. 비트맵 축소 크기를 구한다.
	double downsizedWidth = rect.Width() / 10;
	double downsizedHeight = rect.Height() / 10;
	
	this->dc->StretchBlt(0, 0, rect.Width(), rect.Height(), &dcBgc,
		0, 0, rect.Width(), rect.Height(), SRCCOPY);
	dcBgc.SelectObject(oldBMPBgc);
	::DeleteObject(hbmpBgc);
	dcBgc.DeleteDC();
	
	//11. 프린트화면 크기에 맞게 생성한 비트맵을 현재 화면크기에 맞게 축소시켜서 미리보기 폼의 중앙에 출력한다.
	this->dc->StretchBlt(centerPoint.x - downsizedWidth / 2 * multiplicandWidth,
		centerPoint.y - downsizedHeight / 2 * multiplicandHeight,
		downsizedWidth * multiplicandWidth, downsizedHeight * multiplicandHeight,
		&dcText, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
	//12. 비트맵을 선택한다.
	dcText.SelectObject(oldBMP);
	//13.HBITMAP과 CDC를 지운다.
	::DeleteObject(hbmp);
	dcText.DeleteDC();
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
	//7. 글자를 출력한다.
	this->tempDC->TextOut(this->glyphXPos, this->glyphYPos * text.tmHeight, content);
	//8. font가 폰트공통대화상자에서 변경되었을때 기존 font를 지워야 새로 변경된 font로 적용할 수 있음.
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
	//4. 만약에 글자가 탭문자이면 내용을 띄어쓰기 8개로 바꿔준다.
	if (content == "\t")
	{
		content = "        ";
	}
	//5. 배경색을 흰색으로 설정한다.
	this->tempDC->SetBkColor(RGB(255, 255, 255));
	//6. 글꼴정보를 받아 글자색을 정한다.
	this->tempDC->SetTextColor(this->notepadForm->font.GetColor());
	//7. 글자를 출력한다.
	this->tempDC->TextOut(this->glyphXPos, this->glyphYPos * text.tmHeight, content);
	//8. font가 폰트공통대화상자에서 변경되었을때 기존 font를 지워야 새로 변경된 font로 적용할 수 있음.
	this->tempDC->SelectObject(oldFont);
	font.DeleteObject();
}

//소멸자
PreviewVisitor::~PreviewVisitor()
{

}