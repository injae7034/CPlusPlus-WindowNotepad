#ifndef _NOTEPADFORM_H
#define _NOTEPADFORM_H

#include "Glyph.h"
#include<afxwin.h>//CDialog �������

class Note;//���漱�� Note�� ���漱������ ����.
typedef signed long int Long;

class NotepadForm :public CFrameWnd
{
public:
	NotepadForm();//������
	~NotepadForm();//�Ҹ���
public:
	Glyph* note;//�ּ��̹Ƿ� ���漱���� �ʿ���.
	Glyph* current;//�ּ��̹Ƿ� ���漱�� �ʿ���.
protected://#
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnStartCompostion(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnComposition(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnImeChar(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT OnSetContext(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
private:
	bool IsComposing;
};

#endif // !_NOTEPADFORM_H
