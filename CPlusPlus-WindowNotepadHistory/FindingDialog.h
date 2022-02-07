#ifndef _FINDINGDIALOG_H
#define _FINDINGDIALOG_H
#include "afxdlgs.h"//CCommonDialog�������
#include "resource.h"

typedef signed long int Long;
class NotepadForm;
//CFindReplaceDialog�� �θ��̰� FindingDialog�� ��ӹ��� �ڽ���.
class FindingDialog :public CFindReplaceDialog
{
public:
	enum { IDD = IDD_FINDINGDIALOG };
public:
	NotepadForm* notepadForm;
public:
	FindingDialog(CWnd* parent = NULL);//������, NULL�� ����Ʈ��
	virtual BOOL OnInitDialog();
protected://#
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnFindingContentEditTyped();
	afx_msg void OnFindButtonClicked();
	afx_msg void OnCancelButtonClicked();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
};

#endif // !_FINDINGDIALOG_H

