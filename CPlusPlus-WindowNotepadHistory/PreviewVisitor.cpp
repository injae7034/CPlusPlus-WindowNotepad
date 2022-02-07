#include "PreviewVisitor.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "TextExtent.h"
#include "Font.h"
#include "PrintInformation.h"
#include "PreviewForm.h"
#include "Glyph.h"

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
	//3.배경을 회색으로 칠할 비트맵을 생성한다.
	//CDC dcBgc;
	//dcBgc.CreateCompatibleDC(this->dc);
	//HBITMAP hbmpBgc = ::CreateCompatibleBitmap(dcBgc, rect.Width(), rect.Height());
	//HBITMAP oldBMPBgc= (HBITMAP)dcBgc.SelectObject(hbmpBgc);
	//dcBgc.FillRect(&rect, CBrush::FromHandle((HBRUSH)GetStockObject(GRAY_BRUSH)));
	//4. 글자를 출력할 비트맵을 생성한다.(프린트 화면이 메모장 화면보다 더크기 때문에 메모장 화면에서
	//프린트 화면을 제대로 보여주기 위해서는 축소를 시켜줘야하는데 축소시키기 위해서는 비트맵을 생성해야한다.)
	CDC dcText;
	dcText.CreateCompatibleDC(this->dc);
	HBITMAP hbmp = ::CreateCompatibleBitmap(dcText, rect.Width(), rect.Height());
	HBITMAP oldBMP = (HBITMAP)dcText.SelectObject(hbmp);
	//5. 텍스트가 출력될 부분의 배경을 흰색으로 칠한다.
	dcText.FillRect(&rect, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	//6. 글자가 출력될 비트맵은 dc를 저장한다.
	this->tempDC = &dcText;
	dcText.SetMapMode(MM_ANISOTROPIC);
	dcText.SetWindowExt(12, 12);
	dcText.SetViewportExt(5, 5);
	//7. 인쇄 및 미리보기를 설정할 부분을 흰색으로 칠한다.
	//CDC dcSetUp;
	//dcSetUp.CreateCompatibleDC(this->dc);
	//HBITMAP hbmpSetUp = ::CreateCompatibleBitmap(dcSetUp, rect.Width(), rect.Height());
	//HBITMAP oldBMPSetUp = (HBITMAP)dcSetUp.SelectObject(hbmpSetUp);
	//dcSetUp.FillRect(&rect, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	//dcSetUp.SetMapMode(MM_ANISOTROPIC);
	//dcSetUp.SetWindowExt(12, 12);
	//dcSetUp.SetViewportExt(5, 5);
	//8. 한 페이지당 인쇄할 줄의 개수보다 작은동안 그리고 프린트할 노트의 줄의 개수보다 작은동안 반복한다.
	Glyph* row = 0;
	Long rowIndex = 0;
	while (rowIndex < this->notepadForm->previewForm->GetNote()->GetLength())
	{
		//8.1 현재 줄을 구한다.
		row = note->GetAt(rowIndex);
		//8.2 출력될 부분의 위치를 업데이트해준다.
		this->glyphXPos = 0;
		this->glyphYPos = rowIndex;
		//8.3 한페이지에 줄의 texts를 출력한다.
		row->Accept(this);
		//8.4 줄의 위치를 증가시킨다.
		rowIndex++;
	}
	//9. 비트맵이 축소되면서 글자가 뭉개지는걸 방지하기 위한 조치
	this->dc->SetStretchBltMode(HALFTONE);
	//10. 미리보기 폼의 정중앙을 구한다.
	CRect clientRect;
	this->notepadForm->previewForm->GetClientRect(&clientRect);
	CPoint centerPoint = clientRect.CenterPoint();
	//11. 클라이언트 영역의 크기가 바뀐 만큼의 비율을 구한다. 
	double changedRectWidth = clientRect.Width();
	double changedRectHeight = clientRect.Height();
	double firstRectWidth = this->notepadForm->previewForm->GetFirstClientRect().Width();
	double firstRectHeight = this->notepadForm->previewForm->GetFirstClientRect().Height();
	double multiplicandWidth = changedRectWidth / firstRectWidth;
	double multiplicandHeight = changedRectHeight / firstRectHeight;
	//12. 비트맵 축소 크기를 구한다.
	double downsizedWidth = rect.Width() / 10;
	double downsizedHeight = rect.Height() / 10;
	//13. 프린트화면 크기에 맞게 생성한 회색배경 비트맵을 현재 화면크기에 맞게 축소시켜서 출력한다.
	//this->dc->StretchBlt(0, 0, rect.Width(), rect.Height(), &dcBgc,
	//	0, 0, rect.Width(), rect.Height(), SRCCOPY);
	//dcBgc.SelectObject(oldBMPBgc);
	//dcBgc.DeleteDC();
	CRect fillRect(0, 0, changedRectWidth / 10 * 7, changedRectHeight);
	this->dc->FillRect(&fillRect, CBrush::FromHandle((HBRUSH)GetStockObject(GRAY_BRUSH)));
	//14. 프린트화면 크기에 맞게 생성한 글자비트맵을 현재 화면크기에 맞게 축소시켜서
	//화면의 10/7 영역에 출력한다.
	this->dc->StretchBlt(changedRectWidth / 10 * 7 /2 - downsizedWidth / 2 * multiplicandWidth,
		centerPoint.y - downsizedHeight / 2 * multiplicandHeight,
		downsizedWidth * multiplicandWidth, downsizedHeight * multiplicandHeight,
		&dcText, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
	dcText.SelectObject(oldBMP);
	::DeleteObject(hbmp);
	dcText.DeleteDC();
	fillRect = CRect(changedRectWidth / 10 * 7 / 2 + downsizedWidth / 2 * multiplicandWidth
		+ changedRectWidth / 10 * 7 / 2 - downsizedWidth / 2 * multiplicandWidth, 0,
		rect.Width(), rect.Height());
	this->dc->FillRect(&fillRect, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	//15. 프린트화면 크기에 맞게 생성한  인쇄 및 미리보기 비트맵을 현재 화면크기에 맞게 축소시켜서
	//화면의 10/3 영역에 출력한다.
	//this->dc->StretchBlt(changedRectWidth / 10 * 7 /2 + downsizedWidth / 2 * multiplicandWidth
	//	+ changedRectWidth / 10 * 7 / 2 - downsizedWidth / 2 * multiplicandWidth, 0,
	//	rect.Width(), rect.Height(), &dcSetUp, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
	//dcSetUp.SelectObject(oldBMPSetUp);
	//::DeleteObject(hbmpSetUp);
	//dcSetUp.DeleteDC();
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