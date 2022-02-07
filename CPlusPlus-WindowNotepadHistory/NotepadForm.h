#ifndef _NOTEPADFORM_H
#define _NOTEPADFORM_H

#include "afxdlgs.h"//CCommonDialog�������(CFindReplaceDialog�� �̿��Ϸ��� �ʿ���.)
#include "Subject.h"
#include "Font.h"
#include "resource.h"
#include<afxwin.h>//CDialog �������
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

typedef signed long int Long;

class NotepadForm :public CFrameWnd, public Subject//���߻��
{
public:
	NotepadForm();//������
	//CMenu& GetCMenu() const; GetMenu�� �־ ���� CMenu�� ���ϱ� ���� �ζ����Լ��� �ʿ����
public:
	//�ּҴ� public����
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
	
	//flag�� public����
	bool isRowAutoChanging;//�ڵ������� �Ǵ� ������ �ƴ��� �Ǻ��ϱ� ���� flag
	bool isSelecting;//�ؽ�Ʈ ������ �ϴ� ������ �ƴ��� �Ǻ��ϱ� ���� flag
	bool isComposing;//�ѱ��� ���������� �ƴ��� �Ǻ��ϱ� ���� flag
	bool isDirty;//���������� �ƴ��� �Ǻ��ϱ� ���� flag
	string fileName;//���� �����ִ� �޸����� ���ϸ�
	string filePath;//���� �����ִ� �޸����� ���ϰ��
	Long selectedStartXPos;//������ ���۵Ǵ� x��ǥ
	Long selectedStartYPos;//������ ���۵Ǵ� y��ǥ
	Long previousPageWidth;
	//�ζ����Լ� ����
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
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
private:
	//menu�� notepadForm�� ����� �־�� OnCreate������ ����Ǿ
	//menu�� ��� notepadForm�� setMenu��ä�� �ְԵ� �ܵ������� CMenu�� �ִ� ���
	//OnCreate ������ ����Ǹ� CMenu�� ������⶧���� ���̳�.
	CMenu menu;
	Glyph* glyph;//OnCharcommand�� OnImeCharCommand�� SingleByteLetter��
	//DoubleByteLetter�� �����ϱ� ���� �ӽ�����
	Glyph* removedSelectedTexts;//OnImeCharcommand�� ������ ���õ� ������ �����ϱ� ���� �ӽ�����
};

//�ζ����Լ�����
inline Glyph* NotepadForm::GetGlyph() const
{
	return this->glyph;
}
inline Glyph* NotepadForm::GetRemovedSelectedTexts() const
{
	return this->removedSelectedTexts;
}

//�Լ�����(�Լ������ʹ���)
LRESULT CALLBACK SaveMessageBoxProc(int nCode, WPARAM wParam, LPARAM lParam);
int SaveMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT nType);

#endif // !_NOTEPADFORM_H
