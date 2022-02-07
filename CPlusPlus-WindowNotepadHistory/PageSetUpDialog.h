#ifndef _PAGESETUPDIALOG_H
#define _PAGESETUPDIALOG_H

#include<afxdlgs.h>
#include"resource.h"

class NotepadForm;

class PageSetupDialog :public CPageSetupDialog
{
public:
	CWnd* notepadForm;
	enum { IDD = IDD_PAGESETUPDIALOG };

public:
	PageSetupDialog(CWnd *pParentWnd = NULL);
	virtual INT_PTR DoModal();
	CString GetHeader() const;
	CString GetFooter() const;
	~PageSetupDialog();
protected:
	afx_msg virtual BOOL OnInitDialog();
	afx_msg virtual void OnOK();
	DECLARE_MESSAGE_MAP()
private:
	//멤버로 CComonDialog에서 CString이나 string을 멤버로 하면 메모리 누수가 발생하기 때문에
	//이를 방지하기 위해서 멤버로 배열 포인터를 선언한다.
	char (*header);
	char (*footer);
};

inline CString PageSetupDialog::GetHeader() const
{
	return this->header;
}

inline CString PageSetupDialog::GetFooter() const
{
	return this->footer;
}

#endif // !_PAGESETUPDIALOG_H