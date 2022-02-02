#ifndef _NOTEPADFORM_H
#define _NOTEPADFORM_H

#include "Glyph.h"
#include "Font.h"
#include "Caret.h"
#include "File.h"
#include "resource.h"
#include<afxwin.h>//CDialog �������


typedef signed long int Long;

class NotepadForm :public CFrameWnd
{
public:
	NotepadForm();//������
	//~NotepadForm();//�Ҹ���
	//�ζ����Լ�
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
	afx_msg void OnPaint();
	afx_msg void OnFileOpenClicked();
	afx_msg void OnFileSaveClicked();
	afx_msg void OnFileSaveDifferentNameClicked();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
private:
	CMenu menu;//menu�� notepadForm�� ����� �־�� OnCreate������ ����Ǿ
	//menu�� ��� notepadForm�� setMenu��ä�� �ְԵ� �ܵ������� CMenu�� �ִ� ���
	//OnCreate�� ����Ǹ� CMenu�� ������⶧���� ���̳�.
	bool IsComposing;
	Font font;//���ڵ�<<entity>>
	Caret caret;//���ڵ�<<entity>>
	File file;//���ڵ�<<entity>>
};

//�ζ����Լ�����
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

//�Լ�����(�Լ������ʹ���)
LRESULT CALLBACK SaveMessageBoxProc(int nCode, WPARAM wParam, LPARAM lParam);
int SaveMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT nType);

#endif // !_NOTEPADFORM_H
