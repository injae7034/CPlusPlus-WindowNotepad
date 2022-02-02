#ifndef _NOTEPADFORM_H
#define _NOTEPADFORM_H

#include "Glyph.h"
#include "Font.h"
#include "Caret.h"
#include "resource.h"
#include "Command.h"
#include<afxwin.h>//CDialog �������


typedef signed long int Long;

class NotepadForm :public CFrameWnd
{
public:
	NotepadForm();//������
	//�ζ����Լ�
	//CMenu& GetCMenu() const; GetMenu�� �־ ���� CMenu�� ���ϱ� ���� �ζ����Լ��� �ʿ����
	Font& GetFont() const;
	Caret& GetCaret() const;
public:
	//�ּҴ� public����
	Glyph* note;
	Glyph* current;
	//flag�� public����
	bool IsComposing;//�ѱ��� ���������� �ƴ��� �Ǻ��ϱ� ���� flag
	bool IsDirty;//���������� �ƴ��� �Ǻ��ϱ� ���� flag
	string fileName;//���� �����ִ� �޸����� ���ϸ�
	string filePath;//���� �����ִ� �޸����� ���ϰ��
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
	CMenu menu;//menu�� notepadForm�� ����� �־�� OnCreate������ ����Ǿ
	//menu�� ��� notepadForm�� setMenu��ä�� �ְԵ� �ܵ������� CMenu�� �ִ� ���
	//OnCreate ������ ����Ǹ� CMenu�� ������⶧���� ���̳�.
	Font font;//���ڵ�<<entity>>
	Caret caret;//���ڵ�<<entity>>
	//File file;//���ڵ�<<entity>> file�� Command�� dependency(����ȭ)���踦 �������
	//��, File�� ConcreteCommand���� �ʿ��� �� ���� ������ ������ �����ؼ� ����ϸ� ��!!!
	//Command command; NotepadForm�� Command�� CommandCreator�� ���� �ʿ� X
	//Command�� CommandCreator Ŭ������ notepadForm�� �ּҸ� ������ ������ ��(����ȭ)
};

//�ζ����Լ�����
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
