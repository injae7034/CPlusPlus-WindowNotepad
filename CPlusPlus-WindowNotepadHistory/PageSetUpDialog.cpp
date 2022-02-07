#include "PageSetupDialog.h"
#include "NotepadForm.h"
#include <dlgs.h>
#include <afxdisp.h>
#include "resource.h"

BEGIN_MESSAGE_MAP(PageSetupDialog, CPageSetupDialog)
	ON_BN_CLICKED(IDOK, OnOK)
END_MESSAGE_MAP()

PageSetupDialog::PageSetupDialog(CWnd* pParentWnd)
	: CPageSetupDialog(PSD_ENABLEPAGESETUPTEMPLATE | PSD_MARGINS, pParentWnd)
{
	this->m_psd.Flags |= PSD_ENABLEPAGESETUPTEMPLATE;
	this->m_psd.hInstance = AfxGetResourceHandle();
	this->m_psd.lpPageSetupTemplateName = MAKEINTRESOURCE(PageSetupDialog::IDD);
}

BOOL PageSetupDialog::OnInitDialog() 
{
	CPageSetupDialog::OnInitDialog();


	return TRUE;
}

INT_PTR PageSetupDialog::DoModal()
{
#if 0
	DWORD prevFlags;

	LPDEVMODE devmode = (LPDEVMODE)::GlobalLock(this->m_psd.hDevMode);
	if (devmode == 0) {
		prevFlags = this->m_psd.Flags;
		this->m_psd.Flags |= PSD_RETURNDEFAULT;
		CPageSetupDialog::DoModal();

		devmode = (LPDEVMODE)::GlobalLock(this->m_psd.hDevMode);
		this->m_psd.Flags = prevFlags;
	}

	//devmode->dmFields |= DM_ORIENTATION | DM_PAPERSIZE;


	::GlobalUnlock(devmode);
#endif
	return CPageSetupDialog::DoModal();
}

void PageSetupDialog::OnOK() 
{ // 여기서 써줘야함.
	
	this->GetDlgItem(IDC_EDIT_HEADER)->GetWindowText(this->header);
	this->GetDlgItem(IDC_EDIT_FOOTER)->GetWindowText(this->footer);


	LPDEVMODE devmode = (LPDEVMODE)::GlobalLock(this->m_psd.hDevMode);

	devmode = (LPDEVMODE)::GlobalLock(this->m_psd.hDevMode);
	

	::GlobalUnlock(devmode);

	CPageSetupDialog::OnOK();
}

CString PageSetupDialog::GetHeader()
{
	return this->header;
}

CString PageSetupDialog::GetFooter()
{
	return this->footer;
}