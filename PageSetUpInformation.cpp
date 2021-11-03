#include "PageSetUpInformation.h"

PageSetUpInformation::PageSetUpInformation(CRect printableRect, Long paperSize,
	CString header, CString footer, Long orientation, CString devName)
{
	this->printableRect = printableRect;
	this->paperSize = paperSize;
	this->header = header;
	this->footer = footer;
	this->orientation = orientation;
	this->devName = devName;
}

PageSetUpInformation::~PageSetUpInformation()
{
	
}