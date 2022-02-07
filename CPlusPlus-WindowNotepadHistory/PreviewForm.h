#ifndef _PREVIEWFORM_H
#define _PREVIEWFORM_H

#include<afxwin.h>//CDialog �������

typedef signed long int Long;
class NotepadForm;

class PreviewForm :public CFrameWnd
{
public:
	NotepadForm* notepadForm;
public:
	PreviewForm(NotepadForm* notepadForm);//������
	//�ζ����Լ� ����
	Long GetOriginalRectWidth() const;
	Long GetOriginalRectHeight() const;
protected://#
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
private:
	double originalRectWidth;
	double originalRectHeight;
	CButton button;
};

//�ζ����Լ�����
inline Long PreviewForm::GetOriginalRectWidth() const
{
	return this->originalRectWidth;
}
inline Long PreviewForm::GetOriginalRectHeight() const
{
	return this->originalRectHeight;
}

#endif // !_PREVIEWFORM_H
