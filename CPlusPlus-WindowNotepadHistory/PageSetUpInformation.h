#ifndef _PAGESETUPINFORMATION_H
#define _PAGESETUPINFORMATION_H

#include <afxwin.h>//CRect����� ���ؼ� �ʿ���!
typedef signed long int Long;

class PageSetUpInformation {
public:
	PageSetUpInformation(CRect printableRect, Long paperSize, CString header, CString footer,
		Long orientation);
	~PageSetUpInformation();
	//�ζ����Լ�
	CRect& GetPrintableRect() const;
	Long GetPaperSize() const;
	CString GetHeader() const;
	CString GetFooter() const;
	Long GetOrientation() const;
private:
	CRect printableRect;
	Long paperSize;
	CString header;
	CString footer;
	Long orientation;
};

inline CRect& PageSetUpInformation::GetPrintableRect() const
{
	return const_cast<CRect&>(this->printableRect);
}

inline Long PageSetUpInformation::GetPaperSize() const
{
	return this->paperSize;
}

inline CString PageSetUpInformation::GetHeader() const
{
	return this->header;
}

inline CString PageSetUpInformation::GetFooter() const
{
	return this->footer;
}

inline Long PageSetUpInformation::GetOrientation() const
{
	return this->orientation;
}

#endif // !_PAGESETUPINFORMATION_H
