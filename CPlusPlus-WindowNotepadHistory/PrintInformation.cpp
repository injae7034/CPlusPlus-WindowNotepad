#include "PrintInformation.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "DummyRow.h"

PrintInformation::PrintInformation(NotepadForm* notepadForm, LOGFONT printLogFont,
	CDC* printerDC)
{
	this->notepadForm = notepadForm;
	this->printNote = notepadForm->note->Clone();
	this->printLogFont = printLogFont;
	this->printDC = printerDC;
	//한 페이지당 줄의 수를 구한다.
	CFont font;
	HFONT oldFont;
	font.CreateFontIndirect(&this->printLogFont);
	oldFont = (HFONT)this->printDC->SelectObject(font);
	TEXTMETRIC text;
	this->printDC->GetTextMetrics(&text);
	this->printableAreaHeight = this->printDC->GetDeviceCaps(VERTRES);
	this->pageRowCount = this->printableAreaHeight / text.tmHeight;

	//1. 메모정에 선택영역이 있으면
	if (this->notepadForm->isSelecting == true)
	{
		//선택영역을 해제한다.
		//1. 선택된 범위를 구한다.
		Long startingRowPos = 0;
		Long startingLetterPos = 0;
		Long endingRowPos = 0;
		Long endingLetterPos = 0;
		this->printNote->CalculateSelectedRange(&startingRowPos,
			&startingLetterPos, &endingRowPos, &endingLetterPos);
		Long rowIndex = startingRowPos;
		Long index = startingLetterPos;
		Long rowCount = this->printNote->GetLength();
		Long letterCount = 0;
		Long letterIndex = 0;
		Glyph* row = 0;
		Glyph* letter = 0;
		bool isSelected = true;
		//2. 선택이 시작된 줄의 위치가 선택이 끝나는 줄의 위치보다 작거나 같은동안 반복한다.
		while (rowIndex <= endingRowPos && isSelected == true)
		{
			//2.1 줄을 구한다.
			row = this->printNote->GetAt(rowIndex);
			//2.2 줄의 글자개수를 구한다.
			letterCount = row->GetLength();
			//2.3 글자 위치를 원위치시킨다.
			letterIndex = index;
			//2.4 글자위치가 글자개수보다 작은동안 반복한다.
			while (letterIndex < letterCount && isSelected == true)
			{
				//2.4.1 글자를 구한다.
				letter = row->GetAt(letterIndex);
				//2.4.2 글자가 선택이 되어있으면
				if (letter->IsSelected() == true)
				{
					//2.4.2.1 현재 글자를 선택이 안된 상태로 바꿔준다.
					letter->Select(false);
				}
				//2.4.3 글자가 선택이 안되어있으면
				else
				{
					isSelected = false;
				}
				//2.4.4 글자 위치를 증가시킨다.
				letterIndex++;
			}
			//2.5 줄의 위치를 증가시킨다.
			rowIndex++;
			index = 0;
		}
	}
	Long rowIndex = 0;
	Glyph* row = 0;
	Glyph* previousRow = 0;
	//2. 메모장에서 자동개행이 진행중이면
	if (this->notepadForm->isRowAutoChanging == true)
	{
		// 전체자동개행을 취소한다.
		//1. Note의 총 줄의 개수보다 작은동안 반복한다.
		while (rowIndex < this->printNote->GetLength())
		{
			//1.1 메모장에서 rowIndex번째 줄을 구한다.
			row = this->printNote->GetAt(rowIndex);
			//1.2 가짜줄(DummyRow)이면
			if (dynamic_cast<DummyRow*>(row))
			{
				//1.2.1 가짜줄(DummyRow) 이전의 진짜줄(Row)을 구한다.
				previousRow = this->printNote->GetAt(rowIndex - 1);
				//1.2.2 가짜줄(DummyRow)을 이전의 진짜줄(Row)에 합친다.
				row->Join(previousRow);
				//1.2.3 Note에서 가짜줄(DummyRow)의 주소를 지운다.
				this->printNote->Remove(rowIndex);
			}
			//1.3 가짜줄(DummyRow)이 아니면(진짜줄(Row)이면)
			else
			{
				//1.3.1 다음 줄로 이동한다.
				rowIndex++;
			}
		}
	}
	//3. 전체 자동개행을 다시해준다.
	Long letterIndex = 0;
	Long rowTextWidth = 0;
	Glyph* glyph = 0;
	rowIndex = 0;
	row = 0;
	previousRow = 0;
	//1. 현재 프린트 가능한 화면의 크기를 구한다.
	CRect rect(0, 0, this->printDC->GetDeviceCaps(HORZRES),
		this->printDC->GetDeviceCaps(VERTRES));
	//2. 현재 화면의 가로 길이를 구한다.
	Long pageWidth = rect.Width();
	//3. Note의 총 줄의 개수보다 작은동안 반복한다.
	while (rowIndex < this->printNote->GetLength())
	{
		//3.1 메모장에서 rowIndex번째 줄을 구한다.
		row = this->printNote->GetAt(rowIndex);
		//3.2 letterIndex를 원위치시킨다.
		letterIndex = 0;
		//3.3 rowTextWidth를 원위치시킨다.
		rowTextWidth = 0;
		//3.4 letterIndex가 rowIndex번째 줄의 총글자 개수보다 작은동안 
		//그리고 rowIndex번째 줄의 가로길이가 현재화면의 가로길이보다 작은동안 반복한다.
		while (letterIndex < row->GetLength() && rowTextWidth < pageWidth)
		{
			//3.4.1 증가된 letterIndex까지의 가로 길이를 측정한다.
			rowTextWidth = this->printDC->GetTextExtent
			(row->GetPartOfContent(letterIndex + 1).c_str()).cx;
			//3.4.2 letterIndex를 증가시킨다.
			letterIndex++;
		}
		//3.5 rowIndex번째 줄의 가로 길이가 현재 화면의 가로 길이보다 크거나 같으면
		if (rowTextWidth >= pageWidth)
		{
			//3.5.1 letterIndex까지의 길이가 현재화면의 가로 길이(cx)보다 크기 때문에 
			//이 선택문에 들어왔다. 그래서 캐럿이 이전으로 한 칸 이동을 해서 길이를 재면
			//현재화면의 가로 길이(cx)보다 작다. 캐럿(letterIndex)은 다음 글자를 적을 위치를
			//반영하기 때문에 항상 현재 글자보다 한칸 앞서 있다
			//그래서 letterIndex-1에서 split을 해야 화면을 넘는 글자를 다음 줄로 보낼 수 있다.
			letterIndex--;
			//3.5.2 rowIndex번째 줄의 가로 길이가 현재화면의 가로 길이보다 커진 시점의
			//글자부터 rowIndex번째 줄에서 letterIndex 다음 위치에 있는 글자들을 나눈다.
			//(DummyRow생성)
			glyph = row->Split(letterIndex, true);
			//3.5.3 새로운 줄을 rowIndex번째 줄의 다음 위치에 끼워넣는다.
			rowIndex = this->printNote->Add(rowIndex + 1, glyph);
		}
		//3.6 letterIndex가 rowIndex번째 줄의 총글자 개수보다 크거나 같으면
		else if (letterIndex >= row->GetLength())
		{
			//3.6.1 다음 줄로 이동한다.
			rowIndex++;
		}
	}
}

HBITMAP PrintInformation::CreateBitmap()
{
	CRect rect(0, 0, this->printDC->GetDeviceCaps(HORZRES),
		this->printDC->GetDeviceCaps(VERTRES));
	CDC dcTemp;
	dcTemp.CreateCompatibleDC(this->printDC);
	HBITMAP hbmp = ::CreateCompatibleBitmap(dcTemp, rect.right, rect.bottom);
	HBITMAP oldBMP = (HBITMAP)dcTemp.SelectObject(hbmp);
	dcTemp.FillRect(&rect, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));

	return hbmp;
}

PrintInformation::~PrintInformation()
{
	if (this->printNote != NULL)
	{
		delete this->printNote;
	}
	if (this->printDC != NULL)
	{
		this->printDC->DeleteDC();
	}
}