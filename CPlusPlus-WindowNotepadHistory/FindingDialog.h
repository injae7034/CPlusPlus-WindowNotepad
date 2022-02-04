#ifndef _FINDINGDIALOG_H
#define _FINDINGDIALOG_H
#include "afxdlgs.h"//CCommonDialog헤더파일
#include "resource.h"

typedef signed long int Long;
class NotepadForm;
//CFindReplaceDialog가 부모이고 FindingDialog는 상속받은 자식임.
class FindingDialog :public CFindReplaceDialog
{
public:
	enum { IDD = IDD_FINDINGDIALOG };
public:
	NotepadForm* notepadForm;
public:
	FindingDialog(CWnd* parent = NULL);//생성자, NULL이 디폴트값
	virtual BOOL OnInitDialog();
protected://#
	afx_msg void OnFindButtonClicked();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
};

#endif // !_FINDINGDIALOG_H

