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
	//����� CComonDialog���� CString�̳� string�� ����� �ϸ� �޸� ������ �߻��ϱ� ������
	//�̸� �����ϱ� ���ؼ� ����� �迭 �����͸� �����Ѵ�.
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