#include "PreviewForm.h"
#include "NotepadForm.h"
#include "PrintInformation.h"
#include "PreviewVisitor.h"
#include "Glyph.h"
#include "Note.h"
#include "PrintingVisitor.h"
#include "PageSetUpDialog.h"
#include "PageSetUpInformation.h"

BEGIN_MESSAGE_MAP(PreviewForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()//������۸��� ���� ���� ��ġ
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()//�ּһ�������ϱ�
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUSPAGE, OnPreviousPageButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_NEXTPAGE, OnNextPageButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_PAGESETUP, OnPageSetUpButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_PAGEPRINT, OnPagePrintButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_PAGECANCEL, OnPageCancelButtonClicked)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_COMMAND_PAGEUPDATE, OnCommand)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

//������
PreviewForm::PreviewForm(NotepadForm* notepadForm)
{
	//1. notepadForm�� �ּҸ� �����Ѵ�.
	this->notepadForm = notepadForm;
	//2. ���ο� ��Ʈ�� �����Ѵ�.
	this->note = new Note();
	//3. ����Ʈ�� ��Ʈ�� ���Ѵ�.
	Glyph* printNote = this->notepadForm->printInformation->GetPrintNote();
	//4. �� �������� �μ��� ���� ���� ���Ѵ�.
	Long pageRowCount = this->notepadForm->printInformation->GetPageRowCount();
	//5. ������������ �����Ѵ�.
	this->totalPageCount = printNote->GetLength() / pageRowCount;
	Long remainder = printNote->GetLength() % pageRowCount;
	if (remainder != 0)
	{
		this->totalPageCount++;
	}
	//6. �� �������� �μ��� ���� �������� �������� �׸��� ����Ʈ�� ��Ʈ�� ���� �������� �������� �ݺ��Ѵ�.
	Glyph* row = 0;
	Long rowIndex = 0;
	Long i = 0;
	while (i < pageRowCount && rowIndex < printNote->GetLength())
	{
		//6.1 ���� ���� ���Ѵ�.
		row = printNote->GetAt(rowIndex)->Clone();
		//6.2 ���� ���� previewForm ��Ʈ�� �߰��Ѵ�.
		this->note->Add(row);
		//6.3 ���� ��ġ�� ������Ų��.
		rowIndex++;
		i++;
	}
	//7. ���� ������ ��ȣ�� �����Ѵ�.
	this->currentPageNumber = 1;
}


BOOL PreviewForm::PreTranslateMessage(MSG* pMsg)
{
	//1. ����Ʈ��Ʈ���̸�
	if (pMsg->hwnd == this->currentPageNumberEdit.GetSafeHwnd())
	{
		//1.1 Ű���� �Է��� �Ǹ�
		if (pMsg->message == WM_KEYDOWN)
		{
			//1.1.1 Ű���� �Է��� ���๮���̸�
			if (pMsg->wParam == '\r' || pMsg->wParam == '\n')
			{
				//1.1.1.1 ����Ʈ ��Ʈ�ѿ� ���� �ִ� ���� ������ ���� �д´�.
				CString currentPageString;
				this->currentPageNumberEdit.GetWindowText(currentPageString);
				string currentPageStr(currentPageString);
				//1.1.1.2 ����Ʈ��Ʈ���� �����̳� 0�� �ƴϸ�
				if (currentPageStr.compare("") != 0 && currentPageStr.compare("0") != 0)
				{
					//1.1.1.2.1 ����Ʈ��Ʈ�ѿ��� ���� ���ڿ��ڷ����� ������ �ڷ������� �ٲ��ش�.
					Long currentPageNumber = _ttoi(currentPageString);
					//1.1.1.2.2 ����Ʈ��Ʈ�ѿ� ���� �ִ� ������ ���� �������������� �۰ų� ������
					if (currentPageNumber <= this->totalPageCount)
					{
						//1.1.1.2.2.1 �̸��������� ���� ������ ���� 
						//����Ʈ ��Ʈ�ѿ� ���� �ִ� ���� ������ ���� �ٲ۴�.
						this->currentPageNumber = currentPageNumber;
						//1.1.1.2.2.2 �������� ������Ʈ��Ų��.
						this->SendMessage(WM_COMMAND, ID_COMMAND_PAGEUPDATE);
						//1.1.1.2.2.3 ����Ʈ��Ʈ�ѿ� ��Ŀ���� ���ش�.
						this->currentPageNumberEdit.SendMessage(WM_KILLFOCUS);
						//1.1.1.2.2.4 �̸��������� ��Ŀ���� �����Ѵ�.
						this->SetFocus();
					}
				}	
			}
		}
		//1.2 ����, ���� �Ǵ� Ư�� ���ڰ� �ԷµǸ�
		else if (pMsg->message == WM_CHAR)
		{
			//1.2.1 �Էµ� �ؽ�Ʈ�� Ư�������̰ų� �����̸�
			if ((pMsg->wParam == 9) || (pMsg->wParam > 31 && pMsg->wParam < 48)
				|| (pMsg->wParam > 57 && pMsg->wParam < 128))
			{
				//1.2.1.1 �Է��� ���� �ʰ� �ٲ۴�.
				pMsg->wParam = 0;
			}	
		}
	}
	return CFrameWnd::PreTranslateMessage(pMsg);
}

