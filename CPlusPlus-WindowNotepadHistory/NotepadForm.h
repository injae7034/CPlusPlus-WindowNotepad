#ifndef _NOTEPADFORM_H
#define _NOTEPADFORM_H

#include "Subject.h"
#include "Font.h"
#include "resource.h"
#include<afxwin.h>//CDialog 헤더파일

class Glyph;
typedef signed long int Long;

class NotepadForm :public CFrameWnd, public Subject//다중상속
{
public:
	NotepadForm();//생성자
	//CMenu& GetCMenu() const; GetMenu가 있어서 따로 CMenu를 구하기 위한 인라인함수가 필요없음
public:
	//주소는 public으로
	Glyph* note;
	Glyph* current;
	Font font;
	//flag는 public으로
	bool IsComposing;//한글이 조립중인지 아닌지 판별하기 위한 flag
	bool IsDirty;//새파일인지 아닌지 판별하기 위한 flag
	string fileName;//현재 열려있는 메모장의 파일명
	string filePath;//현재 열려있는 메모장의 파일경로
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
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
private:
	//menu가 notepadForm의 멤버로 있어야 OnCreate스택이 종료되어도
	//menu가 계속 notepadForm에 setMenu된채로 있게됨 단독적으로 CMenu가 있는 경우
	//OnCreate 스택이 종료되면 CMenu가 사라지기때문에 뻑이남.
	CMenu menu;
};

//함수선언(함수포인터느낌)
LRESULT CALLBACK SaveMessageBoxProc(int nCode, WPARAM wParam, LPARAM lParam);
int SaveMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT nType);

#endif // !_NOTEPADFORM_H
