#ifndef _PAGESETUPDIALOG_H
#define _PAGESETUPDIALOG_H

#include<afxdlgs.h>
#include"resource.h"

class NotepadForm;

class PageSetupDialog :public CPageSetupDialog
{
public:
	enum { IDD = IDD_PAGESETUPFORM };

public:
	PageSetupDialog(CWnd *pParentWnd = NULL);
	virtual INT_PTR DoModal();
	CString GetHeader();
	CString GetFooter();

protected:
	afx_msg virtual BOOL OnInitDialog();
	afx_msg virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	CString header;
	CString footer;
};

#endif // !_PAGESETUPDIALOG_H