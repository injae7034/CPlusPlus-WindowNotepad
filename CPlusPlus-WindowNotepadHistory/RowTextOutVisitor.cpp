#include "RowTextOutVisitor.h"
#include "NotepadForm.h"
#include "Glyph.h"

//디폴트생성자
RowTextOutVisitor::RowTextOutVisitor(NotepadForm* notepadForm)
	:TextOutVisitor(notepadForm)
{

}

void RowTextOutVisitor::VisitNoteTextOut(Glyph *note)
{
	//1. CPaintDC를 생성한다.
	CPaintDC dc(notepadForm);
	//2. 텍스트의 색깔을 정함.
	dc.SetTextColor(notepadForm->font.GetColor());
	//3. 왼쪽을 기준선으로 정함.
	dc.SetTextAlign(TA_LEFT);
	//4. CFont를 생성한다.
	CFont font;
	//5. 글씨크기와 글씨체를 정하다.
	font.CreateFontIndirect(&notepadForm->font.GetLogFont());
	//6. 폰트를 dc에 지정한다.
	HFONT oldFont;
	oldFont = (HFONT)dc.SelectObject(font);
	//7. TEXTMETRIC을 생성한다.
	TEXTMETRIC text;
	//8. 글꼴의 정보를 얻는다.
	dc.GetTextMetrics(&text);
	//9. note에 저장된 글자들을 출력한다.
	Long i = 0;
	Long currentXPos;
	Long currentYPos;
	CString content;
	//10. 줄단위의 반복구조를 통해서 줄을 나눠서 줄개수만큼 출력하도록 함.
	while (i < notepadForm->note->GetLength())
	{
		//10.1 현재 줄의 글자들을 구한다.
		content = CString(notepadForm->note->GetAt(i)->GetContent().c_str());
		//10.2 스크롤의 위치를 구한다.
		currentXPos = notepadForm->GetScrollPos(SB_HORZ);
		currentYPos = notepadForm->GetScrollPos(SB_VERT);
		//10.3 텍스트 시작 위치설정 처음줄은 (0,0)에서 시작하고 두번째줄은 (0, 글자평균높이)에서 시작함.
		// 텍스트 시작위치는 고정되어고 화면만 이동하므로 이동한만큼 빼줘야함!
		//그럼 원래 화면은 처음 시작점에 고정되어 있는데 -해줌으로써 화면이 움직이는 것처럼 보임.
		dc.TextOut(0 - currentXPos, i * text.tmHeight - currentYPos, content);
		//dc.TextOut(0, i * text.tmHeight, content);
		//this->Notify(); 여기서 Notify 해주면 안됨! 캐럿이 계속 남게되고 안사라짐.
		i++;
	}
	dc.SelectObject(oldFont);
	//font가 폰트공통대화상자에서 변경되었을때 기존 font를 지워야 새로 변경된 font로 적용할 수 있음. 
	font.DeleteObject();
}

void RowTextOutVisitor::VisitRowTextOut(Glyph* row)
{

}

void RowTextOutVisitor::VisitLetterTextOut(Glyph* letter)
{

}

//소멸자
RowTextOutVisitor::~RowTextOutVisitor()
{

}