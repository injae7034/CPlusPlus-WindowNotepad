#include "PreviewForm.h"
#include "NotepadForm.h"
#include "PrintInformation.h"
#include "PreviewVisitor.h"
#include "Glyph.h"
#include "Note.h"
#include "PrintingVisitor.h"


BEGIN_MESSAGE_MAP(PreviewForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()//더블버퍼링을 막기 위한 조치
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()//최소사이즈설정하기
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUSPAGE, OnPreviousPageButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_NEXTPAGE, OnNextPageButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_PAGESETUP, OnPageSetUpButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_PAGEPRINT, OnPagePrintButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_PAGECANCEL, OnPageCancelButtonClicked)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_COMMAND_PAGEUPDATE, OnCommand)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

//생성자
PreviewForm::PreviewForm(NotepadForm* notepadForm)
{
	//1. notepadForm의 주소를 저장한다.
	this->notepadForm = notepadForm;
	//2. 새로운 노트를 생성한다.
	this->note = new Note();
	//3. 프린트할 노트를 구한다.
	Glyph* printNote = this->notepadForm->printInformation->GetPrintNote();
	//4. 한 페이지당 인쇄할 줄의 수를 구한다.
	Long pageRowCount = this->notepadForm->printInformation->GetPageRowCount();
	//5. 총페이지수를 설정한다.
	this->totalPageCount = printNote->GetLength() / pageRowCount + 1;//버림으로 인해 +1해준다.
	//6. 한 페이지당 인쇄할 줄의 개수보다 작은동안 그리고 프린트할 노트의 줄의 개수보다 작은동안 반복한다.
	Glyph* row = 0;
	Long rowIndex = 0;
	Long i = 0;
	while (i < pageRowCount && rowIndex < printNote->GetLength())
	{
		//6.1 현재 줄을 구한다.
		row = printNote->GetAt(rowIndex)->Clone();
		//6.2 현재 줄을 previewForm 노트에 추가한다.
		this->note->Add(row);
		//6.3 줄의 위치를 증가시킨다.
		rowIndex++;
		i++;
	}
	//7. 현재 페이지 번호를 설정한다.
	this->currentPageNumber = 1;
}


BOOL PreviewForm::PreTranslateMessage(MSG* pMsg)
{
	//1. 에디트컨트롤이면
	if (pMsg->hwnd == this->currentPageNumberEdit.GetSafeHwnd())
	{
		//1.1 키보드 입력이 되면
		if (pMsg->message == WM_KEYDOWN)
		{
			//1.1.1 키보드 입력이 개행문자이면
			if (pMsg->wParam == '\r' || pMsg->wParam == '\n')
			{
				//1.1.1.1 에디트 컨트롤에 적혀 있는 현재 페이지 수를 읽는다.
				CString currentPageString;
				this->currentPageNumberEdit.GetWindowText(currentPageString);
				string currentPageStr(currentPageString);
				//1.1.1.2 에디트컨트롤이 공란이나 0이 아니면
				if (currentPageStr.compare("") != 0 && currentPageStr.compare("0") != 0)
				{
					//1.1.1.2.1 에디트컨트롤에서 읽은 문자열자료형을 정수형 자료형으로 바꿔준다.
					Long currentPageNumber = _ttoi(currentPageString);
					//1.1.1.2.2 에디트건트롤에 적혀 있는 페이지 수가 총페이지수보다 작거나 같으면
					if (currentPageNumber <= this->totalPageCount)
					{
						//1.1.1.2.2.1 미리보기폼의 현재 페이지 수를 
						//에디트 컨트롤에 적혀 있는 현재 페이지 수로 바꾼다.
						this->currentPageNumber = currentPageNumber;
						//1.1.1.2.2.2 페이지를 업데이트시킨다.
						this->SendMessage(WM_COMMAND, ID_COMMAND_PAGEUPDATE);
						//1.1.1.2.2.3 에디트컨트롤에 포커스를 없앤다.
						this->currentPageNumberEdit.SendMessage(WM_KILLFOCUS);
						//1.1.1.2.2.4 미리보기폼에 포커스를 설정한다.
						this->SetFocus();
					}
				}	
			}
		}
		//1.2 영문, 숫자 또는 특수 문자가 입력되면
		else if (pMsg->message == WM_CHAR)
		{
			//1.2.1 입력된 텍스트가 특수문자이거나 영문이면
			if ((pMsg->wParam == 9) || (pMsg->wParam > 31 && pMsg->wParam < 48)
				|| (pMsg->wParam > 57 && pMsg->wParam < 128))
			{
				//1.2.1.1 입력이 되지 않게 바꾼다.
				pMsg->wParam = 0;
			}	
		}
	}
	return CFrameWnd::PreTranslateMessage(pMsg);
}

