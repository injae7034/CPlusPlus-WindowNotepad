#ifndef _PREVIEWFORM_H
#define _PREVIEWFORM_H

#include<afxwin.h>//CDialog �������

typedef signed long int Long;
class NotepadForm;
class Glyph;

class PreviewForm :public CFrameWnd
{
public:
	enum {
		IDC_BUTTON_PREVIOUSPAGE = 40001, IDC_BUTTON_NEXTPAGE = 40002,
		IDC_EDIT_CURRENTPAGENUMBER = 40003, IDC_STATIC_SLASH = 40004,
		IDC_STATIC_TOTALPAGECOUNT = 40005, ID_COMMAND_PAGEUPDATE = 40006,
		IDC_BUTTON_PAGESETUP = 40007, IDC_BUTTON_PAGEPRINT = 40008,
		IDC_BUTTON_PAGECANCEL = 40009
	};
public:
	NotepadForm* notepadForm;
public:
	PreviewForm(NotepadForm* notepadForm);//������
	//�ζ����Լ� ����
	CRect GetFirstClientRect() const;
	Glyph* GetNote() const;
protected://#
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMinMaxInfo);
	afx_msg HBRUSH  OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPreviousPageButtonClicked();
	afx_msg void OnNextPageButtonClicked();
	afx_msg void OnPageSetUpButtonClicked();
	afx_msg void OnPagePrintButtonClicked();
	afx_msg void OnPageCancelButtonClicked();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCommand();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
private:
	CRect firstClientRect;//�̸����� �� Ȯ�� �� ����� �� ������ ���ϱ� ���ؼ� ó�� ȭ��ũ�⸦ ������.
	CButton previousPageButton;//���������� ��ư
	CButton nextPageButton;//���������� ��ư
	CButton pageSetUpButton;//���������� ��ư
	CButton pagePrintButton;//�μ��ϱ� ��ư
	CButton pageCancelButton;//����ϱ� ��ư
	CEdit currentPageNumberEdit;//���� ������ ��ȣ�� �� ����Ʈ��Ʈ��
	CStatic slash;
	CStatic totalPageCountStatic;//�� ������ ���� �� ����ƽ��Ʈ��
	CFont controlFont;//��Ʈ�ѿ� ����� ��Ʈ
	Glyph* note;//���������� ��µ� ��Ʈ
	Long currentPageNumber;//���� ������ ��ȣ
	Long totalPageCount;//�� ������ ��
};

//�ζ����Լ�����
inline CRect PreviewForm::GetFirstClientRect() const
{
	return this->firstClientRect;
}
inline Glyph* PreviewForm::GetNote() const
{
	return const_cast<Glyph*>(this->note);
}

#endif // !_PREVIEWFORM_H
