#include "PreviewForm.h"
#include "NotepadForm.h"
#include "PrintInformation.h"
#include "PreviewVisitor.h"
#include "Glyph.h"

BEGIN_MESSAGE_MAP(PreviewForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()//������۸��� ���� ���� ��ġ
	ON_WM_CLOSE()
END_MESSAGE_MAP()

//������
PreviewForm::PreviewForm(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
	this->originalRectWidth = 0;
	this->originalRectHeight = 0;
}

//�޸��� �����찡 ������ ��
int PreviewForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CFrameWnd::OnCreate(lpCreateStruct);

	// �̸����� ���� ������ �� Ŭ���̾�Ʈ������ ũ�⸦ ���Ѵ�.
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
	//1. CPaintDC�� �����Ѵ�.
	CPaintDC dc(this);
	//2. previewVisitor�� �����Ѵ�.
	PreviewVisitor previewVisitor = PreviewVisitor(this->notepadForm, &dc, 0, 0);
    //3. �̸����⸦ ����Ѵ�.
    this->notepadForm->printInformation->GetPrintNote()->Accept(&previewVisitor);
}

BOOL PreviewForm::OnEraseBkgnd(CDC* pDC)
{
	return true;
}

//�Ҹ���
void PreviewForm::OnClose()
{
	this->notepadForm->previewForm = 0;
	CFrameWnd::OnClose();
}