#include "PageSetupDialog.h"
#include "NotepadForm.h"
#include "PreviewForm.h"
#include <dlgs.h>
#include <afxdisp.h>
#include "resource.h"
#pragma warning(disable:4996)

BEGIN_MESSAGE_MAP(PageSetupDialog, CPageSetupDialog)
	ON_BN_CLICKED(IDOK, OnOK)
END_MESSAGE_MAP()

PageSetupDialog::PageSetupDialog(CWnd* pParentWnd)
	: CPageSetupDialog(PSD_ENABLEPAGESETUPTEMPLATE | PSD_MARGINS, pParentWnd)
{
	this->notepadForm = pParentWnd;
	this->m_psd.Flags |= PSD_ENABLEPAGESETUPTEMPLATE;
	this->m_psd.hInstance = AfxGetResourceHandle();
	this->m_psd.lpPageSetupTemplateName = MAKEINTRESOURCE(PageSetupDialog::IDD);
	//배열을 힙에 할당하고 주소를 멤버에 저장함.
	this->header = new char[512];
	this->header[0] = '\0';
	this->footer = new char[512];
	this->footer[0] = '\0';
}

//FileSetUpCommand함수 스택에서 PageSetupDialog가 생성되어 호출되기 때문에 FileSetUpCommand함수 스택이
//종료될 때 자동으로 PageSetupDialog의 소멸자가 호출된다.
PageSetupDialog::~PageSetupDialog()
{
	//힙에 할당한 문자배열을 할당해제해줌.
	if (this->header != 0)
	{
		delete[] this->header;
	}
	if (this->footer != 0)
	{
		delete[] this->footer;
	}
}

BOOL PageSetupDialog::OnInitDialog() 
{
	CPageSetupDialog::OnInitDialog();
	//2A와 4A용지는 너무 커서 미리보기와 인쇄가 안되기 때문에 지워버림!
	((CComboBox*)this->GetDlgItem(1137))->DeleteString(5);
	((CComboBox*)this->GetDlgItem(1137))->DeleteString(5);

	//1. 레지스트리에 저장된 머리글과 바닥글을 읽는다.
	//머리글이나 바닥글이 저장이 안되어있으면 ""을 읽는다.
	CString header = AfxGetApp()->GetProfileString("NotepadSection", "Header", "");
	CString footer = AfxGetApp()->GetProfileString("NotepadSection", "Footer", "");
	//2. 레지스트리에서 읽은 머리글과 바닥글을 머리글과 바닥글 에디트에 출력한다.
	this->GetDlgItem(IDC_EDIT_HEADER)->SetWindowText(header);
	this->GetDlgItem(IDC_EDIT_FOOTER)->SetWindowText(footer);
	//3. 레지스트리에 저장된 여백정보를 읽는다.
	//저장된 여백정보가 없으면 0을 읽는다.
	Long marginLeft = AfxGetApp()->GetProfileInt("NotepadSection", "MarginLeft", 0) / 100;
	Long marginTop = AfxGetApp()->GetProfileInt("NotepadSection", "MarginTop", 0) / 100;
	Long marginRight = AfxGetApp()->GetProfileInt("NotepadSection", "MarginRight", 0) / 100;
	Long marginBottom = AfxGetApp()->GetProfileInt("NotepadSection", "MarginBottom", 0) / 100;
	//4. 레지스트리에서 읽은 여백 정보를 여백에디트에 출력한다.
	string left = to_string(marginLeft);
	this->GetDlgItem(1155)->SetWindowText(left.c_str());
	string top = to_string(marginTop);
	this->GetDlgItem(1156)->SetWindowText(top.c_str());
	string right = to_string(marginRight);
	this->GetDlgItem(1157)->SetWindowText(right.c_str());
	string bottom = to_string(marginBottom);
	this->GetDlgItem(1158)->SetWindowText(bottom.c_str());
	//5. 레지스트리에서 읽은 가로/세로 정보를 읽는다.
	Long paperOrientation = AfxGetApp()->GetProfileInt("NotepadSection", "PaperOrientation",
		1);
	//6. 세로면
	if (paperOrientation == 1)
	{
		//6.1 세로 버튼을 체크한다.
		((CButton*)this->GetDlgItem(1056))->SetCheck(BST_CHECKED);
		((CButton*)this->GetDlgItem(1057))->SetCheck(BST_UNCHECKED);	
	}
	//7. 가로면
	else
	{
		//7.1 가로 버튼을 체크한다.
		((CButton*)this->GetDlgItem(1056))->SetCheck(BST_UNCHECKED);
		((CButton*)this->GetDlgItem(1057))->SetCheck(BST_CHECKED);
	}

	return TRUE;
}

