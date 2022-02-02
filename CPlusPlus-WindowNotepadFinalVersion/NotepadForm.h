#ifndef _NOTEPADFORM_H
#define _NOTEPADFORM_H

#include "afxdlgs.h"//CCommonDialog헤더파일(CFindReplaceDialog를 이용하려면 필요함.)
#include "Subject.h"
#include "Font.h"
#include "resource.h"
#include<afxwin.h>//CDialog 헤더파일
#include<string>
using namespace std;

class CommandHistory;
class SelectingTexts;
class CaretController;
class PageMoveController;
class ScrollController;
class TextExtent;
class Glyph;
class PrintInformation;
class PreviewForm;
class PageSetUpInformation;

typedef signed long int Long;

class NotepadForm :public CFrameWnd, public Subject//다중상속
{
public:
	NotepadForm();//생성자
public:
	//주소는 public으로
	PageSetUpInformation* pageSetUpInformation;
	PreviewForm* previewForm;
	PrintInformation* printInformation;
	Glyph* note;
	Glyph* current;
	Glyph* clipboard;
	Font font;
	TextExtent* textExtent;
	SelectingTexts* selectingTexts;
	ScrollController* scrollController;
	PageMoveController* pageMoveController;
	CaretController* caretController;
	CFindReplaceDialog* findReplaceDialog;
	CommandHistory* commandHistory;
	
	//flag는 public으로
	bool isMouseLButtonClicked;//마우스 왼쪽 버튼을 클릭했는지 판별하기 위한 flag
	bool isRowAutoChanging;//자동개행이 되는 중인지 아닌지 판별하기 위한 flag
	bool isSelecting;//텍스트 선택을 하는 중인지 아닌지 판별하기 위한 flag
	bool isComposing;//한글이 조립중인지 아닌지 판별하기 위한 flag
	bool isDirty;//새파일인지 아닌지 판별하기 위한 flag
	string fileName;//현재 열려있는 메모장의 파일명
	string filePath;//현재 열려있는 메모장의 파일경로
	Long selectedStartXPos;//선택이 시작되는 x좌표
	Long selectedStartYPos;//선택이 시작되는 y좌표
	Long previousPageWidth;
	CMenu mouseRButtonMenu;
	//인라인함수 정의
	Glyph* GetGlyph() const;
	Glyph* GetRemovedSelectedTexts() const;
protected://#
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg LRESULT OnStartCompostion(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnComposition(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnImeChar(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnCommand(UINT nID);
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
private:
	//menu가 notepadForm의 멤버로 있어야 OnCreate스택이 종료되어도
	//menu가 계속 notepadForm에 setMenu된채로 있게됨 단독적으로 CMenu가 있는 경우
	//OnCreate 스택이 종료되면 CMenu가 사라지기때문에 뻑이남.
	CMenu menu;
	Glyph* glyph;//OnCharcommand와 OnImeCharCommand에 SingleByteLetter나
	//DoubleByteLetter를 전달하기 위해 임시저장
	Glyph* removedSelectedTexts;//OnImeCharcommand에 지워진 선택된 영역을 전달하기 위해 임시저장
	HICON icon;
};

//인라인함수정의
inline Glyph* NotepadForm::GetGlyph() const
{
	return const_cast<Glyph*>(this->glyph);
}
inline Glyph* NotepadForm::GetRemovedSelectedTexts() const
{
	return const_cast<Glyph*>(this->removedSelectedTexts);
}

//함수선언(함수포인터느낌)
LRESULT CALLBACK SaveMessageBoxProc(int nCode, WPARAM wParam, LPARAM lParam);
int SaveMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT nType);

#endif // !_NOTEPADFORM_H