//메모장 윈도우가 생성될 때
int PreviewForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CFrameWnd::OnCreate(lpCreateStruct);

	//1. ClientDC를 생성한다.
	CClientDC dc(this);
	//2. 글씨크기와 글씨체를 정하다.
	this->controlFont.CreatePointFont(150, _T("나눔바른펜"), &dc);
	//3 폰트를 dc에 지정한다.
	dc.SelectObject(this->controlFont);
	//4. 미리보기 폼이 생성될 때 최초의 클라이언트 영역 크기를 구한다.
	this->GetClientRect(&this->firstClientRect);
	//5. 이전 페이지 버튼을 생성한다.
	double clientRectWidth = this->firstClientRect.Width();
	double previewWidth = clientRectWidth / 10 * 7;
	double printSetUpAreaWidth = clientRectWidth / 10 * 3;
	double centerOfprintSetUpAreaWidth = printSetUpAreaWidth / 2;
	this->previousPageButton.Create(_T("이전 페이지"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(previewWidth + centerOfprintSetUpAreaWidth - 100, 100, previewWidth 
			+ centerOfprintSetUpAreaWidth + 100, 150), this, IDC_BUTTON_PREVIOUSPAGE);
	//6. 이전 페이지 버튼의 글자크기와 글꼴을 변경시킨다.
	this->previousPageButton.SetFont(&this->controlFont);
	//7. 다음 페이지 버튼을 생성한다.
	this->nextPageButton.Create(_T("다음 페이지"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(previewWidth + centerOfprintSetUpAreaWidth - 100, 250, previewWidth
			+ centerOfprintSetUpAreaWidth + 100, 300), this, IDC_BUTTON_NEXTPAGE);
	//8. 다음 페이지 버튼의 글자크기와 글꼴을 변경시킨다.
	this->nextPageButton.SetFont(&this->controlFont);
	//9. 현재 페이지 번호 에디트 컨트롤을 생성한다.
	this->currentPageNumberEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP
		| WS_BORDER | SS_CENTER,
		CRect(previewWidth + centerOfprintSetUpAreaWidth - 100, 180, previewWidth
			+ centerOfprintSetUpAreaWidth - 30, 220), this, IDC_EDIT_CURRENTPAGENUMBER);
	//10. 현재 페이지 번호 에디트 컨트롤의 글자크기와 글꼴을 변경시킨다.
	this->GetDlgItem(IDC_EDIT_CURRENTPAGENUMBER)->SetFont(&this->controlFont);
	//11. 현재 페이지 번호를 출력한다.
	this->currentPageNumberEdit.SetWindowText(to_string(this->currentPageNumber).c_str());
	//12. 슬래쉬 스태틱 컨트롤을 설정한다.
	this->slash.Create(_T("  /  "), WS_CHILD | WS_VISIBLE | SS_CENTER,
		CRect(previewWidth + centerOfprintSetUpAreaWidth - 30, 180, previewWidth
			+ centerOfprintSetUpAreaWidth + 30, 220), this, IDC_STATIC_SLASH);
	//13. 슬래쉬 스태틱 컨트롤의 글자크기와 글꼴을 변경한다.
	this->GetDlgItem(IDC_STATIC_SLASH)->SetFont(&this->controlFont);
	//14.  총 페이지 수 스태틱 컨트롤을 생성한다.
	this->totalPageCountStatic.Create(_T(to_string(this->totalPageCount).c_str()),
		WS_CHILD | WS_VISIBLE | SS_CENTER,
		CRect(previewWidth + centerOfprintSetUpAreaWidth + 30, 180, previewWidth
			+ centerOfprintSetUpAreaWidth + 100, 220), this, IDC_STATIC_TOTALPAGECOUNT);
	//15. 총 페이지 수 스태틱 컨트롤의 글자크기와 글꼴을 변경한다.
	this->GetDlgItem(IDC_STATIC_TOTALPAGECOUNT)->SetFont(&this->controlFont);
	//16. 페이지 설정버튼을 생성한다.
	this->pageSetUpButton.Create(_T("페이지 설정"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(previewWidth + centerOfprintSetUpAreaWidth - 100, 350, previewWidth
			+ centerOfprintSetUpAreaWidth + 100, 400), this, IDC_BUTTON_PAGESETUP);
	//17. 페이지 설정 버튼의 글자크기와 글꼴을 변경시킨다.
	this->pageSetUpButton.SetFont(&this->controlFont);
	//18. 인쇄 버튼을 생성한다.
	this->pagePrintButton.Create(_T("인    쇄"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(previewWidth + centerOfprintSetUpAreaWidth - 100, 450, previewWidth
			+ centerOfprintSetUpAreaWidth + 100, 500), this, IDC_BUTTON_PAGEPRINT);
	//19. 인쇄 버튼의 글자크기와 글꼴을 변경시킨다.
	this->pagePrintButton.SetFont(&this->controlFont);
	//20. 취소 버튼을 생성한다.
	this->pageCancelButton.Create(_T("취    소"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(previewWidth + centerOfprintSetUpAreaWidth - 100, 550, previewWidth
			+ centerOfprintSetUpAreaWidth + 100, 600), this, IDC_BUTTON_PAGECANCEL);
	//21. 취소 버튼의 글자크기와 글꼴을 변경시킨다.
	this->pageCancelButton.SetFont(&this->controlFont);

	return 0;
}

void PreviewForm::OnPaint()
{
	//1. CPaintDC를 생성한다.
	CPaintDC dc(this);
	//2. previewVisitor를 생성한다.
	PreviewVisitor previewVisitor = PreviewVisitor(this->notepadForm, &dc, 0, 0);
    //3. 미리보기를 출력한다.
    this->note->Accept(&previewVisitor);
}

//미리보기에서 화면의 크기가 변경될 때
void PreviewForm::OnSize(UINT nType, int cx, int cy)
{
	//1. 현재 메모장의 창의 상태(최소화, 최대화, 이전 크기로 복원)와 가로 길이와 세로 길이를 입력받는다.
	CFrameWnd::OnSize(nType, cx, cy);
	//2. 현재 화면의 크기를 구한다.
	CRect clientRect;
	this->notepadForm->previewForm->GetClientRect(&clientRect);
	//3. 현재 메모장의 상태가 최소화가 아니면(최소화이면 cx와 cy 값이 둘다 0이 되고, 
	//cx가 0이면 아래에서 cx크기로 반복을 돌리는데 무한반복이 발생해서 최소화버튼을 누르면 뻑이남!) 
	if (nType != SIZE_MINIMIZED)
	{
		//3.1 바뀐 클라이언트 영역의 가로길이와 세로길이를 구해준다. 
		double changedRectWidth = clientRect.Width();
		double changedRectHeight = clientRect.Height();
		//3.2 처음 클라이언트 영역의 가로길이와 세로길이를 구해준다.
		double firstRectWidth = this->firstClientRect.Width();
		double firstRectHeight = this->firstClientRect.Height();
		//3.3 바뀐 클라이언트 영역의 크기와 처음 클라이언트 영역의 크기의 비율을 구한다.
		double multiplicandWidth = changedRectWidth / firstRectWidth;
		double multiplicandHeight = changedRectHeight / firstRectHeight;
		double multiplicandPoint = multiplicandWidth + multiplicandHeight 
			/ (firstRectWidth + firstRectHeight);
		//3.4 controlFont의 글자를 지운다.
		this->controlFont.DeleteObject();
		//3.5 ClientDC를 생성한다.
		CClientDC dc(this);
		//3.6 글씨크기와 글씨체를 정하다.
		this->controlFont.CreatePointFont(150 * multiplicandPoint, _T("나눔바른펜"), &dc);
		//3.7  폰트를 dc에 지정한다.
		dc.SelectObject(this->controlFont);
		//3.8 이전 페이지 버튼을 이동시킨다.
		double clientRectWidth = changedRectWidth;
		double previewWidth = clientRectWidth / 10 * 7;
		double printSetUpAreaWidth = clientRectWidth / 10 * 3;
		double centerOfprintSetUpAreaWidth = printSetUpAreaWidth / 2;
		this->previousPageButton.MoveWindow(CRect(previewWidth + centerOfprintSetUpAreaWidth
			- 100 * multiplicandWidth, 100 * multiplicandHeight, previewWidth
			+ centerOfprintSetUpAreaWidth + 100 * multiplicandWidth, 150 * multiplicandHeight));
		//3.9 이전 페이지 버튼의 글자크기와 글꼴을 변경시킨다.
		this->previousPageButton.SetFont(&this->controlFont);
		//3.10 다음 페이지 버튼을 이동시킨다.
		this->nextPageButton.MoveWindow(CRect(previewWidth + centerOfprintSetUpAreaWidth
			- 100 * multiplicandWidth, 250 * multiplicandHeight, previewWidth
			+ centerOfprintSetUpAreaWidth + 100 * multiplicandWidth, 300 * multiplicandHeight));
		//3.11 다음 페이지 버튼의 글자크기와 글꼴을 변경시킨다.
		this->nextPageButton.SetFont(&this->controlFont);
		//3.12  현재 페이지 번호 에디트 컨트롤을 이동시킨다.
		this->currentPageNumberEdit.MoveWindow(CRect(previewWidth + centerOfprintSetUpAreaWidth
			- 100 * multiplicandWidth, 180 * multiplicandHeight, previewWidth + centerOfprintSetUpAreaWidth
			- 30 * multiplicandWidth, 220 * multiplicandHeight));
		//3.13 현재 페이지 번호 에디트 컨트롤의 글자크기와 글꼴을 변경시킨다.
		this->GetDlgItem(IDC_EDIT_CURRENTPAGENUMBER)->SetFont(&this->controlFont);
		//3.14 슬래쉬 스태틱 컨트롤을 이동시킨다.
		this->slash.MoveWindow(CRect(previewWidth + centerOfprintSetUpAreaWidth 
			- 30 * multiplicandWidth, 180 * multiplicandHeight, previewWidth + 
			centerOfprintSetUpAreaWidth + 30 * multiplicandWidth, 220 * multiplicandHeight));
		//3.15 슬래쉬 스태틱 컨트롤의 글자크기와 글꼴을 변경한다.
		this->GetDlgItem(IDC_STATIC_SLASH)->SetFont(&this->controlFont);
		//3.16 총 페이지 수 스태틱 컨트롤을 이동시킨다.
		this->totalPageCountStatic.MoveWindow(CRect(previewWidth + centerOfprintSetUpAreaWidth 
			+ 30 * multiplicandWidth, 180 * multiplicandHeight, previewWidth 
			+ centerOfprintSetUpAreaWidth + 100 * multiplicandWidth, 220 * multiplicandHeight));
		//3.17 총 페이지 수 스태틱 컨트롤의 글자크기와 글꼴을 변경한다.
		this->GetDlgItem(IDC_STATIC_TOTALPAGECOUNT)->SetFont(&this->controlFont);
		//3.18 페이지 설정 버튼을 이동시킨다.
		this->pageSetUpButton.MoveWindow(CRect(previewWidth + centerOfprintSetUpAreaWidth
			- 100 * multiplicandWidth, 350 * multiplicandHeight, previewWidth
			+ centerOfprintSetUpAreaWidth + 100 * multiplicandWidth, 400 * multiplicandHeight));
		//3.19 페이지 설정 버튼의 글자크기와 글꼴을 변경시킨다.
		this->pageSetUpButton.SetFont(&this->controlFont);
		//3.20 인쇄 버튼을 이동시킨다.
		this->pagePrintButton.MoveWindow(CRect(previewWidth + centerOfprintSetUpAreaWidth
			- 100 * multiplicandWidth, 450 * multiplicandHeight, previewWidth
			+ centerOfprintSetUpAreaWidth + 100 * multiplicandWidth, 500 * multiplicandHeight));
		//3.21 인쇄 버튼의 글자크기와 글꼴을 변경시킨다.
		this->pagePrintButton.SetFont(&this->controlFont);
		//3.22 취소 버튼을 이동시킨다.
		this->pageCancelButton.MoveWindow(CRect(previewWidth + centerOfprintSetUpAreaWidth
			- 100 * multiplicandWidth, 550 * multiplicandHeight, previewWidth
			+ centerOfprintSetUpAreaWidth + 100 * multiplicandWidth, 600 * multiplicandHeight));
		//3.23 인쇄하기 버튼의 글자크기와 글꼴을 변경시킨다.
		this->pageCancelButton.SetFont(&this->controlFont);
	}
	//4. 변경사항을 갱신한다.
	this->Invalidate(TRUE);
}

//최소사이즈설정하기
void PreviewForm::OnGetMinMaxInfo(MINMAXINFO* lpMinMaxInfo)
{
	lpMinMaxInfo->ptMinTrackSize.x = 600;
	lpMinMaxInfo->ptMinTrackSize.y = 300;
}
//더블버퍼링을 막기 위한 조치
BOOL PreviewForm::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
//스태틱 컨트롤의 기본 배경색이 회색이라서 배경색을 투명하게 바꾸기 위한 조치
HBRUSH PreviewForm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	
	HBRUSH hbr = CFrameWnd::OnCtlColor(pDC, pWnd, nCtlColor);

	const UINT nID = pWnd->GetDlgCtrlID();
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT); /// 배경을 투명하게
		hbr = (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	return hbr;
}

//이전페이지 버튼을 클릭했을 때
void PreviewForm::OnPreviousPageButtonClicked()
{
	//1. 현재 페이지 번호가 1보다 크면
	if (this->currentPageNumber > 1)
	{
		//1.1 현재 페이지를 감소시킨다.
		this->currentPageNumber--;
		//1.2 현재 페이지 번호를 출력한다.
		this->currentPageNumberEdit.SetWindowText(to_string(this->currentPageNumber).c_str());
		//1.3 페이지를 업데이트시킨다.
		this->SendMessage(WM_COMMAND, ID_COMMAND_PAGEUPDATE);
	}
	//2. 현재 페이지 번호가 1이면
	else
	{
		//2.1 현재 페이지 번호를 출력한다.
		this->currentPageNumberEdit.SetWindowText(to_string(this->currentPageNumber).c_str());
	}
	//3. 이전페이지 버튼에 포커스를 없앤다.
	this->previousPageButton.SendMessage(WM_KILLFOCUS);
	//4. 미리보기폼에 포커스를 설정한다.
	this->SetFocus();
}

//다음페이지 버튼을 클릭했을 때
void PreviewForm::OnNextPageButtonClicked()
{
	//1. 현재 페이지 번호가 총페이지수보다 작으면
	if (this->currentPageNumber < this->totalPageCount)
	{
		//1.1 현재 페이지 번호를 증가시킨다.
		this->currentPageNumber++;
		//1.2 현재 페이지 번호를 출력한다.
		this->currentPageNumberEdit.SetWindowText(to_string(this->currentPageNumber).c_str());
		//1.3 페이지를 업데이트시킨다.
		this->SendMessage(WM_COMMAND, ID_COMMAND_PAGEUPDATE);
	}
	//2. 현재 페이지 번호가 총페이지수와 같으면
	else
	{
		//2.1 현재 페이지 번호를 출력한다.
		this->currentPageNumberEdit.SetWindowText(to_string(this->currentPageNumber).c_str());
	}
	//3. 다음페이지 버튼에 포커스를 없앤다.
	this->nextPageButton.SendMessage(WM_KILLFOCUS);
	//4. 미리보기 폼에 포커스를 설정한다.
	this->SetFocus();
}

//페이지 설정 버튼을 클릭했을 때
void PreviewForm::OnPageSetUpButtonClicked()
{
#if 0
	//1. 페이지 설정 다이얼로그를 생성한다.
	CPageSetupDialog dlg(PSD_MARGINS | PSD_INHUNDREDTHSOFMILLIMETERS);

	// Initialize margins
	dlg.m_psd.rtMargin.top = 1000;
	dlg.m_psd.rtMargin.left = 1000;
	dlg.m_psd.rtMargin.right = 1000;
	dlg.m_psd.rtMargin.bottom = 1000;
	//dlg.m_psd.lpfnPagePaintHook = (LPPAGEPAINTHOOK)PaintHook;

	if (IDOK == dlg.DoModal())
	{
		// Propagate changes to the app
		AfxGetApp()->SelectPrinter(dlg.m_psd.hDevNames, dlg.m_psd.hDevMode);
	}
	else
	{
		TRACE(_T("CommDlgExtendedError returned error %d from ")
			_T("CPageSetupDialog::DoModal().\n"),
			(int)CommDlgExtendedError());
	}
#endif
	CPageSetupDialog psd(PSD_INTHOUSANDTHSOFINCHES | PSD_MARGINS |
		PSD_ENABLEPAGEPAINTHOOK, this);

	// Initialize margins
	psd.m_psd.rtMargin.top = 1000;
	psd.m_psd.rtMargin.left = 1250;
	psd.m_psd.rtMargin.right = 1250;
	psd.m_psd.rtMargin.bottom = 1000;
	//psd.m_psd.lpfnPagePaintHook = (LPPAGEPAINTHOOK)PaintHook;

	if (IDOK == psd.DoModal())
	{
		// Propagate changes to the app
		AfxGetApp()->SelectPrinter(psd.m_psd.hDevNames, psd.m_psd.hDevMode);
	}
	else
	{
		TRACE(_T("CommDlgExtendedError returned error %d from ")
			_T("CPageSetupDialog::DoModal().\n"),
			(int)CommDlgExtendedError());
	}

	//3. 다음페이지 버튼에 포커스를 없앤다.
	this->nextPageButton.SendMessage(WM_KILLFOCUS);
	//4. 미리보기 폼에 포커스를 설정한다.
	this->SetFocus();
}

//인쇄 버튼을 클릭했을 때
void PreviewForm::OnPagePrintButtonClicked()
{
	//1. 프린트 다이얼로그를 생성한다.
	CPrintDialog dlg(FALSE, PD_ALLPAGES | PD_COLLATE | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE);
	//2. 프린트 다이얼로그에서 OK버튼을 눌렀으면
	if (dlg.DoModal() == IDOK)
	{
		//2.1 기존에 printInformation이 있으면
		if (this->notepadForm->printInformation != 0)
		{
			//2.1.1 기존에 있는 printInformation을 할당해제한다.
			delete this->notepadForm->printInformation;
		}
		//2.2 프린트 다이얼로그에서 hdc를 구한다.
		HDC hdc = dlg.CreatePrinterDC();
		ASSERT(hdc);
		//2.3 프린트 다이얼로그의 hdc에서 cdc를 구한다.
		CDC* cdc = CDC::FromHandle(hdc);
		//2.4 프린트할 문서 정보를 담을 공간을 선언한다.
		DOCINFO docinfo;
		//2.5 프린트할 문서 정보를 담을 공간을 초기화시켜준다.
		ZeroMemory(&docinfo, sizeof(DOCINFO));
		//2.6 프린트할 문서 정보의 이름을 정한다.
		docinfo.lpszDocName = "메모장";
		//2.7 프린트를 시작한다.
		cdc->StartDoc(&docinfo);
		//2.8 PrintingVisitor를 생성한다.
		PrintingVisitor printingVisitor = PrintingVisitor(this->notepadForm, cdc, 0, 0);
		//2.9 프린트 비율로 글꼴의 비율을 맞춰준다.
		LOGFONT printLogFont = this->notepadForm->font.GetLogFont();
		printLogFont.lfHeight = -MulDiv(this->notepadForm->font.GetSize() / 10, 600, 72);
		CFont font;
		HFONT oldFont;
		font.CreateFontIndirect(&printLogFont);
		oldFont = (HFONT)cdc->SelectObject(font);
		//2.10 프린트정보를 저장할 클래스를 생성한다.
		this->notepadForm->printInformation = new PrintInformation(this->notepadForm,
			printLogFont, hdc);
		//2.11 프린트 출력을 한다.
		this->notepadForm->printInformation->GetPrintNote()->Accept(&printingVisitor);
		//2.12 프린트를 끝낸다.
		cdc->EndDoc();
	}
	//3. 다음페이지 버튼에 포커스를 없앤다.
	this->nextPageButton.SendMessage(WM_KILLFOCUS);
	//4. 미리보기 폼에 포커스를 설정한다.
	this->SetFocus();
}

//취소 버튼을 클릭했을 때
void PreviewForm::OnPageCancelButtonClicked()
{
	this->PostMessage(WM_CLOSE);
}

//방향키버튼을 클릭했을 떄
void PreviewForm::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 오른쪽 방향키 또는 아래 방향키를 눌렀으면
	if (nChar == VK_RIGHT || nChar == VK_DOWN)
	{
		//1.1 다음페이지 버튼틀 클릭했을 때로 메세지를 보낸다.
		this->SendMessage(WM_COMMAND, IDC_BUTTON_NEXTPAGE);
	}
	//2. 왼쪽 방향키 또는 위쪽 방향키를 눌렀으면
	else if (nChar == VK_LEFT || nChar == VK_UP)
	{
		//2.1 이전페이지 버튼틀 클릭했을 때로 메세지를 보낸다.
		this->SendMessage(WM_COMMAND, IDC_BUTTON_PREVIOUSPAGE);
	}
}

//Command패턴
void PreviewForm::OnCommand()
{
	//1. 미리보기폼의 노트를 할당해제한다.
	if (this->note != 0)
	{
		delete this->note;
	}
	//2. 미리보기폼의 노트를 새로 생성한다.
	this->note = new Note();
	//3. 프린트할 노트를 구한다.
	Glyph* printNote = this->notepadForm->printInformation->GetPrintNote();
	//4. 한 페이지당 인쇄할 줄의 수를 구한다.
	Long pageRowCount = this->notepadForm->printInformation->GetPageRowCount();
	//5. 한 페이지당 인쇄할 줄의 개수보다 작은동안 그리고
	//프린트할 노트의 줄의 개수보다 작은동안 반복한다.
	Glyph* row = 0;
	Long rowIndex = pageRowCount * this->currentPageNumber - pageRowCount;
	Long i = 0;
	while (i < pageRowCount && rowIndex < printNote->GetLength())
	{
		//5.1 현재 줄을 구한다.
		row = printNote->GetAt(rowIndex)->Clone();
		//5.2 현재 줄을 previewForm 노트에 추가한다.
		this->note->Add(row);
		//5.3 줄의 위치를 증가시킨다.
		rowIndex++;
		i++;
	}
	//6. 변경사항을 갱신한다.
	this->Invalidate(TRUE);
}

//소멸자
void PreviewForm::OnClose()
{
	this->notepadForm->previewForm = 0;
	if (this->note != 0)
	{
		delete this->note;
	}
	CFrameWnd::OnClose();
}