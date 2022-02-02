#ifndef _NOTEPADFORM_H
#define _NOTEPADFORM_H

#include "Glyph.h"
#include<afxwin.h>//CDialog 헤더파일

class Note;//전방선언 Note가 전방선언으로 사용됨.
typedef signed long int Long;

class NotepadForm :public CFrameWnd
{
public:
	NotepadForm();//생성자
	~NotepadForm();//소멸자
public:
	Glyph* note;//주소이므로 전방선언이 필요함.
protected://#
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

#endif // !_NOTEPADFORM_H
