#include "LetterTextOutVisitor.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "TextExtent.h"

//디폴트생성자
LetterTextOutVisitor::LetterTextOutVisitor(NotepadForm* notepadForm)
	:TextOutVisitor(notepadForm)
{

}

void LetterTextOutVisitor::VisitNoteTextOut(Glyph* note)
{
	//1. CPaintDC를 생성한다.
	CPaintDC dc(notepadForm);
	//2. 왼쪽을 기준선으로 정함.
	dc.SetTextAlign(TA_LEFT);
	//3. CFont를 생성한다.
	CFont font;
	//4. 글씨크기와 글씨체를 정하다.
	font.CreateFontIndirect(&notepadForm->font.GetLogFont());
	//5. 폰트를 dc에 지정한다.
	HFONT oldFont;
	oldFont = (HFONT)dc.SelectObject(font);
	//6. TEXTMETRIC을 생성한다.
	TEXTMETRIC text;
	//7. 글꼴의 정보를 얻는다.
	dc.GetTextMetrics(&text);
	//8. note에 저장된 글자들을 출력한다.
	Long rowIndex = 0;
	Long currentXPos;
	Long currentYPos;
	CString content;

	Long currentWidth = 0;
	Long letterWidth = 0;
	Long letterCount = 0;
	Long letterIndex = 0;
	Glyph* row = 0;
	Glyph* letter = 0;

	//9. 줄단위의 반복구조를 통해서 줄을 나눠서 줄개수만큼 출력하도록 함.
	while (rowIndex < notepadForm->note->GetLength())
	{
		//9.1 현재 줄을 구한다.
		row = notepadForm->note->GetAt(rowIndex);
		//9.2 스크롤의 위치를 구한다.
		currentXPos = notepadForm->GetScrollPos(SB_HORZ);
		currentYPos = notepadForm->GetScrollPos(SB_VERT);
		//9.3 현재줄의 첫 글자를 구한다.
		letterIndex = 0;
		currentWidth = 0;
		letterWidth = 0;
		//9.4 현재줄의 개수를 구한다.
		letterCount = row->GetLength();
		//9.5 현재글자 위치가 줄의 글자개수보다 작은동안 반복한다.
		while (letterIndex < letterCount)
		{
			//9.5.1 현재 글자를 구한다.
			letter = row->GetAt(letterIndex);
			//9.5.2 현재 글자의 내용을 구한다.
			content = CString(letter->GetContent().c_str());
			//9.5.3 글자의 너비를 구한다.
			//처음엔 0에서 시작해야하기 때문에 currentWidth += letterWidth가 
			//letterWidth = this->textExtent->GetTextWidth((string)content) 보다 앞에 있어야함.
			//그래서 현재 글자 너비를 구해서 다음 글자의 시작점으로 삼아주면 됨
			//첫글자의 시작점은 0이고 다음 글자의 시작점은 첫글자의 너비이다!!!
			currentWidth += letterWidth;
			letterWidth = notepadForm->textExtent->GetTextWidth((string)content);
			//만약에 글자가 탭문자이면 내용을 띄어쓰기 8개로 바꿔준다.
			if (content == "\t")
			{
				content = "        ";
			}
			//9.5.3. 현재 글자가 선택이 안되어있으면
			if (letter->IsSelected() == false)
			{
				//9.5.3.1 글자를 화면에 출력한다.
				dc.SetBkColor(RGB(255, 255, 255));
				//텍스트의 색깔을 정함. 이렇게해야 나중에 글꼴상자에서 색깔을 바꾸면 반영할 수 있음.
				dc.SetTextColor(notepadForm->font.GetColor());
				//dc.SetTextColor(RGB(0, 0, 0));//이렇게하면 나중에 글꼴에서 글자색을 바꿀수없음
				dc.TextOut(currentWidth - currentXPos, rowIndex * text.tmHeight
					- currentYPos, content);
			}
			//9.5.4 현재 글자가 선택이 되어있으면
			else
			{
				//9.5.4.1
				dc.SetBkColor(GetSysColor(COLOR_HIGHLIGHT));//red, green, blue 세개 색깔 
				dc.SetTextColor(RGB(255, 255, 255));//이렇게하면 나중에 글꼴에서 글자색을 바꿀수 없음
				dc.TextOut(currentWidth - currentXPos, rowIndex * text.tmHeight
					- currentYPos, content);
			}
			letterIndex++;
		}
		//9.6 텍스트 시작위치는 고정되어고 화면만 이동하므로 이동한만큼 빼줘야함!
		//그럼 원래 화면은 처음 시작점에 고정되어 있는데 -해줌으로써 화면이 움직이는 것처럼 보임.
		//dc.TextOut(0 - currentXPos, rowIndex * text.tmHeight - currentYPos, content);
		rowIndex++;
	}
	dc.SelectObject(oldFont);
	//font가 폰트공통대화상자에서 변경되었을때 기존 font를 지워야 새로 변경된 font로 적용할 수 있음. 
	font.DeleteObject();
}

void LetterTextOutVisitor::VisitRowTextOut(Glyph* row)
{

}

void LetterTextOutVisitor::VisitLetterTextOut(Glyph* letter)
{

}

//소멸자
LetterTextOutVisitor::~LetterTextOutVisitor()
{

}