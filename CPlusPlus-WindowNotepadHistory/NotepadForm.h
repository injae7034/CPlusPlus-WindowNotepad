#ifndef _NOTEPADFORM_H
#define _NOTEPADFORM_H

#include "Glyph.h"
#include "Font.h"
#include "Caret.h"
#include "File.h"
#include<afxwin.h>//CDialog 헤더파일


typedef signed long int Long;

class NotepadForm :public CFrameWnd
{
public:
	NotepadForm();//생성자
	//~NotepadForm();//소멸자
	//인라인함수
	bool GetIsComposing() const;
	Font& GetFont() const;
	Caret& GetCaret() const;
public:
	Glyph* note;
	Glyph* current;
protected://#
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg LRESULT OnStartCompostion(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnComposition(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnImeChar(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT OnSetContext(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
private:
	bool IsComposing;
	Font font;//레코드<<entity>>
	Caret caret;//레코드<<entity>>
	File file;//레코드<<entity>>
};

//인라인함수정의
inline bool NotepadForm::GetIsComposing() const
{
	return this->IsComposing;
}

inline Font& NotepadForm::GetFont() const
{
	return const_cast<Font&>(this->font);
}

inline Caret& NotepadForm::GetCaret() const
{
	return const_cast<Caret&>(this->caret);
}
#endif // !_NOTEPADFORM_H
