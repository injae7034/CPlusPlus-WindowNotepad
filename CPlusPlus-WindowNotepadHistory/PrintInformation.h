#ifndef _PRINTINFORMATION_H
#define _PRINTINFORMATION_H

#include <afxwin.h>
typedef signed long int Long;
class NotepadForm;
class Glyph;
class TextExtent;
class Font;

class PrintInformation {
public:
	NotepadForm* notepadForm;
public:
	PrintInformation(NotepadForm* notepadForm, LOGFONT printLogFont, CDC* printerDC = 0);
	~PrintInformation();
	HBITMAP CreateBitmap();
	//인라인함수
	Glyph* GetPrintNote() const;
	LOGFONT& GetPrintLogFont() const;
	CDC* GetPrintDC() const;
	Long GetPageRowCount() const;
	Long GetPrintableAreaHeight() const;
private:
	Glyph* printNote;
	LOGFONT printLogFont;
	CDC* printDC;
	Long pageRowCount;
	Long printableAreaHeight;
};

inline Glyph* PrintInformation::GetPrintNote() const
{
	return const_cast<Glyph*>(this->printNote);
}
inline LOGFONT& PrintInformation::GetPrintLogFont() const
{
	return const_cast<LOGFONT&>(this->printLogFont);
}
inline CDC* PrintInformation::GetPrintDC() const
{
	return const_cast<CDC*>(this->printDC);
}
inline Long PrintInformation::GetPageRowCount() const
{
	return this->pageRowCount;
}
inline Long PrintInformation::GetPrintableAreaHeight() const
{
	return this->printableAreaHeight;
}
#endif // !_PRINTINFORMATION_H
