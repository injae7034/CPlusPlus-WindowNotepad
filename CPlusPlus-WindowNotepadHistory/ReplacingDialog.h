#ifndef _REPLACINGDIALOG_H
#define _REPLACINGDIALOG_H
#include "afxdlgs.h"//CCommonDialog�������
#include "resource.h"
#include<string>
using namespace std;

typedef signed long int Long;
class NotepadForm;
//CFindReplaceDialog�� �θ��̰� ReplacingDialog�� ��ӹ��� �ڽ���.
class ReplacingDialog :public CFindReplaceDialog
{
public:
	enum { IDD = IDD_REPLACINGDIALOG };
public:
	NotepadForm* notepadForm;
	string findingKeyword;//ã�� �ܾ�
	string replacingKeyword;//�ٲ� �ܾ�
	int matchCaseChecked;//��ҹ��� ���� üũ
public:
	ReplacingDialog(CWnd* parent = NULL);//������, NULL�� ����Ʈ��
	virtual BOOL OnInitDialog();
protected://#
	afx_msg void OnFindingContentEditTyped();
	afx_msg void OnSwapButtonClicked();
	afx_msg void OnFindButtonClicked();
	afx_msg void OnReplacedButtonClicked();
	afx_msg void OnReplaceAllButtonClicked();
	afx_msg void OnCancelButtonClicked();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
};

#endif // !_REPLACINGDIALOG_H