//�޸��� �����찡 ������ ��
int PreviewForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CFrameWnd::OnCreate(lpCreateStruct);

	//1. ClientDC�� �����Ѵ�.
	CClientDC dc(this);
	//2. �۾�ũ��� �۾�ü�� ���ϴ�.
	this->controlFont.CreatePointFont(150, _T("�����ٸ���"), &dc);
	//3 ��Ʈ�� dc�� �����Ѵ�.
	dc.SelectObject(this->controlFont);
	//4. �̸����� ���� ������ �� ������ Ŭ���̾�Ʈ ���� ũ�⸦ ���Ѵ�.
	this->GetClientRect(&this->firstClientRect);
	//5. ���� ������ ��ư�� �����Ѵ�.
	double clientRectWidth = this->firstClientRect.Width();
	double previewWidth = clientRectWidth / 10 * 7;
	double printSetUpAreaWidth = clientRectWidth / 10 * 3;
	double centerOfprintSetUpAreaWidth = printSetUpAreaWidth / 2;
	this->previousPageButton.Create(_T("���� ������"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(previewWidth + centerOfprintSetUpAreaWidth - 100, 100, previewWidth 
			+ centerOfprintSetUpAreaWidth + 100, 150), this, IDC_BUTTON_PREVIOUSPAGE);
	//6. ���� ������ ��ư�� ����ũ��� �۲��� �����Ų��.
	this->previousPageButton.SetFont(&this->controlFont);
	//7. ���� ������ ��ư�� �����Ѵ�.
	this->nextPageButton.Create(_T("���� ������"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(previewWidth + centerOfprintSetUpAreaWidth - 100, 250, previewWidth
			+ centerOfprintSetUpAreaWidth + 100, 300), this, IDC_BUTTON_NEXTPAGE);
	//8. ���� ������ ��ư�� ����ũ��� �۲��� �����Ų��.
	this->nextPageButton.SetFont(&this->controlFont);
	//9. ���� ������ ��ȣ ����Ʈ ��Ʈ���� �����Ѵ�.
	this->currentPageNumberEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP
		| WS_BORDER | SS_CENTER,
		CRect(previewWidth + centerOfprintSetUpAreaWidth - 100, 180, previewWidth
			+ centerOfprintSetUpAreaWidth - 30, 220), this, IDC_EDIT_CURRENTPAGENUMBER);
	//10. ���� ������ ��ȣ ����Ʈ ��Ʈ���� ����ũ��� �۲��� �����Ų��.
	this->GetDlgItem(IDC_EDIT_CURRENTPAGENUMBER)->SetFont(&this->controlFont);
	//11. ���� ������ ��ȣ�� ����Ѵ�.
	this->currentPageNumberEdit.SetWindowText(to_string(this->currentPageNumber).c_str());
	//12. ������ ����ƽ ��Ʈ���� �����Ѵ�.
	this->slash.Create(_T("  /  "), WS_CHILD | WS_VISIBLE | SS_CENTER,
		CRect(previewWidth + centerOfprintSetUpAreaWidth - 30, 180, previewWidth
			+ centerOfprintSetUpAreaWidth + 30, 220), this, IDC_STATIC_SLASH);
	//13. ������ ����ƽ ��Ʈ���� ����ũ��� �۲��� �����Ѵ�.
	this->GetDlgItem(IDC_STATIC_SLASH)->SetFont(&this->controlFont);
	//14.  �� ������ �� ����ƽ ��Ʈ���� �����Ѵ�.
	this->totalPageCountStatic.Create(_T(to_string(this->totalPageCount).c_str()),
		WS_CHILD | WS_VISIBLE | SS_CENTER,
		CRect(previewWidth + centerOfprintSetUpAreaWidth + 30, 180, previewWidth
			+ centerOfprintSetUpAreaWidth + 100, 220), this, IDC_STATIC_TOTALPAGECOUNT);
	//15. �� ������ �� ����ƽ ��Ʈ���� ����ũ��� �۲��� �����Ѵ�.
	this->GetDlgItem(IDC_STATIC_TOTALPAGECOUNT)->SetFont(&this->controlFont);
	//16. ������ ������ư�� �����Ѵ�.
	this->pageSetUpButton.Create(_T("������ ����"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(previewWidth + centerOfprintSetUpAreaWidth - 100, 350, previewWidth
			+ centerOfprintSetUpAreaWidth + 100, 400), this, IDC_BUTTON_PAGESETUP);
	//17. ������ ���� ��ư�� ����ũ��� �۲��� �����Ų��.
	this->pageSetUpButton.SetFont(&this->controlFont);
	//18. �μ� ��ư�� �����Ѵ�.
	this->pagePrintButton.Create(_T("��    ��"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(previewWidth + centerOfprintSetUpAreaWidth - 100, 450, previewWidth
			+ centerOfprintSetUpAreaWidth + 100, 500), this, IDC_BUTTON_PAGEPRINT);
	//19. �μ� ��ư�� ����ũ��� �۲��� �����Ų��.
	this->pagePrintButton.SetFont(&this->controlFont);
	//20. ��� ��ư�� �����Ѵ�.
	this->pageCancelButton.Create(_T("��    ��"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(previewWidth + centerOfprintSetUpAreaWidth - 100, 550, previewWidth
			+ centerOfprintSetUpAreaWidth + 100, 600), this, IDC_BUTTON_PAGECANCEL);
	//21. ��� ��ư�� ����ũ��� �۲��� �����Ų��.
	this->pageCancelButton.SetFont(&this->controlFont);

	return 0;
}

void PreviewForm::OnPaint()
{
	//1. CPaintDC�� �����Ѵ�.
	CPaintDC dc(this);
	//2. previewVisitor�� �����Ѵ�.
	PreviewVisitor previewVisitor = PreviewVisitor(this->notepadForm, &dc, 0, 0);
    //3. �̸����⸦ ����Ѵ�.
    this->note->Accept(&previewVisitor);
}

//�̸����⿡�� ȭ���� ũ�Ⱑ ����� ��
void PreviewForm::OnSize(UINT nType, int cx, int cy)
{
	//1. ���� �޸����� â�� ����(�ּ�ȭ, �ִ�ȭ, ���� ũ��� ����)�� ���� ���̿� ���� ���̸� �Է¹޴´�.
	CFrameWnd::OnSize(nType, cx, cy);
	//2. ���� ȭ���� ũ�⸦ ���Ѵ�.
	CRect clientRect;
	this->notepadForm->previewForm->GetClientRect(&clientRect);
	//3. ���� �޸����� ���°� �ּ�ȭ�� �ƴϸ�(�ּ�ȭ�̸� cx�� cy ���� �Ѵ� 0�� �ǰ�, 
	//cx�� 0�̸� �Ʒ����� cxũ��� �ݺ��� �����µ� ���ѹݺ��� �߻��ؼ� �ּ�ȭ��ư�� ������ ���̳�!) 
	if (nType != SIZE_MINIMIZED)
	{
		//3.1 �ٲ� Ŭ���̾�Ʈ ������ ���α��̿� ���α��̸� �����ش�. 
		double changedRectWidth = clientRect.Width();
		double changedRectHeight = clientRect.Height();
		//3.2 ó�� Ŭ���̾�Ʈ ������ ���α��̿� ���α��̸� �����ش�.
		double firstRectWidth = this->firstClientRect.Width();
		double firstRectHeight = this->firstClientRect.Height();
		//3.3 �ٲ� Ŭ���̾�Ʈ ������ ũ��� ó�� Ŭ���̾�Ʈ ������ ũ���� ������ ���Ѵ�.
		double multiplicandWidth = changedRectWidth / firstRectWidth;
		double multiplicandHeight = changedRectHeight / firstRectHeight;
		double multiplicandPoint = multiplicandWidth + multiplicandHeight 
			/ (firstRectWidth + firstRectHeight);
		//3.4 controlFont�� ���ڸ� �����.
		this->controlFont.DeleteObject();
		//3.5 ClientDC�� �����Ѵ�.
		CClientDC dc(this);
		//3.6 �۾�ũ��� �۾�ü�� ���ϴ�.
		this->controlFont.CreatePointFont(150 * multiplicandPoint, _T("�����ٸ���"), &dc);
		//3.7  ��Ʈ�� dc�� �����Ѵ�.
		dc.SelectObject(this->controlFont);
		//3.8 ���� ������ ��ư�� �̵���Ų��.
		double clientRectWidth = changedRectWidth;
		double previewWidth = clientRectWidth / 10 * 7;
		double printSetUpAreaWidth = clientRectWidth / 10 * 3;
		double centerOfprintSetUpAreaWidth = printSetUpAreaWidth / 2;
		this->previousPageButton.MoveWindow(CRect(previewWidth + centerOfprintSetUpAreaWidth
			- 100 * multiplicandWidth, 100 * multiplicandHeight, previewWidth
			+ centerOfprintSetUpAreaWidth + 100 * multiplicandWidth, 150 * multiplicandHeight));
		//3.9 ���� ������ ��ư�� ����ũ��� �۲��� �����Ų��.
		this->previousPageButton.SetFont(&this->controlFont);
		//3.10 ���� ������ ��ư�� �̵���Ų��.
		this->nextPageButton.MoveWindow(CRect(previewWidth + centerOfprintSetUpAreaWidth
			- 100 * multiplicandWidth, 250 * multiplicandHeight, previewWidth
			+ centerOfprintSetUpAreaWidth + 100 * multiplicandWidth, 300 * multiplicandHeight));
		//3.11 ���� ������ ��ư�� ����ũ��� �۲��� �����Ų��.
		this->nextPageButton.SetFont(&this->controlFont);
		//3.12  ���� ������ ��ȣ ����Ʈ ��Ʈ���� �̵���Ų��.
		this->currentPageNumberEdit.MoveWindow(CRect(previewWidth + centerOfprintSetUpAreaWidth
			- 100 * multiplicandWidth, 180 * multiplicandHeight, previewWidth + centerOfprintSetUpAreaWidth
			- 30 * multiplicandWidth, 220 * multiplicandHeight));
		//3.13 ���� ������ ��ȣ ����Ʈ ��Ʈ���� ����ũ��� �۲��� �����Ų��.
		this->GetDlgItem(IDC_EDIT_CURRENTPAGENUMBER)->SetFont(&this->controlFont);
		//3.14 ������ ����ƽ ��Ʈ���� �̵���Ų��.
		this->slash.MoveWindow(CRect(previewWidth + centerOfprintSetUpAreaWidth 
			- 30 * multiplicandWidth, 180 * multiplicandHeight, previewWidth + 
			centerOfprintSetUpAreaWidth + 30 * multiplicandWidth, 220 * multiplicandHeight));
		//3.15 ������ ����ƽ ��Ʈ���� ����ũ��� �۲��� �����Ѵ�.
		this->GetDlgItem(IDC_STATIC_SLASH)->SetFont(&this->controlFont);
		//3.16 �� ������ �� ����ƽ ��Ʈ���� �̵���Ų��.
		this->totalPageCountStatic.MoveWindow(CRect(previewWidth + centerOfprintSetUpAreaWidth 
			+ 30 * multiplicandWidth, 180 * multiplicandHeight, previewWidth 
			+ centerOfprintSetUpAreaWidth + 100 * multiplicandWidth, 220 * multiplicandHeight));
		//3.17 �� ������ �� ����ƽ ��Ʈ���� ����ũ��� �۲��� �����Ѵ�.
		this->GetDlgItem(IDC_STATIC_TOTALPAGECOUNT)->SetFont(&this->controlFont);
		//3.18 ������ ���� ��ư�� �̵���Ų��.
		this->pageSetUpButton.MoveWindow(CRect(previewWidth + centerOfprintSetUpAreaWidth
			- 100 * multiplicandWidth, 350 * multiplicandHeight, previewWidth
			+ centerOfprintSetUpAreaWidth + 100 * multiplicandWidth, 400 * multiplicandHeight));
		//3.19 ������ ���� ��ư�� ����ũ��� �۲��� �����Ų��.
		this->pageSetUpButton.SetFont(&this->controlFont);
		//3.20 �μ� ��ư�� �̵���Ų��.
		this->pagePrintButton.MoveWindow(CRect(previewWidth + centerOfprintSetUpAreaWidth
			- 100 * multiplicandWidth, 450 * multiplicandHeight, previewWidth
			+ centerOfprintSetUpAreaWidth + 100 * multiplicandWidth, 500 * multiplicandHeight));
		//3.21 �μ� ��ư�� ����ũ��� �۲��� �����Ų��.
		this->pagePrintButton.SetFont(&this->controlFont);
		//3.22 ��� ��ư�� �̵���Ų��.
		this->pageCancelButton.MoveWindow(CRect(previewWidth + centerOfprintSetUpAreaWidth
			- 100 * multiplicandWidth, 550 * multiplicandHeight, previewWidth
			+ centerOfprintSetUpAreaWidth + 100 * multiplicandWidth, 600 * multiplicandHeight));
		//3.23 �μ��ϱ� ��ư�� ����ũ��� �۲��� �����Ų��.
		this->pageCancelButton.SetFont(&this->controlFont);
	}
	//4. ��������� �����Ѵ�.
	this->Invalidate(TRUE);
}

//�ּһ�������ϱ�
void PreviewForm::OnGetMinMaxInfo(MINMAXINFO* lpMinMaxInfo)
{
	lpMinMaxInfo->ptMinTrackSize.x = 600;
	lpMinMaxInfo->ptMinTrackSize.y = 300;
}

//������۸��� ���� ���� ��ġ
BOOL PreviewForm::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

//����ƽ ��Ʈ���� �⺻ ������ ȸ���̶� ������ �����ϰ� �ٲٱ� ���� ��ġ
HBRUSH PreviewForm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	
	HBRUSH hbr = CFrameWnd::OnCtlColor(pDC, pWnd, nCtlColor);

	const UINT nID = pWnd->GetDlgCtrlID();
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT); /// ����� �����ϰ�
		hbr = (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	return hbr;
}

//���������� ��ư�� Ŭ������ ��
void PreviewForm::OnPreviousPageButtonClicked()
{
	//1. ���� ������ ��ȣ�� 1���� ũ��
	if (this->currentPageNumber > 1)
	{
		//1.1 ���� �������� ���ҽ�Ų��.
		this->currentPageNumber--;
		//1.2 ���� ������ ��ȣ�� ����Ѵ�.
		this->currentPageNumberEdit.SetWindowText(to_string(this->currentPageNumber).c_str());
		//1.3 �������� ������Ʈ��Ų��.
		this->SendMessage(WM_COMMAND, ID_COMMAND_PAGEUPDATE);
	}
	//2. ���� ������ ��ȣ�� 1�̸�
	else
	{
		//2.1 ���� ������ ��ȣ�� ����Ѵ�.
		this->currentPageNumberEdit.SetWindowText(to_string(this->currentPageNumber).c_str());
	}
	//3. ���������� ��ư�� ��Ŀ���� ���ش�.
	this->previousPageButton.SendMessage(WM_KILLFOCUS);
	//4. �̸��������� ��Ŀ���� �����Ѵ�.
	this->SetFocus();
}

//���������� ��ư�� Ŭ������ ��
void PreviewForm::OnNextPageButtonClicked()
{
	//1. ���� ������ ��ȣ�� �������������� ������
	if (this->currentPageNumber < this->totalPageCount)
	{
		//1.1 ���� ������ ��ȣ�� ������Ų��.
		this->currentPageNumber++;
		//1.2 ���� ������ ��ȣ�� ����Ѵ�.
		this->currentPageNumberEdit.SetWindowText(to_string(this->currentPageNumber).c_str());
		//1.3 �������� ������Ʈ��Ų��.
		this->SendMessage(WM_COMMAND, ID_COMMAND_PAGEUPDATE);
	}
	//2. ���� ������ ��ȣ�� ������������ ������
	else
	{
		//2.1 ���� ������ ��ȣ�� ����Ѵ�.
		this->currentPageNumberEdit.SetWindowText(to_string(this->currentPageNumber).c_str());
	}
	//3. ���������� ��ư�� ��Ŀ���� ���ش�.
	this->nextPageButton.SendMessage(WM_KILLFOCUS);
	//4. �̸����� ���� ��Ŀ���� �����Ѵ�.
	this->SetFocus();
}

//������ ���� ��ư�� Ŭ������ ��
void PreviewForm::OnPageSetUpButtonClicked()
{
	//1. pageSetUpDialog�� �����Ѵ�.
	PageSetupDialog pageSetupDialog(this);
	//2. pageSetUpDialog�� ȭ�鿡 ����Ѵ�.
	LONG id;
	id = pageSetupDialog.DoModal();
	//3. OK��ư�� ��������
	if (id == IDOK)
	{
		//3.1 ������ ���� ���� ��ȭ���ڿ��� devmode����ü�� ���Ѵ�.
		DEVMODE* devmode = pageSetupDialog.GetDevMode();
		//3.2 ������ ����Ʈ ������ ������
		if (this->notepadForm->printInformation != 0)
		{
			//3.2.1 ������ �ִ� ����Ʈ ������ ���ش�.
			delete this->notepadForm->printInformation;
		}
		//3.3 ������ ���� ��ȭ���ڿ��� hdc�� ���Ѵ�.
		HDC hdc = pageSetupDialog.CreatePrinterDC();
		ASSERT(hdc);
		//3.4 hdc���� cdc�� ���Ѵ�.
		CDC* cdc = CDC::FromHandle(hdc);
		//3.5 ����Ʈ ������ ������ ���Ѵ�.
		CRect rect(0, 0, cdc->GetDeviceCaps(HORZRES), cdc->GetDeviceCaps(VERTRES));
		//3.6 ���������� ��ȭ���ڿ��� ������ ���Ѵ�.
		CRect marginRect;
		pageSetupDialog.GetMargins(&marginRect, NULL);
		//3.7 ���� ������ ������ ȭ�� ������ �°� �ٽ� �����Ѵ�.
		Long changedMarginRectLeft = marginRect.left * 600 / 2540;
		Long changedMarginRectTop = marginRect.top * 600 / 2540;
		Long changedMarginRectRight = marginRect.right * 600 / 2540;
		Long changedMarginRectBottom = marginRect.bottom * 600 / 2540;
		CRect changedMarginRect(changedMarginRectLeft, changedMarginRectTop,
			changedMarginRectRight, changedMarginRectBottom);
		//3.8 ������ �����ϰ� ����Ʈ ������ ������ ���Ѵ�.
		CRect printableRect(changedMarginRect.left, changedMarginRect.top,
			rect.Width() - changedMarginRect.right, rect.Height() - changedMarginRect.bottom);
		//3.9 ����Ʈ ������ �۲��� ������ �����ش�.
		LOGFONT printLogFont = this->notepadForm->font.GetLogFont();
		printLogFont.lfHeight = -MulDiv(this->notepadForm->font.GetSize() / 10, 600, 72);
		CFont font;
		HFONT oldFont;
		font.CreateFontIndirect(&printLogFont);
		oldFont = (HFONT)cdc->SelectObject(font);
		//3.10 ���� ������ ���� ������ ������
		if (this->notepadForm->pageSetUpInformation != 0)
		{
			//3.10.1 ���� ������ ���� ������ ���ش�.
			delete this->notepadForm->pageSetUpInformation;
			this->notepadForm->pageSetUpInformation = 0;
		}
		//3.11 ������ ������ ����Ʈ ������ ����, ����ũ��, �Ӹ���, �ٴڱ�, ���������� ������ ��������
		//���ο� ������ ���� ������ �����.
		this->notepadForm->pageSetUpInformation = new PageSetUpInformation(printableRect,
			devmode->dmPaperSize, pageSetupDialog.GetHeader(),
			pageSetupDialog.GetFooter(), devmode->dmOrientation);
		//3.12 ����Ʈ������ ������ Ŭ������ �����Ѵ�.
		this->notepadForm->printInformation = new PrintInformation(this->notepadForm,
			printLogFont, hdc, this->notepadForm->pageSetUpInformation->GetPrintableRect());
		//3.13 ������ note�� ������
		if (this->note != 0)
		{
			//3.13.1 ���� note�� �����.
			delete this->note;
		}
		//3.14 ���ο� ������ ������ ���ο� ��Ʈ�� �����Ѵ�.
		this->note = new Note();
		//3.15 ����Ʈ�� ��Ʈ�� ���Ѵ�.
		Glyph* printNote = this->notepadForm->printInformation->GetPrintNote();
		//3.16 �� �������� �μ��� ���� ���� ���Ѵ�.
		Long pageRowCount = this->notepadForm->printInformation->GetPageRowCount();
		//5. ������������ �����Ѵ�.
		this->totalPageCount = printNote->GetLength() / pageRowCount;
		Long remainder = printNote->GetLength() % pageRowCount;
		if (remainder != 0)
		{
			this->totalPageCount++;
		}
		//3.18 �� �������� �μ��� ���� �������� �������� �׸��� ����Ʈ�� ��Ʈ�� ���� �������� �������� �ݺ��Ѵ�.
		Glyph* row = 0;
		Long rowIndex = 0;
		Long i = 0;
		while (i < pageRowCount && rowIndex < printNote->GetLength())
		{
			//3.18.1 ���� ���� ���Ѵ�.
			row = printNote->GetAt(rowIndex)->Clone();
			//3.18.2 ���� ���� previewForm ��Ʈ�� �߰��Ѵ�.
			this->note->Add(row);
			//3.18.3 ���� ��ġ�� ������Ų��.
			rowIndex++;
			i++;
		}
		//3.19 ���� ������ ��ȣ�� �����Ѵ�.
		this->currentPageNumber = 1;
		//3.20 ���� ������ ��ȣ�� ����Ʈ ��Ʈ�ѿ� ����Ѵ�.
		this->currentPageNumberEdit.SetWindowText(to_string(this->currentPageNumber).c_str());
		//3.21 �� ������ ���� ����ƽ ��Ʈ�ѿ� ����Ѵ�.
		this->totalPageCountStatic.SetWindowText(_T(to_string(this->totalPageCount).c_str()));
		//3.22 ��������� �����Ѵ�.
		this->Invalidate(TRUE);
	}
	GlobalFree(pageSetupDialog.m_psd.hDevMode);
	GlobalFree(pageSetupDialog.m_psd.hDevNames);
	//4. ���������� ��ư�� ��Ŀ���� ���ش�.
	this->nextPageButton.SendMessage(WM_KILLFOCUS);
	//5. �̸����� ���� ��Ŀ���� �����Ѵ�.
	this->SetFocus();
}

//�μ� ��ư�� Ŭ������ ��
void PreviewForm::OnPagePrintButtonClicked()
{
	//1. ����Ʈ ���̾�α׸� �����Ѵ�.
	CPrintDialog dlg(FALSE, PD_ALLPAGES | PD_COLLATE | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE);
	//2. ����Ʈ ���̾�α׿��� OK��ư�� ��������
	if (dlg.DoModal() == IDOK)
	{
		//2.1 ������ printInformation�� ������
		if (this->notepadForm->printInformation != 0)
		{
			//2.1.1 ������ �ִ� printInformation�� �Ҵ������Ѵ�.
			delete this->notepadForm->printInformation;
		}
		//2.2 ����Ʈ ���̾�α׿��� hdc�� ���Ѵ�.
		HDC hdc = dlg.CreatePrinterDC();
		ASSERT(hdc);
		//2.3 ����Ʈ ���̾�α��� hdc���� cdc�� ���Ѵ�.
		CDC* cdc = CDC::FromHandle(hdc);
		//2.4 ����Ʈ ���̾�α׿��� devmode�� ���Ѵ�.
		DEVMODE* devmode = dlg.GetDevMode();
		 //2.5 ������ ���� ������ ������
        if (this->notepadForm->pageSetUpInformation != 0)
        {
            //2.5.1 ������ �������� ������ �������� ������ devmode�� �����Ѵ�.
            devmode->dmOrientation = this->notepadForm->pageSetUpInformation->GetOrientation();
            //2.5.2 ������ ���� �������� ���� ����ũ�� ������ devmode�� �����Ѵ�.
            devmode->dmPaperSize = this->notepadForm->pageSetUpInformation->GetPaperSize();
        }
        //2.6 ������ ���� ������ ������
        else
        {
            //2.6.1 ���� ������ ����Ʈ�� �����Ѵ�.
            devmode->dmOrientation = 1;
            //2.6.2 A4����ũ�⸦ devmode�� �����Ѵ�.
            devmode->dmPaperSize = 9;
        }
		//2.7 devmode�� ������ �ݿ��ؼ� cdc�� reste(upadate)���ش�.
		cdc->ResetDCA(devmode);
		//2.8 ����Ʈ�� ���� ������ ���� ������ �����Ѵ�.
		DOCINFO docinfo;
		//2.9 ����Ʈ�� ���� ������ ���� ������ �ʱ�ȭ�����ش�.
		ZeroMemory(&docinfo, sizeof(DOCINFO));
		//2.10 ����Ʈ�� ���� ������ �̸��� ���Ѵ�.
		docinfo.lpszDocName = "�޸���";
		//2.11 ����Ʈ�� �����Ѵ�.
		cdc->StartDoc(&docinfo);
		//2.12 PrintingVisitor�� �����Ѵ�.
		PrintingVisitor printingVisitor = PrintingVisitor(this->notepadForm, cdc, 0, 0);
		//2.13 ����Ʈ ������ �۲��� ������ �����ش�.
		LOGFONT printLogFont = this->notepadForm->font.GetLogFont();
		printLogFont.lfHeight = -MulDiv(this->notepadForm->font.GetSize() / 10, 600, 72);
		CFont font;
		HFONT oldFont;
		font.CreateFontIndirect(&printLogFont);
		oldFont = (HFONT)cdc->SelectObject(font);
		//2.14 ������ ���� ������ ������
		if (this->notepadForm->pageSetUpInformation != 0)
		{
			//2.14.1 ������ �������� ������ ����Ʈ�� ������ ������ �ݿ��� ����Ʈ������ ������ Ŭ������ �����Ѵ�.
			this->notepadForm->printInformation = new PrintInformation(this->notepadForm,
				printLogFont, hdc, this->notepadForm->pageSetUpInformation->GetPrintableRect());
		}
		//2.15 ������ ���� ������ ������
		else
		{
			//2.15.1 �⺻���� ����Ʈ�� ������ ������ �ݿ��� ����Ʈ������ ������ Ŭ������ �����Ѵ�.
			this->notepadForm->printInformation = new PrintInformation(this->notepadForm,
				printLogFont, hdc,
				CRect(0, 0, cdc->GetDeviceCaps(HORZRES), cdc->GetDeviceCaps(VERTRES)));
		}
		//2.16 ����Ʈ ����� �Ѵ�.
		this->notepadForm->printInformation->GetPrintNote()->Accept(&printingVisitor);
		//2.17 ����Ʈ�� ������.
		cdc->EndDoc();
	}
	//3. ���������� ��ư�� ��Ŀ���� ���ش�.
	this->nextPageButton.SendMessage(WM_KILLFOCUS);
	//4. �̸����� ���� ��Ŀ���� �����Ѵ�.
	this->SetFocus();
}

//��� ��ư�� Ŭ������ ��
void PreviewForm::OnPageCancelButtonClicked()
{
	this->PostMessage(WM_CLOSE);
}

//����Ű��ư�� Ŭ������ ��
void PreviewForm::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ������ ����Ű �Ǵ� �Ʒ� ����Ű�� ��������
	if (nChar == VK_RIGHT || nChar == VK_DOWN)
	{
		//1.1 ���������� ��ưƲ Ŭ������ ���� �޼����� ������.
		this->SendMessage(WM_COMMAND, IDC_BUTTON_NEXTPAGE);
	}
	//2. ���� ����Ű �Ǵ� ���� ����Ű�� ��������
	else if (nChar == VK_LEFT || nChar == VK_UP)
	{
		//2.1 ���������� ��ưƲ Ŭ������ ���� �޼����� ������.
		this->SendMessage(WM_COMMAND, IDC_BUTTON_PREVIOUSPAGE);
	}
}

//Command����
void PreviewForm::OnCommand()
{
	//1. �̸��������� ��Ʈ�� �Ҵ������Ѵ�.
	if (this->note != 0)
	{
		delete this->note;
	}
	//2. �̸��������� ��Ʈ�� ���� �����Ѵ�.
	this->note = new Note();
	//3. ����Ʈ�� ��Ʈ�� ���Ѵ�.
	Glyph* printNote = this->notepadForm->printInformation->GetPrintNote();
	//4. �� �������� �μ��� ���� ���� ���Ѵ�.
	Long pageRowCount = this->notepadForm->printInformation->GetPageRowCount();
	//5. �� �������� �μ��� ���� �������� �������� �׸���
	//����Ʈ�� ��Ʈ�� ���� �������� �������� �ݺ��Ѵ�.
	Glyph* row = 0;
	Long rowIndex = pageRowCount * this->currentPageNumber - pageRowCount;
	Long i = 0;
	while (i < pageRowCount && rowIndex < printNote->GetLength())
	{
		//5.1 ���� ���� ���Ѵ�.
		row = printNote->GetAt(rowIndex)->Clone();
		//5.2 ���� ���� previewForm ��Ʈ�� �߰��Ѵ�.
		this->note->Add(row);
		//5.3 ���� ��ġ�� ������Ų��.
		rowIndex++;
		i++;
	}
	//6. ��������� �����Ѵ�.
	this->Invalidate(TRUE);
}

//�Ҹ���
void PreviewForm::OnClose()
{
	this->notepadForm->previewForm = 0;
	if (this->note != 0)
	{
		delete this->note;
	}
	CFrameWnd::OnClose();
}