#ifndef _PREVIEWFORM_H
#define _PREVIEWFORM_H

#include<afxwin.h>//CDialog 헤더파일

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
	PreviewForm(NotepadForm* notepadForm);//생성자
	//인라인함수 정의
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
	CRect firstClientRect;//미리보기 폼 확대 및 축소할 때 비율을 구하기 위해서 처음 화면크기를 저장함.
	CButton previousPageButton;//이전페이지 버튼
	CButton nextPageButton;//다음페이지 버튼
	CButton pageSetUpButton;//페이지설정 버튼
	CButton pagePrintButton;//인쇄하기 버튼
	CButton pageCancelButton;//취소하기 버튼
	CEdit currentPageNumberEdit;//현재 페이지 번호를 쓸 에디트컨트롤
	CStatic slash;
	CStatic totalPageCountStatic;//총 페이지 수로 쓸 스태틱컨트롤
	CFont controlFont;//컨트롤에 사용할 폰트
	Glyph* note;//한페이지에 출력될 노트
	Long currentPageNumber;//현재 페이지 번호
	Long totalPageCount;//총 페이지 수
};

//인라인함수정의
inline CRect PreviewForm::GetFirstClientRect() const
{
	return this->firstClientRect;
}
inline Glyph* PreviewForm::GetNote() const
{
	return const_cast<Glyph*>(this->note);
}

#endif // !_PREVIEWFORM_H
