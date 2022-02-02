#ifndef _NOTEPADFORM_H
#define _NOTEPADFORM_H

#include "Glyph.h"
#include "Font.h"
#include "Caret.h"
#include "resource.h"
#include "Command.h"
#include<afxwin.h>//CDialog 헤더파일


typedef signed long int Long;

class NotepadForm :public CFrameWnd
{
public:
	NotepadForm();//생성자
	//인라인함수
	//CMenu& GetCMenu() const; GetMenu가 있어서 따로 CMenu를 구하기 위한 인라인함수가 필요없음
	Font& GetFont() const;
	Caret& GetCaret() const;
public:
	//주소는 public으로
	Glyph* note;
	Glyph* current;
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
	//afx_msg void OnFileOpenClicked();
	//afx_msg void OnFileSaveClicked();
	//afx_msg void OnFileSaveDifferentNameClicked();
	afx_msg void OnCommand(UINT nID);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
private:
	CMenu menu;//menu가 notepadForm의 멤버로 있어야 OnCreate스택이 종료되어도
	//menu가 계속 notepadForm에 setMenu된채로 있게됨 단독적으로 CMenu가 있는 경우
	//OnCreate 스택이 종료되면 CMenu가 사라지기때문에 뻑이남.
	Font font;//레코드<<entity>>
	Caret caret;//레코드<<entity>>
	//File file;//레코드<<entity>> file은 Command에 dependency(의존화)관계를 맺으면됨
	//즉, File은 ConcreteCommand에서 필요할 때 쓰고 버리는 변수로 선언해서 사용하면 됨!!!
	//Command command; NotepadForm은 Command나 CommandCreator를 가질 필요 X
	//Command랑 CommandCreator 클래스가 notepadForm의 주소를 가지고 있으면 됨(연관화)
};

//인라인함수정의
inline Font& NotepadForm::GetFont() const
{
	return const_cast<Font&>(this->font);
}

inline Caret& NotepadForm::GetCaret() const
{
	return const_cast<Caret&>(this->caret);
}

//함수선언(함수포인터느낌)
LRESULT CALLBACK SaveMessageBoxProc(int nCode, WPARAM wParam, LPARAM lParam);
int SaveMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT nType);

#endif // !_NOTEPADFORM_H
