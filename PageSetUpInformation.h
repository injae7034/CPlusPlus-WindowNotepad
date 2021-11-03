#ifndef _PAGESETUPINFORMATION_H
#define _PAGESETUPINFORMATION_H

#include <afxwin.h>//CRect����� ���ؼ� �ʿ���!
typedef signed long int Long;

class PageSetUpInformation {
public:
	PageSetUpInformation(CRect printableRect, Long paperSize, CString header, CString footer,
		Long orientation, CString devName);
	~PageSetUpInformation();
	//�ζ����Լ�
	CRect& GetPrintableRect() const;
	Long GetPaperSize() const;
	CString& GetHeader() const;
	CString& GetFooter() const;
	Long GetOrientation() const;
	CString& GetDevName() const;
private:
	CRect printableRect;
	Long paperSize;
	CString header;
	CString footer;
	Long orientation;
	CString devName;
};

inline CRect& PageSetUpInformation::GetPrintableRect() const
{
	return const_cast<CRect&>(this->printableRect);
}

inline Long PageSetUpInformation::GetPaperSize() const
{
	return this->paperSize;
}

inline CString& PageSetUpInformation::GetHeader() const
{
	return const_cast<CString&>(this->header);
}

inline CString& PageSetUpInformation::GetFooter() const
{
	return const_cast<CString&>(this->footer);
}

inline Long PageSetUpInformation::GetOrientation() const
{
	return this->orientation;
}
inline CString& PageSetUpInformation::GetDevName() const
{
	return const_cast<CString&>(this->devName);
}

#endif // !_PAGESETUPINFORMATION_H