INT_PTR PageSetupDialog::DoModal()
{
	//용지 방향과 크기 정보를 설정하기 위해 PageSetupDialog가 뜨기 전에 정보를 얻어와서 설정하기
	DWORD prevFlags;
	LPDEVMODE devmode = (LPDEVMODE)::GlobalLock(this->m_psd.hDevMode);
	if (devmode == 0)
	{
		prevFlags = this->m_psd.Flags;
		this->m_psd.Flags |= PSD_RETURNDEFAULT;
		CPageSetupDialog::DoModal();

		devmode = (LPDEVMODE)::GlobalLock(this->m_psd.hDevMode);
		this->m_psd.Flags = prevFlags;
	}
	
	devmode->dmFields |= DM_ORIENTATION | DM_PAPERSIZE;
	devmode->dmPaperSize = AfxGetApp()->GetProfileInt("NotepadSection", "PaperSize",
		DMPAPER_A4);
	Long paperOrientation = AfxGetApp()->GetProfileInt("NotepadSection", "PaperOrientation",
		1);
	if (paperOrientation == 1)
	{
		devmode->dmOrientation = DMORIENT_PORTRAIT;
	}
	else
	{
		devmode->dmOrientation = DMORIENT_LANDSCAPE;
	}

	::GlobalUnlock(devmode);

	return CPageSetupDialog::DoModal();
}

