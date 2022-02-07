#include "PreviewForm.h"
#include "NotepadForm.h"
#include "PrintInformation.h"
#include "PreviewVisitor.h"
#include "Glyph.h"

BEGIN_MESSAGE_MAP(PreviewForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()//더블버퍼링을 막기 위한 조치
	ON_WM_CLOSE()
END_MESSAGE_MAP()

//생성자
PreviewForm::PreviewForm(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
	this->originalRectWidth = 0;
	this->originalRectHeight = 0;
}

//메모장 윈도우가 생성될 때
int PreviewForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CFrameWnd::OnCreate(lpCreateStruct);

	// 미리보기 폼이 생성될 때 클라이언트영역의 크기를 구한다.
	CRect clientRect;
	this->GetClientRect(&clientRect);
	this->originalRectWidth = clientRect.Width();
	this->originalRectHeight = clientRect.Height();


	this->button.Create(_T("My button"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(100, 100, 200, 200), this, 1);

	return 0;
}

void PreviewForm::OnPaint()
{
	//1. CPaintDC를 생성한다.
	CPaintDC dc(this);
	//2. previewVisitor를 생성한다.
	PreviewVisitor previewVisitor = PreviewVisitor(this->notepadForm, &dc, 0, 0);
    //3. 미리보기를 출력한다.
    this->notepadForm->printInformation->GetPrintNote()->Accept(&previewVisitor);
}

BOOL PreviewForm::OnEraseBkgnd(CDC* pDC)
{
	return true;
}

//소멸자
void PreviewForm::OnClose()
{
	this->notepadForm->previewForm = 0;
	CFrameWnd::OnClose();
}