//페이지 설정에서 OK버튼을 클릭했을 때
void PageSetupDialog::OnOK() 
{
	//1. 머리글과 바닥글 에디트 컨트롤에 적혀 있는 글자를 읽는다.
	CString header;
	CString footer;
	this->GetDlgItem(IDC_EDIT_HEADER)->GetWindowText(header);
	this->GetDlgItem(IDC_EDIT_FOOTER)->GetWindowText(footer);
	strcpy(this->header, (LPCTSTR)header);
	strcpy(this->footer, (LPCTSTR)footer);
	AfxGetApp()->WriteProfileString("NotepadSection", "Header", header);
	AfxGetApp()->WriteProfileString("NotepadSection", "Footer", footer);
	//2. 페이지 설정 대화상자에서 devmode구조체를 구한다.
	DEVMODE* devmode = this->GetDevMode();
	//3. 페이지 설정 대화상자에서 hdc를 구한다.
	HDC hdc = this->CreatePrinterDC();
	//4. hdc에서 cdc를 구한다.
	CDC* cdc = CDC::FromHandle(hdc);
	if (cdc != 0)
	{
		//5. 프린트가 가능한 영역을 구한다.
		CRect rect(0, 0, cdc->GetDeviceCaps(HORZRES), cdc->GetDeviceCaps(VERTRES));
		//6. 페이지설정 대화상자에서 여백을 구한다.
		CRect marginRect;
		this->GetMargins(&marginRect, NULL);
		AfxGetApp()->WriteProfileInt("NotepadSection", "MarginLeft", marginRect.left);
		AfxGetApp()->WriteProfileInt("NotepadSection", "MarginTop", marginRect.top);
		AfxGetApp()->WriteProfileInt("NotepadSection", "MarginRight", marginRect.right);
		AfxGetApp()->WriteProfileInt("NotepadSection", "MarginBottom", marginRect.bottom);
		//7. 구한 여백을 페이지 화면 비율에 맞게 다시 설정한다.
		Long changedMarginRectLeft = marginRect.left * 600 / 2540;
		Long changedMarginRectTop = marginRect.top * 600 / 2540;
		Long changedMarginRectRight = marginRect.right * 600 / 2540;
		Long changedMarginRectBottom = marginRect.bottom * 600 / 2540;
		CRect changedMarginRect(changedMarginRectLeft, changedMarginRectTop,
			changedMarginRectRight, changedMarginRectBottom);
		//8. 여백을 제외하고 프린트 가능한 영역을 구한다.
		CRect printableRect(changedMarginRect.left, changedMarginRect.top,
			rect.Width() - changedMarginRect.right, rect.Height() - changedMarginRect.bottom);
		//9. 현재 화면의 가로 길이를 구한다.
		Long pageWidth = printableRect.right - printableRect.left;
		Long pageHeight = printableRect.bottom - printableRect.top;
		LOGFONT printLogFont;
		//10. PageSetUpDialog의 부모 윈도우가 NotepadForm이면
		if (dynamic_cast<NotepadForm*>(this->notepadForm))
		{
			//10.1 프린트 비율로 글꼴의 비율을 맞춰준다.
			printLogFont = dynamic_cast<NotepadForm*>(this->notepadForm)->font.GetLogFont();
			printLogFont.lfHeight = -MulDiv(dynamic_cast<NotepadForm*>(this->notepadForm)->
				font.GetSize() / 10, 600, 72);
		}
		//11. PageSetUpDialog의 부모 윈도우가 PreviewForm이면
		else if (dynamic_cast<PreviewForm*>(this->notepadForm))
		{
			//11.1 프린트 비율로 글꼴의 비율을 맞춰준다.
			printLogFont = dynamic_cast<PreviewForm*>(this->notepadForm)
				->notepadForm->font.GetLogFont();
			printLogFont.lfHeight = -MulDiv(dynamic_cast<PreviewForm*>(this->notepadForm)
				->notepadForm->font.GetSize() / 10, 600, 72);
		}
		//레지스트리에 용지방향을 저장한다.
		AfxGetApp()->WriteProfileInt("NotepadSection", "PaperOrientation", 
			devmode->dmOrientation);
		//레지스트리에 용지크기를 저장한다.
		AfxGetApp()->WriteProfileInt("NotepadSection", "PaperSize", devmode->dmPaperSize);

		CFont font;
		HFONT oldFont;
		font.CreateFontIndirect(&printLogFont);
		oldFont = (HFONT)cdc->SelectObject(font);
		//11. 글꼴의 높이를 통해 한페이지당 줄의 개수를 센다.
		TEXTMETRIC text;
		cdc->GetTextMetrics(&text);
		Long pageRowCount = pageHeight / text.tmHeight;
		//12. 머리글이 있으면
		Long headerWidth = 0;
		if (strcmp(this->header, "") != 0)
		{
			//12.1 한 페이지당 줄의 개수를 감소시킨다.
			pageRowCount--;
			//12.2 머리글의 길이를 구한다.
			headerWidth = cdc->GetTextExtent(this->header).cx;
		}
		//13. 바닥글이 있으면
		Long footerWidth = 0;
		if (strcmp(this->footer, "") != 0)
		{
			//13.1 한 페이지당 줄의 개수를 감소시킨다.
			pageRowCount--;
			//13.2 바닥들의 길이를 구한다.
			footerWidth = cdc->GetTextExtent(this->footer).cx;
		}
		//14. 한글자의 최대 길이를 구한다.
		Long letterMaxWidth = text.tmMaxCharWidth;
		//13. 여백을 제외한 페이지 길이가 한글자의 최대 길이보다 크고 페이지 줄의 개수가 0보다 크고,
		//여백을 제외한 페이지 길이가 머리글과 바닥글의 길이보다 크면
		if (pageWidth > letterMaxWidth && pageRowCount > 0
			&& pageWidth > headerWidth && pageWidth > footerWidth)
		{
			//13.1 페이지 설정 대화상자에서 페이지 설정 정보를 반영하고 페이지 대화상자를 닫는다.
			CPageSetupDialog::OnOK();
		}
		//14. 그렇지 않으면
		else
		{
			//14.1 여백을 다시 설정하라는 메세지박스를 출력한다.
			int messageBoxButton = MessageBox("여백 또는 머리글이나 바닥글이 종이 크기를 넘었습니다.\n"
				"여백 크기 또는 머리글이나 바닥글이 종이 크기를 넘지 않게 다시 입력하십시오.",
				"페이지 설정", MB_OK);
		}
	}
	else
	{
		CPageSetupDialog::OnOK();
	}
}